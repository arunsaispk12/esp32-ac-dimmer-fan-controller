# Advanced Examples

This directory contains advanced usage examples for the ESP32 AC Dimmer Fan Controller.

⚠️ **HIGH VOLTAGE WARNING:** See [SAFETY.md](../../docs/SAFETY.md) before proceeding.

---

## Available Examples

### 1. Serial Control Interface

**File:** `serial_control.c` (to be implemented)

**Features:**
- Control fan speed via serial commands
- Real-time speed adjustment
- Status reporting
- Emergency stop command

**Commands:**
```
SPEED <0-100>  - Set fan speed
STATUS         - Get current status
STOP           - Emergency stop
START          - Resume operation
HELP           - Show available commands
```

**Use case:** Manual control during development and testing

---

### 2. Temperature-Based Control

**File:** `temperature_control.c` (to be implemented)

**Features:**
- NTC thermistor temperature sensing
- Automatic speed adjustment based on temperature
- Configurable temperature thresholds
- Thermal runaway protection

**Hardware additions:**
- NTC thermistor (10kΩ) on TRIAC heatsink
- Voltage divider for ADC input
- ESP32 ADC pin connection

**Temperature mapping:**
```
< 40°C: Fan at minimum speed (20%)
40-50°C: Linear ramp to 50%
50-60°C: Linear ramp to 75%
60-70°C: Linear ramp to 100%
> 80°C: Emergency shutdown
```

**Use case:** Automatic cooling system with thermal protection

---

### 3. WiFi Control

**File:** `wifi_control.c` (to be implemented)

**Features:**
- Web interface for fan control
- RESTful API
- MQTT integration
- Home Assistant compatible

**Endpoints:**
```
GET  /status           - Get current state
POST /speed            - Set speed {"speed": 50}
POST /power            - Power on/off {"power": true}
GET  /config           - Get configuration
POST /config           - Update configuration
```

**Use case:** Smart home integration

---

### 4. Multi-Zone Control

**File:** `multi_zone.c` (to be implemented)

**Features:**
- Control multiple fans independently
- Synchronized operation modes
- Group control
- Individual status monitoring

**Hardware modifications:**
- Duplicate TRIAC circuits
- Additional GPIO pins for triggers
- Shared zero-cross detection

**Use case:** Whole-house ventilation system

---

### 5. Power Monitoring

**File:** `power_monitoring.c` (to be implemented)

**Features:**
- Current sensing with ACS712
- Power calculation
- Energy usage tracking
- Overcurrent protection

**Hardware additions:**
- ACS712 current sensor module (5A or 20A version)
- ESP32 ADC input connection
- Data logging to SD card or cloud

**Metrics:**
- Real-time current (A)
- Power consumption (W)
- Energy usage (kWh)
- Power factor

**Use case:** Energy monitoring and analytics

---

### 6. PID-Based Speed Control

**File:** `pid_control.c` (to be implemented)

**Features:**
- PID controller for precise speed regulation
- Closed-loop control using current sensing
- Auto-tuning capability
- Disturbance rejection

**Benefits:**
- Maintains consistent speed under varying load
- Compensates for voltage fluctuations
- Smoother operation

**Use case:** Precision applications requiring stable fan speed

---

### 7. Safety Monitoring System

**File:** `safety_monitor.c` (to be implemented)

**Features:**
- Continuous isolation monitoring
- TRIAC temperature monitoring
- Zero-cross signal validation
- Automatic fault detection and shutdown
- Fault logging and reporting

**Safety checks:**
- Zero-cross frequency validation
- TRIAC trigger confirmation
- Temperature limits
- Current limits
- Isolation integrity

**Use case:** Critical installations requiring high reliability

---

### 8. Schedule-Based Control

**File:** `scheduler.c` (to be implemented)

**Features:**
- Time-based speed scheduling
- Multiple daily profiles
- Weekly schedules
- NTP time synchronization

**Example schedule:**
```
06:00-08:00: 75% (morning rush)
08:00-17:00: 30% (daytime)
17:00-22:00: 50% (evening)
22:00-06:00: 20% (night)
```

**Use case:** Automated ventilation based on occupancy patterns

---

## Implementation Status

| Example | Status | Complexity | Hardware Mods |
|---------|--------|------------|---------------|
| Serial Control | Planned | Low | None |
| Temperature Control | Planned | Medium | NTC thermistor |
| WiFi Control | Planned | High | None |
| Multi-Zone | Planned | High | Duplicate circuits |
| Power Monitoring | Planned | Medium | Current sensor |
| PID Control | Planned | High | Current sensor |
| Safety Monitor | Planned | Medium | Optional sensors |
| Scheduler | Planned | Medium | WiFi/RTC |

---

## How to Use Advanced Examples

### 1. Choose an Example

Select the example that matches your use case.

### 2. Check Hardware Requirements

Some examples require additional hardware:
- Temperature sensors
- Current sensors
- Additional GPIO pins
- WiFi connectivity

### 3. Build and Flash

```bash
cd firmware
# Copy example code to main/
cp ../examples/advanced/serial_control.c main/

idf.py build
idf.py -p /dev/ttyUSB0 flash
idf.py -p /dev/ttyUSB0 monitor
```

### 4. Configure

Most examples have configuration defines at the top of the file:

```c
#define TEMP_SENSOR_PIN     GPIO_NUM_36
#define HIGH_TEMP_THRESHOLD 70
#define LOW_TEMP_THRESHOLD  40
```

Edit these to match your setup.

---

## Combining Examples

You can combine multiple features:

**Example: Temperature + WiFi + Power Monitoring**

```c
void app_main(void)
{
    // Initialize base dimmer
    init_gpio();
    init_timer();

    // Initialize WiFi
    wifi_init();

    // Initialize temperature sensor
    temp_sensor_init();

    // Initialize current sensor
    current_sensor_init();

    // Create tasks
    xTaskCreate(temperature_control_task, "temp_ctrl", 4096, NULL, 5, NULL);
    xTaskCreate(wifi_server_task, "wifi", 8192, NULL, 5, NULL);
    xTaskCreate(power_monitor_task, "power", 4096, NULL, 5, NULL);
}
```

---

## Development Guidelines

### Adding Your Own Examples

1. **Create a new file** in `examples/advanced/`
2. **Document thoroughly** with comments
3. **Test extensively** with safety as priority
4. **Update this README** with your example
5. **Submit PR** following [CONTRIBUTING.md](../../CONTRIBUTING.md)

### Example Template

```c
/**
 * Example: [Name]
 *
 * Description: [What it does]
 *
 * Hardware Required:
 * - [List hardware]
 *
 * Connections:
 * - [Pin mappings]
 *
 * Usage:
 * - [How to use]
 *
 * Safety Notes:
 * - [Any safety considerations]
 */

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
// ... other includes

// Configuration
#define EXAMPLE_CONFIG_1    value
#define EXAMPLE_CONFIG_2    value

// ... implementation

void app_main(void)
{
    // Initialize
    // Create tasks
    // Main loop
}
```

---

## Testing Advanced Examples

### General Testing Procedure

1. **Bench test:** Test on workbench with lamp load
2. **Thermal test:** Monitor temperatures during extended operation
3. **Fault testing:** Test failure modes (sensor disconnection, etc.)
4. **Integration test:** Test with actual fan load
5. **Long-term test:** 24-hour continuous operation test

### Safety Testing

For advanced features affecting safety:

1. **Isolation test:** Verify isolation maintained (>10MΩ)
2. **Fault mode test:** Ensure safe shutdown on failures
3. **Overload test:** Verify protection circuits work
4. **Thermal test:** Ensure no overheating under all conditions

---

## Community Contributions

Have an advanced example to share?

1. Test it thoroughly
2. Document it well
3. Ensure it's safe
4. Submit a pull request

**Most wanted examples:**
- Bluetooth control
- Solar panel powered operation
- Battery backup integration
- Multiple sensor fusion
- Machine learning-based control

---

## Hardware Modifications for Advanced Features

### Adding Temperature Sensing

**NTC Thermistor Circuit:**
```
3.3V ----[10kΩ]----+---- ADC Pin (GPIO36)
                   |
                 [NTC]
                 10kΩ
                   |
                  GND
```

**Calculation:**
```c
float voltage = (adc_reading / 4095.0) * 3.3;
float resistance = 10000.0 * (3.3 / voltage - 1.0);
float temperature = beta_calculation(resistance);
```

### Adding Current Sensing

**ACS712 Connection:**
```
ACS712 Vout ---- ADC Pin (GPIO39)
ACS712 GND  ---- ESP32 GND
ACS712 VCC  ---- 5V (NOT 3.3V)

Note: ACS712 output is 0-5V, use voltage divider:
Vout ----[10kΩ]----+---- ADC Pin
                   |
                 [10kΩ]
                   |
                  GND
```

### Adding WiFi Antenna

**For metal enclosure:**
- Use external antenna
- Connector on enclosure
- Keep antenna away from mains traces

---

## Performance Considerations

### Task Priorities

Recommended task priorities (higher number = higher priority):

```c
// Critical timing
xTaskCreate(zero_cross_task, "zc", 2048, NULL, 10, NULL);

// Control loops
xTaskCreate(temperature_task, "temp", 4096, NULL, 5, NULL);
xTaskCreate(pid_control_task, "pid", 4096, NULL, 5, NULL);

// Communication
xTaskCreate(wifi_task, "wifi", 8192, NULL, 3, NULL);
xTaskCreate(mqtt_task, "mqtt", 8192, NULL, 3, NULL);

// Monitoring
xTaskCreate(power_monitor_task, "power", 4096, NULL, 2, NULL);
xTaskCreate(logger_task, "log", 4096, NULL, 1, NULL);
```

### Memory Considerations

Monitor stack usage:

```c
UBaseType_t uxHighWaterMark;
uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);
ESP_LOGI(TAG, "Stack remaining: %d bytes", uxHighWaterMark * sizeof(StackType_t));
```

### CPU Usage

Avoid blocking the timing-critical ISR:
- Keep ISR minimal
- Use queues/semaphores for data transfer
- Process data in tasks, not ISRs

---

## Debugging Advanced Features

### Enable Debug Logging

```c
esp_log_level_set("*", ESP_LOG_DEBUG);
esp_log_level_set("TAG", ESP_LOG_VERBOSE);
```

### Use Telemetry

Send debug data over serial:

```c
printf("TELEMETRY: speed=%d, temp=%0.1f, current=%0.2f\n",
       current_speed, temperature, current);
```

### Oscilloscope Triggers

For timing debug:
- CH1: Zero-cross
- CH2: TRIAC trigger
- CH3: ADC trigger
- CH4: Debug GPIO toggle

---

## Resources

- **ESP-IDF Documentation:** https://docs.espressif.com/projects/esp-idf/
- **FreeRTOS Documentation:** https://www.freertos.org/
- **Phase Control Theory:** See BUILD_GUIDE.md
- **Safety Guidelines:** See SAFETY.md

---

## Support

Need help with advanced examples?

1. Check example documentation
2. Review [TROUBLESHOOTING.md](../../docs/TROUBLESHOOTING.md)
3. Ask in [GitHub Discussions](https://github.com/yourusername/esp32-ac-dimmer-fan-controller/discussions)
4. Open an issue if you find a bug

---

**⚡ Advanced features require advanced safety awareness ⚡**

*Last Updated: 2025-01-11*
