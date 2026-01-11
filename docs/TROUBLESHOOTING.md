# Troubleshooting Guide

This guide covers common issues encountered when building and using the ESP32 AC Dimmer Fan Controller.

⚠️ **SAFETY FIRST:** Always disconnect power before troubleshooting. See [SAFETY.md](SAFETY.md).

---

## Table of Contents

1. [Zero-Cross Detection Issues](#zero-cross-detection-issues)
2. [TRIAC Won't Trigger](#triac-wont-trigger)
3. [Fan Hums or Vibrates](#fan-hums-or-vibrates)
4. [TRIAC Overheating](#triac-overheating)
5. [Erratic Behavior](#erratic-behavior)
6. [ESP32 Resets](#esp32-resets)
7. [Works with Lamp but Not Fan](#works-with-lamp-but-not-fan)
8. [Inconsistent Speed Control](#inconsistent-speed-control)
9. [Debugging Tools](#debugging-tools)
10. [When to Replace Components](#when-to-replace-components)

---

## Zero-Cross Detection Issues

### Symptom: No zero-cross signal detected

**Check serial monitor output:**
```
GPIO initialized successfully
Timer initialized successfully
Fan OFF
```

But no speed changes occur.

### Solutions:

#### 1. Verify Zero-Cross Circuit

**Test with oscilloscope:**
- Connect scope ground to ESP32 ground (NOT mains ground)
- Probe GPIO34
- Should see pulses at 100Hz (50Hz AC) or 120Hz (60Hz AC)
- Pulse width: ~1-5ms typical

**If no pulses seen:**

```
Power OFF → Check:
1. MOC3021 orientation (pin 1 marked)
2. Bridge rectifier polarity
3. 330Ω resistor value (should be 2W rated)
4. Solder joints on optocoupler
5. AC voltage present at circuit input
```

#### 2. Check GPIO Configuration

**Verify in code:**
```c
#define ZERO_CROSS_PIN GPIO_NUM_34  // Input-only pin on ESP32
```

**Common mistakes:**
- Using GPIO34 as output (impossible - it's input-only)
- Wrong pin number in code
- Not enabling pull-down (though external circuit provides bias)

#### 3. Test Zero-Cross Circuit Separately

**Isolation test:**
1. Disconnect GPIO34 from circuit
2. Use multimeter in DC voltage mode
3. Should read ~3.3V when optocoupler OFF
4. Should read ~0V when optocoupler ON (near zero-cross)
5. Should toggle 100 times per second (50Hz AC)

**If voltage doesn't change:**
- MOC3021 may be damaged
- Bridge rectifier may be incorrectly installed
- Input resistor may be wrong value (should be 330Ω, 2W)

#### 4. Software Verification

Add debug logging to ISR (careful - can affect timing):

```c
static void IRAM_ATTR zero_cross_isr_handler(void *arg)
{
    static uint32_t count = 0;
    count++;

    if (count % 100 == 0) {
        ESP_EARLY_LOGI(TAG, "ZC count: %lu", count);
    }

    // ... rest of ISR code
}
```

**Expected output:**
- Should see count incrementing rapidly
- 100 counts = 1 second (at 50Hz)
- 120 counts = 1 second (at 60Hz)

---

## TRIAC Won't Trigger

### Symptom: Zero-cross detected, but fan doesn't run

**Serial monitor shows:**
```
Fan speed set to 50% (delay: 5000 us)
```

But fan remains off.

### Solutions:

#### 1. Verify TRIAC Trigger Circuit

**Measure trigger signal:**
- Oscilloscope on GPIO25
- Should see brief pulses (50µs width)
- Pulses occur at phase delay time after zero-cross
- At 100% speed: pulses ~500µs after zero-cross
- At 50% speed: pulses ~5000µs after zero-cross

**If no pulses:**
```c
// Test GPIO directly
gpio_set_level(TRIAC_TRIGGER_PIN, 1);
vTaskDelay(pdMS_TO_TICKS(1000));
gpio_set_level(TRIAC_TRIGGER_PIN, 0);
```

Should see GPIO25 go high then low.

#### 2. Check Trigger Optocoupler

**Power OFF - Test MOC3021 (trigger side):**
1. Verify orientation (pin 1 marked)
2. Check 330Ω input resistor
3. Measure resistance across LED pins (1-2): should be ~50-100Ω
4. Measure resistance across output pins (4-6): should be >1MΩ when off

**Power ON - Test with multimeter:**
1. Measure voltage across input LED (pins 1-2): should be ~1.2V when triggered
2. If no voltage, GPIO25 may not be outputting correctly

#### 3. Check TRIAC

**Power OFF - Test BTA16:**
1. Measure resistance MT1 to MT2: should be >100kΩ (open)
2. Measure resistance gate to MT1: should be ~10-100Ω
3. If short circuit (low resistance MT1-MT2), TRIAC is damaged

**Test TRIAC triggering manually:**
```
DANGER: Mains voltage present. Use isolation transformer.

1. Remove ESP32 from circuit
2. Connect gate to MT1 through 1kΩ resistor
3. TRIAC should conduct (MT1-MT2 low resistance)
4. Remove gate connection
5. TRIAC should continue conducting until AC zero-cross
```

If TRIAC doesn't conduct, it's defective.

#### 4. Verify TRIAC Driver Circuit

**Check connections:**
- MOC3021 pin 6 → TRIAC gate via 180Ω resistor
- MOC3021 pin 4 → TRIAC MT1
- 10nF capacitor gate-MT1 properly installed
- 1kΩ pull-down resistor gate-MT1 present

**Common issues:**
- Missing 180Ω gate resistor
- Missing 10nF capacitor (causes erratic triggering)
- Missing 1kΩ pull-down (TRIAC may trigger randomly)

---

## Fan Hums or Vibrates

### Symptom: Fan makes buzzing/humming noise, especially at low speeds

This is normal to some degree with phase-control dimming, but excessive humming indicates issues.

### Solutions:

#### 1. Check Minimum Speed Setting

**Fans are unstable below ~20% speed:**

```c
#define MIN_SPEED_PERCENT 20  // Increase if humming persists
```

Try increasing to 25% or 30%.

#### 2. Verify Snubber Circuit

**Missing or incorrect snubber causes humming:**

```
Check:
- 47nF X2 capacitor across MT1-MT2
- 47Ω, 3W resistor in series with capacitor
- Both properly rated for voltage
```

**Test snubber:**
1. Power OFF
2. Measure capacitance: should read ~45-50nF
3. Measure resistance: should read ~47Ω
4. Check connections are solid

**If snubber missing:** TRIAC may be switching too abruptly, causing mechanical vibration in fan motor.

#### 3. Check Fan Compatibility

**Not all fans work with phase-control dimmers:**

✅ **Compatible:**
- Standard induction motor fans
- Single-phase AC motors
- Simple mechanical speed control fans
- Older ceiling fans

❌ **NOT Compatible:**
- Fans with electronic speed controllers
- BLDC (brushless DC) fans
- Fans with remote controls
- "Smart" fans
- Variable frequency drive (VFD) fans

**Test:** If fan has remote control or electronic buttons, it likely won't work with this dimmer.

#### 4. Try Different Trigger Pulse Width

**Some motors need longer pulses:**

```c
#define TRIAC_PULSE_WIDTH 100  // Increase from 50µs to 100µs
```

Longer pulses ensure TRIAC latches properly with inductive loads.

#### 5. Add Mechanical Vibration Damping

- Check fan mounting - should be secure
- Verify fan blades are balanced
- Check for loose screws in fan housing
- Some humming is unavoidable with phase control

---

## TRIAC Overheating

### Symptom: TRIAC heatsink very hot (>80°C) or thermal shutdown

### Solutions:

#### 1. Verify Heatsink Installation

**Check:**
- Heatsink properly attached with thermal paste
- TRIAC tab making full contact
- Mounting screw tight (but not over-tightened)
- Heatsink has adequate surface area (recommended: >25 cm²)
- Airflow around heatsink not blocked

**Thermal paste:**
- Apply thin, even layer
- Use silicone-based thermal compound
- Too much paste is worse than too little

#### 2. Check Load Current

**TRIAC is rated for 16A, but:**
- Derate to 60% for reliability = **9.6A maximum**
- At 240V: 9.6A × 240V = **2300W maximum**
- At 120V: 9.6A × 120V = **1150W maximum**

**Measure actual load current:**
```
With lamp load: I = P / V
  60W lamp at 120V: 60W / 120V = 0.5A ✓ OK
  200W fan at 120V: 200W / 120V = 1.67A ✓ OK
  1000W heater at 120V: 1000W / 120V = 8.3A ⚠️ High
```

**If current >8A:** Use larger TRIAC (BTA24 or BTA41) or parallel TRIACs.

#### 3. Verify Snubber Circuit

**Missing snubber → high dV/dt → increased switching losses → heat**

Must have:
- 47nF X2 capacitor
- 47Ω resistor (3W minimum)

#### 4. Check for Partial Conduction

**TRIAC not fully turning on → resistive losses → heat**

Causes:
- Insufficient gate current
- Missing gate pull-down resistor
- Damaged TRIAC

**Test:** Measure voltage drop across TRIAC when conducting:
- Should be <1.5V typically
- If >2V, TRIAC may be damaged or not fully triggered

#### 5. Improve Cooling

- Larger heatsink
- Add cooling fan
- Better airflow in enclosure
- Heat-conducting enclosure mounting

---

## Erratic Behavior

### Symptom: Unpredictable speed, random triggering, unstable operation

### Solutions:

#### 1. Check Power Supply to ESP32

**ESP32 needs clean, stable 3.3V or 5V:**

```
Measure with multimeter:
- Voltage: 4.9-5.1V (if using 5V) or 3.2-3.4V (if 3.3V)
- Ripple: <100mV peak-to-peak (use scope)
```

**If unstable power:**
- Add 100µF electrolytic + 100nF ceramic near ESP32
- Use regulated power supply, not direct from mains
- Ensure adequate current capacity (500mA minimum)

#### 2. Verify Isolation

**Mains noise coupling to ESP32 causes erratic behavior:**

**Test isolation:**
1. Disconnect ESP32 ground from circuit ground
2. Measure AC voltage between them
3. Should be <0.1V AC
4. If >1V AC, isolation is compromised

**Check:**
- Optocouplers properly installed
- No accidental ground connections
- PCB traces maintain proper clearance

#### 3. Check for Electrical Noise

**Add filtering:**

```
At ESP32 power input:
- 100µF electrolytic capacitor
- 100nF ceramic capacitor
- Optional: ferrite bead on power line

At GPIO pins:
- 100nF ceramic capacitor on GPIO34 (ZC input)
- Close to ESP32 pin
```

#### 4. Software Issues

**Verify timing:**

```c
// Enable debug logging
ESP_LOGI(TAG, "Delay: %lu us, Speed: %d%%", trigger_delay_us, current_speed);
```

**Common software issues:**
- Timer not stopping/restarting properly
- Race conditions in ISR
- Incorrect AC_HALF_PERIOD calculation
- Wrong AC_FREQUENCY setting (50Hz vs 60Hz)

#### 5. EMI/RFI Interference

**Reduce electromagnetic interference:**

- Shielded enclosure
- Proper grounding
- Keep mains wiring away from ESP32
- Use twisted pair for AC connections
- Add common-mode choke on AC input

---

## ESP32 Resets

### Symptom: ESP32 randomly resets, watchdog timer triggers

### Solutions:

#### 1. Power Supply Brownout

**Most common cause:**

```
Check:
- Power supply voltage stable
- Sufficient current capacity (1A recommended)
- Brownout detector level in sdkconfig
```

**Disable brownout detector for testing:**
```c
CONFIG_ESP_BROWNOUT_DET=n  // In sdkconfig
```

**If resets stop:** Power supply is inadequate.

#### 2. Watchdog Timer

**ISR taking too long:**

```c
// Remove debug logging from ISR
// Keep ISR execution time <100µs
```

**Increase watchdog timeout:**
```c
CONFIG_ESP_TASK_WDT_TIMEOUT_S=10
```

#### 3. Electrical Transients

**TRIAC switching creates voltage spikes:**

**Add protection:**
- MOV at ESP32 power input
- TVS diode array on GPIO pins
- Optocouplers should provide adequate isolation

#### 4. Ground Loops

**Ensure ESP32 ground is separate from mains ground:**

- Use optocouplers for isolation
- No direct connection between grounds
- Verify with multimeter resistance test (should be >1MΩ)

---

## Works with Lamp but Not Fan

### Symptom: Incandescent lamp dims perfectly, but fan doesn't run

### Causes:

#### 1. Inductive Load Requirements

**Fans require:**
- Longer trigger pulse (50-100µs vs 10µs for lamps)
- Snubber circuit (resistive loads don't need this)
- Higher holding current

**Verify in code:**
```c
#define TRIAC_PULSE_WIDTH 50  // Try increasing to 100
```

#### 2. Minimum Speed Too Low

**Fans need ~20% minimum to overcome inertia:**

```c
#define MIN_SPEED_PERCENT 30  // Increase from 20
```

#### 3. Fan Motor Type Incompatible

**Check fan type:**
- If fan has electronic controller → Won't work
- If fan has capacitor start → Should work
- If fan is brushless DC → Won't work

**Test:** Try different fan if possible.

#### 4. Insufficient Holding Current

**Some TRIACs need minimum holding current:**

BTA16 holding current: ~50mA typical

At low speed with light fan load, current may drop below holding current, causing TRIAC to turn off prematurely.

**Solutions:**
- Increase minimum speed
- Use TRIAC with lower holding current
- Add dummy load (small resistor) in parallel with fan

---

## Inconsistent Speed Control

### Symptom: Speed varies randomly, doesn't stay at set point

### Solutions:

#### 1. Check AC Frequency Setting

**Wrong frequency causes incorrect timing:**

```c
// For US/Japan (60Hz):
#define AC_FREQUENCY 60
#define AC_HALF_PERIOD 8333  // microseconds

// For EU/AU/Asia (50Hz):
#define AC_FREQUENCY 50
#define AC_HALF_PERIOD 10000  // microseconds
```

**Verify:** Use oscilloscope to measure zero-cross frequency.

#### 2. Timer Accuracy

**ESP timer drift:**

```c
// Ensure high-precision timing
CONFIG_ESP_DEFAULT_CPU_FREQ_MHZ=240  // Maximum CPU speed
CONFIG_FREERTOS_HZ=1000              // 1ms tick
```

#### 3. Zero-Cross Signal Quality

**Noisy zero-cross signal causes jitter:**

**Check with oscilloscope:**
- Clean, consistent pulses
- Minimal noise
- Consistent timing

**Add filtering:**
```
100nF capacitor across optocoupler output (pins 5-4)
Reduces noise on zero-cross signal
```

#### 4. Mechanical Load Variation

**Fan speed varies with load:**
- Dirty blades (unbalanced)
- Bearing wear
- Obstructed airflow

**Test with different fan.**

---

## Debugging Tools

### Essential Tools

#### 1. Serial Monitor

```bash
idf.py monitor -p /dev/ttyUSB0
```

**Look for:**
- Initialization messages
- Speed change confirmations
- Error messages
- Watchdog resets

#### 2. Oscilloscope

**Critical measurements:**

| Signal | Pin | Expected |
|--------|-----|----------|
| Zero-cross | GPIO34 | 100Hz pulses (50Hz AC), 3.3V logic |
| TRIAC trigger | GPIO25 | 50µs pulses at phase delay |
| AC waveform | TRIAC MT2 | Chopped sine wave |

**Triggering tips:**
- Trigger on zero-cross signal (GPIO34)
- Use single-shot capture for timing analysis

#### 3. Multimeter

**Key measurements (POWER OFF):**
- Resistance across TRIAC MT1-MT2: >100kΩ
- Capacitor values: 47nF, 10nF
- Resistor values: 330Ω, 180Ω, 47Ω, 1kΩ

**Key measurements (POWER ON - USE ISOLATION TRANSFORMER):**
- AC voltage at input
- DC voltage at ESP32: 3.3V or 5V
- Zero-cross signal: toggles between 0-3.3V

#### 4. Thermal Camera

**Hot spot detection:**
- TRIAC temperature: <60°C normal, <80°C maximum
- Resistor temperatures: should be warm but not hot
- Trace temperatures: uniform, no hot spots

#### 5. Insulation Tester (Megger)

**Test isolation:**
```
Mains side to ESP32 side: >10 MΩ @ 500V DC
```

**If <10 MΩ:** Isolation compromised, dangerous condition.

---

## When to Replace Components

### MOC3021 Optocouplers

**Replace if:**
- No output with valid input
- Shorted (low resistance across output)
- Visible damage or discoloration

**Lifetime:** 20,000+ hours typical, but can fail early with overvoltage.

### BTA16 TRIAC

**Replace if:**
- Shorted (low resistance MT1-MT2 when off)
- Won't trigger (no conduction with gate current)
- Excessive heat even with proper heatsink
- Visible damage, cracked package

**Lifetime:** 100,000+ cycles, but heat stress reduces life.

### Capacitors

**Replace if:**
- Capacitance <80% of rated value
- Visible bulging or leakage
- Discoloration
- Cracked body

**X2 capacitors:** Longer life, but should be tested annually.

### Resistors

**Replace if:**
- Value >10% out of tolerance
- Discolored or burned
- Cracked body
- Thermal damage

**High-wattage resistors:** Check for proper heat dissipation.

### MOV (Metal Oxide Varistor)

**Replace if:**
- Visible damage
- Discoloration
- Cracked
- After major surge event

**Lifetime:** Can fail open circuit after absorbing surges.

---

## Still Having Issues?

### Systematic Debugging Process

1. **Verify hardware:**
   - Visual inspection
   - Continuity tests
   - Component values
   - Solder joints

2. **Test in stages:**
   - Test power supply separately
   - Test zero-cross circuit separately
   - Test TRIAC trigger separately
   - Test with lamp before fan

3. **Simplify:**
   - Remove ESP32, test power circuit
   - Manual TRIAC triggering
   - Known-good fan for testing

4. **Compare with reference:**
   - Check schematic carefully
   - Verify all component values
   - Compare PCB layout with recommended design

5. **Get help:**
   - Post issue on GitHub with:
     - Clear description
     - Photos of circuit
     - Oscilloscope captures
     - Serial monitor output
     - Schematic used
     - Component list

---

## Quick Reference Table

| Symptom | Most Likely Cause | Quick Fix |
|---------|------------------|-----------|
| No zero-cross pulses | MOC3021 incorrect | Check orientation, power |
| TRIAC won't trigger | Gate circuit issue | Verify 180Ω resistor, connections |
| Fan hums | Missing snubber | Add 47nF + 47Ω snubber |
| TRIAC overheats | Poor heatsinking | Add thermal paste, larger heatsink |
| Erratic behavior | Power supply noise | Add capacitors, check isolation |
| ESP32 resets | Brownout | Improve power supply |
| Works with lamp only | Inductive load issue | Increase pulse width, add snubber |
| Speed inconsistent | Wrong AC frequency | Check AC_FREQUENCY define |

---

**Remember:** Safety first. If troubleshooting doesn't resolve the issue and you're not comfortable proceeding, consult a qualified electrician.

**Document your build:** Take photos, keep notes, maintain records for future troubleshooting.

---

*Last Updated: 2025-01-11*
