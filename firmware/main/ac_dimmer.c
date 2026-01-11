/**
 * ESP32 AC Dimmer Fan Controller
 *
 * DANGER: HIGH VOLTAGE PROJECT
 * This code controls AC mains voltage. Improper use can result in:
 * - Electric shock
 * - Fire
 * - Death
 *
 * Only qualified personnel should build and use this device.
 *
 * Copyright (c) 2025
 * Licensed under MIT License
 */

#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "esp_timer.h"
#include "esp_log.h"
#include "esp_system.h"

static const char *TAG = "AC_DIMMER";

/* ========================================
 * Hardware Configuration
 * ======================================== */
#define ZERO_CROSS_PIN      GPIO_NUM_34
#define TRIAC_TRIGGER_PIN   GPIO_NUM_25

/* ========================================
 * AC Timing Parameters
 * ======================================== */
#define AC_FREQUENCY        50          // Hz (change to 60 for US/JP)
#define AC_HALF_PERIOD      10000       // microseconds (1000000 / AC_FREQUENCY / 2)
#define TRIAC_PULSE_WIDTH   50          // microseconds (longer pulse for inductive loads)
#define MIN_DELAY           500         // microseconds (safety margin)
#define MAX_DELAY           9000        // microseconds (safety margin)

/* ========================================
 * Fan Control Parameters
 * ======================================== */
#define MIN_SPEED_PERCENT   20          // Don't go below 20% for fan stability
#define MAX_SPEED_PERCENT   100
#define DEFAULT_SPEED       50

/* ========================================
 * Global Variables
 * ======================================== */
static esp_timer_handle_t triac_timer = NULL;
static volatile uint8_t current_speed = 0;
static volatile uint32_t trigger_delay_us = AC_HALF_PERIOD;
static volatile bool dimmer_enabled = false;
static portMUX_TYPE spinlock = portMUX_INITIALIZER_UNLOCKED;

/* ========================================
 * Forward Declarations
 * ======================================== */
static void IRAM_ATTR zero_cross_isr_handler(void *arg);
static void IRAM_ATTR triac_timer_callback(void *arg);
static uint32_t speed_to_delay(uint8_t speed_percent);
static void init_gpio(void);
static void init_timer(void);
static void set_fan_speed(uint8_t speed_percent);
static void fan_control_demo_task(void *pvParameters);

/* ========================================
 * Speed to Phase Delay Conversion
 * ======================================== */
/**
 * Convert speed percentage to phase delay in microseconds
 *
 * Phase control: Higher speed = shorter delay = more power
 * Speed 0%   -> Full delay (9000us) -> Minimum power
 * Speed 100% -> Min delay (500us)   -> Maximum power
 *
 * @param speed_percent: Fan speed (0-100%)
 * @return delay in microseconds
 */
static uint32_t speed_to_delay(uint8_t speed_percent)
{
    if (speed_percent == 0) {
        return AC_HALF_PERIOD; // Off
    }

    // Enforce minimum speed for fan stability
    if (speed_percent < MIN_SPEED_PERCENT) {
        speed_percent = MIN_SPEED_PERCENT;
    }

    if (speed_percent > MAX_SPEED_PERCENT) {
        speed_percent = MAX_SPEED_PERCENT;
    }

    // Linear mapping: speed 100% -> MIN_DELAY, speed MIN% -> MAX_DELAY
    uint32_t delay = MAX_DELAY - ((MAX_DELAY - MIN_DELAY) * speed_percent / 100);

    return delay;
}

/* ========================================
 * TRIAC Timer Callback
 * ======================================== */
/**
 * Timer callback to trigger TRIAC
 * This runs after the calculated phase delay from zero-crossing
 *
 * IRAM_ATTR: Function must be in IRAM for timing accuracy
 */
static void IRAM_ATTR triac_timer_callback(void *arg)
{
    // Trigger TRIAC with a short pulse
    gpio_set_level(TRIAC_TRIGGER_PIN, 1);

    // Keep pulse high for TRIAC_PULSE_WIDTH microseconds
    // For inductive loads, we need a longer pulse (50us typical)
    ets_delay_us(TRIAC_PULSE_WIDTH);

    gpio_set_level(TRIAC_TRIGGER_PIN, 0);
}

/* ========================================
 * Zero-Cross Interrupt Handler
 * ======================================== */
/**
 * ISR triggered on each zero-crossing of AC waveform
 * Starts timer to trigger TRIAC after calculated delay
 *
 * IRAM_ATTR: ISR must be in IRAM for performance
 */
static void IRAM_ATTR zero_cross_isr_handler(void *arg)
{
    if (!dimmer_enabled) {
        return;
    }

    // Start one-shot timer for TRIAC trigger
    // This provides the phase delay for power control
    if (trigger_delay_us < AC_HALF_PERIOD) {
        esp_timer_start_once(triac_timer, trigger_delay_us);
    }
}

/* ========================================
 * GPIO Initialization
 * ======================================== */
static void init_gpio(void)
{
    ESP_LOGI(TAG, "Initializing GPIO...");

    // Configure TRIAC trigger pin as output
    gpio_config_t triac_conf = {
        .pin_bit_mask = (1ULL << TRIAC_TRIGGER_PIN),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&triac_conf);
    gpio_set_level(TRIAC_TRIGGER_PIN, 0);

    // Configure zero-cross detection pin as input with interrupt
    gpio_config_t zc_conf = {
        .pin_bit_mask = (1ULL << ZERO_CROSS_PIN),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_NEGEDGE  // Trigger on falling edge
    };
    gpio_config(&zc_conf);

    // Install GPIO ISR service
    gpio_install_isr_service(0);

    // Attach ISR handler to zero-cross pin
    gpio_isr_handler_add(ZERO_CROSS_PIN, zero_cross_isr_handler, NULL);

    ESP_LOGI(TAG, "GPIO initialized successfully");
}

/* ========================================
 * Timer Initialization
 * ======================================== */
static void init_timer(void)
{
    ESP_LOGI(TAG, "Initializing ESP timer...");

    // Create high-resolution timer for TRIAC triggering
    esp_timer_create_args_t timer_args = {
        .callback = &triac_timer_callback,
        .arg = NULL,
        .dispatch_method = ESP_TIMER_TASK,
        .name = "triac_timer"
    };

    esp_err_t ret = esp_timer_create(&timer_args, &triac_timer);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to create timer: %s", esp_err_to_name(ret));
        return;
    }

    ESP_LOGI(TAG, "Timer initialized successfully");
}

/* ========================================
 * Set Fan Speed (Main API)
 * ======================================== */
/**
 * Set fan speed as percentage
 *
 * @param speed_percent: Target speed (0-100%)
 *                       0 = OFF
 *                       1-19 = Forced to 20% (minimum safe speed)
 *                       20-100 = Linear speed control
 */
static void set_fan_speed(uint8_t speed_percent)
{
    portENTER_CRITICAL(&spinlock);

    if (speed_percent == 0) {
        // Turn off dimmer
        dimmer_enabled = false;
        current_speed = 0;
        trigger_delay_us = AC_HALF_PERIOD;
        ESP_LOGI(TAG, "Fan OFF");
    } else {
        // Enforce minimum speed
        if (speed_percent < MIN_SPEED_PERCENT) {
            ESP_LOGW(TAG, "Speed %d%% below minimum, setting to %d%%",
                     speed_percent, MIN_SPEED_PERCENT);
            speed_percent = MIN_SPEED_PERCENT;
        }

        current_speed = speed_percent;
        trigger_delay_us = speed_to_delay(speed_percent);
        dimmer_enabled = true;

        ESP_LOGI(TAG, "Fan speed set to %d%% (delay: %lu us)",
                 current_speed, trigger_delay_us);
    }

    portEXIT_CRITICAL(&spinlock);
}

/* ========================================
 * Demo Task
 * ======================================== */
/**
 * Demo task that cycles through different fan speeds
 * This demonstrates the dimmer functionality
 */
static void fan_control_demo_task(void *pvParameters)
{
    ESP_LOGI(TAG, "Starting fan control demo");
    ESP_LOGI(TAG, "AC Frequency: %d Hz, Half Period: %d us", AC_FREQUENCY, AC_HALF_PERIOD);

    // Wait for system to stabilize
    vTaskDelay(pdMS_TO_TICKS(2000));

    // Demo sequence: cycle through different speeds
    uint8_t demo_speeds[] = {0, 30, 50, 75, 100};
    uint8_t num_speeds = sizeof(demo_speeds) / sizeof(demo_speeds[0]);

    while (1) {
        for (int i = 0; i < num_speeds; i++) {
            ESP_LOGI(TAG, "=== Setting speed to %d%% ===", demo_speeds[i]);
            set_fan_speed(demo_speeds[i]);

            // Hold this speed for 10 seconds
            vTaskDelay(pdMS_TO_TICKS(10000));
        }
    }
}

/* ========================================
 * Application Entry Point
 * ======================================== */
void app_main(void)
{
    ESP_LOGI(TAG, "========================================");
    ESP_LOGI(TAG, "ESP32 AC Dimmer Fan Controller");
    ESP_LOGI(TAG, "WARNING: HIGH VOLTAGE DEVICE");
    ESP_LOGI(TAG, "========================================");

    // Initialize hardware
    init_gpio();
    init_timer();

    // Start with fan off
    set_fan_speed(0);

    ESP_LOGI(TAG, "Initialization complete");
    ESP_LOGI(TAG, "Zero-cross pin: GPIO%d", ZERO_CROSS_PIN);
    ESP_LOGI(TAG, "TRIAC trigger pin: GPIO%d", TRIAC_TRIGGER_PIN);
    ESP_LOGI(TAG, "Min speed: %d%%", MIN_SPEED_PERCENT);

    // Create demo task
    xTaskCreate(fan_control_demo_task, "fan_demo", 4096, NULL, 5, NULL);

    ESP_LOGI(TAG, "System ready");
}
