# Basic Fan Control Example

This example demonstrates basic usage of the ESP32 AC Dimmer Fan Controller.

⚠️ **HIGH VOLTAGE WARNING:** See [SAFETY.md](../../docs/SAFETY.md) before building or testing.

---

## What This Example Does

1. Initializes the AC dimmer hardware
2. Cycles through preset fan speeds
3. Demonstrates the main API functions

**Speed Sequence:**
- 0% (OFF) for 10 seconds
- 30% (Low speed) for 10 seconds
- 50% (Medium speed) for 10 seconds
- 75% (High speed) for 10 seconds
- 100% (Full speed) for 10 seconds
- Repeat

---

## Hardware Required

- ESP32 development board
- Completed AC dimmer circuit (see [BUILD_GUIDE.md](../../docs/guides/BUILD_GUIDE.md))
- Fan load (or incandescent lamp for testing)
- Proper safety equipment

**Pin Configuration:**
- GPIO34 → Zero-cross detection input
- GPIO25 → TRIAC trigger output

---

## Building and Flashing

```bash
# Navigate to the firmware directory
cd ../../firmware

# Configure project (if needed)
idf.py menuconfig

# Build
idf.py build

# Flash to ESP32
idf.py -p /dev/ttyUSB0 flash

# Monitor output
idf.py -p /dev/ttyUSB0 monitor
```

---

## Expected Serial Output

```
ESP32 AC Dimmer Fan Controller
WARNING: HIGH VOLTAGE DEVICE
========================================
Initializing GPIO...
GPIO initialized successfully
Initializing ESP timer...
Timer initialized successfully
Fan OFF
Initialization complete
Zero-cross pin: GPIO34
TRIAC trigger pin: GPIO25
Min speed: 20%
System ready
Starting fan control demo
AC Frequency: 50 Hz, Half Period: 10000 us

=== Setting speed to 0% ===
Fan OFF

=== Setting speed to 30% ===
Fan speed set to 30% (delay: 7000 us)

=== Setting speed to 50% ===
Fan speed set to 50% (delay: 5000 us)

=== Setting speed to 75% ===
Fan speed set to 75% (delay: 2500 us)

=== Setting speed to 100% ===
Fan speed set to 100% (delay: 500 us)

(Cycle repeats)
```

---

## Code Explanation

### Main Components

#### 1. Initialization

```c
void app_main(void)
{
    // Initialize GPIO pins (zero-cross input, TRIAC output)
    init_gpio();

    // Initialize high-resolution timer for TRIAC triggering
    init_timer();

    // Start with fan off
    set_fan_speed(0);
}
```

#### 2. Setting Fan Speed

```c
// Set fan to 50%
set_fan_speed(50);

// Fan speed range: 0-100%
// 0 = OFF
// 1-19 = Automatically increased to 20% (minimum safe speed)
// 20-100 = Linear speed control
```

#### 3. Speed to Delay Conversion

The `speed_to_delay()` function converts percentage to phase delay:

```c
static uint32_t speed_to_delay(uint8_t speed_percent)
{
    // Higher speed = shorter delay = more power
    // 100% speed: 500us delay
    // 20% speed: 9000us delay
    // Linear interpolation between these points
}
```

#### 4. Zero-Cross Detection

```c
static void IRAM_ATTR zero_cross_isr_handler(void *arg)
{
    // Called every time AC voltage crosses zero (100 times/sec at 50Hz)
    // Starts timer to trigger TRIAC after calculated delay
    esp_timer_start_once(triac_timer, trigger_delay_us);
}
```

#### 5. TRIAC Triggering

```c
static void IRAM_ATTR triac_timer_callback(void *arg)
{
    // Trigger TRIAC with short pulse (50us)
    gpio_set_level(TRIAC_TRIGGER_PIN, 1);
    ets_delay_us(TRIAC_PULSE_WIDTH);
    gpio_set_level(TRIAC_TRIGGER_PIN, 0);
}
```

---

## Customization

### Change AC Frequency (60Hz)

Edit `firmware/main/ac_dimmer.c`:

```c
#define AC_FREQUENCY        60          // Change from 50 to 60
#define AC_HALF_PERIOD      8333        // Change from 10000 to 8333
```

### Adjust Minimum Speed

```c
#define MIN_SPEED_PERCENT   25          // Increase from 20 if needed
```

### Change Speed Sequence

Edit the `fan_control_demo_task()` function:

```c
uint8_t demo_speeds[] = {0, 40, 60, 80, 100};  // Custom sequence
```

### Adjust Hold Time

```c
vTaskDelay(pdMS_TO_TICKS(5000));  // Change from 10000 to 5000 for 5 seconds
```

---

## Troubleshooting

### No Speed Changes

**Check:**
- Zero-cross circuit connected to GPIO34
- Serial monitor shows "GPIO initialized successfully"
- Oscilloscope shows pulses on GPIO34

**Solutions:**
- See [TROUBLESHOOTING.md](../../docs/TROUBLESHOOTING.md#zero-cross-detection-issues)

### Fan Hums at Low Speeds

**Normal:** Some humming is expected with phase-control dimming

**If excessive:**
- Increase MIN_SPEED_PERCENT to 25 or 30
- Verify snubber circuit installed (47nF + 47Ω)
- Try different fan (some fans work better than others)

### TRIAC Overheats

**Check:**
- Heatsink properly installed with thermal paste
- Load current within ratings (<10A)
- Adequate airflow around heatsink

**Solutions:**
- See [TROUBLESHOOTING.md](../../docs/TROUBLESHOOTING.md#triac-overheating)

---

## Next Steps

Once basic control is working:

1. **Test with different speeds:**
   - Try manual speed changes
   - Test edge cases (0%, 1%, 19%, 100%)

2. **Monitor performance:**
   - Use oscilloscope to verify timing
   - Check thermal performance over time
   - Measure actual power consumption

3. **Try advanced examples:**
   - See `examples/advanced/` for more features
   - External control via serial commands
   - Temperature-based speed control

---

## Safety Checklist

Before running this example:

- [ ] Read SAFETY.md completely
- [ ] Hardware built according to BUILD_GUIDE.md
- [ ] Tested with incandescent lamp first
- [ ] Isolation verified (>10MΩ)
- [ ] Using isolation transformer
- [ ] Proper enclosure installed
- [ ] Fuse installed and correct rating
- [ ] Emergency shutdown accessible
- [ ] Fire extinguisher nearby
- [ ] Another person present

**If ANY checkbox is unchecked: DO NOT PROCEED**

---

## API Reference

### Functions

```c
/**
 * Initialize GPIO pins
 * Call once during startup
 */
void init_gpio(void);

/**
 * Initialize ESP timer
 * Call once during startup after init_gpio()
 */
void init_timer(void);

/**
 * Set fan speed
 *
 * @param speed_percent: Target speed (0-100%)
 *                       0 = OFF
 *                       1-19 = Clamped to MIN_SPEED_PERCENT
 *                       20-100 = Linear speed control
 */
void set_fan_speed(uint8_t speed_percent);

/**
 * Convert speed percentage to phase delay
 *
 * @param speed_percent: Fan speed (0-100%)
 * @return delay in microseconds
 */
uint32_t speed_to_delay(uint8_t speed_percent);
```

### Configuration Defines

```c
#define ZERO_CROSS_PIN      GPIO_NUM_34  // Zero-cross input
#define TRIAC_TRIGGER_PIN   GPIO_NUM_25  // TRIAC trigger output
#define AC_FREQUENCY        50           // AC frequency in Hz
#define AC_HALF_PERIOD      10000        // Microseconds per half-cycle
#define TRIAC_PULSE_WIDTH   50           // Trigger pulse width (us)
#define MIN_SPEED_PERCENT   20           // Minimum allowed speed
```

---

## Additional Resources

- [Main README](../../README.md) - Project overview
- [BUILD_GUIDE.md](../../docs/guides/BUILD_GUIDE.md) - Hardware construction
- [TROUBLESHOOTING.md](../../docs/TROUBLESHOOTING.md) - Common issues
- [SAFETY.md](../../docs/SAFETY.md) - Critical safety information

---

**⚡ Remember: This is HIGH VOLTAGE. Stay safe! ⚡**

*Example Version 1.0 - 2025-01-11*
