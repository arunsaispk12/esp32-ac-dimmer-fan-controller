# Complete Build Guide

# ESP32 AC Dimmer Fan Controller

⚠️ **DANGER: HIGH VOLTAGE PROJECT**

**STOP:** Before proceeding, read [SAFETY.md](../SAFETY.md) completely.

This guide provides complete instructions for building a professional AC phase-control dimmer for fan speed control.

---

## Table of Contents

1. [Understanding Inductive Loads](#understanding-inductive-loads)
2. [Complete Circuit Schematic](#complete-circuit-schematic)
3. [Component Selection](#component-selection)
4. [PCB Layout Guidelines](#pcb-layout-guidelines)
5. [Assembly Instructions](#assembly-instructions)
6. [Testing Procedures](#testing-procedures)
7. [Final Installation](#final-installation)

---

## Understanding Inductive Loads

### Why Fans are Different from Lamps

**Resistive loads (incandescent lamps):**
- Current in phase with voltage
- Simple TRIAC control works
- No special circuitry needed

**Inductive loads (fans, motors):**
- Current lags voltage by ~30-60°
- Generate back-EMF when switched
- Require:
  - **Snubber circuits** for dV/dt protection
  - **Longer trigger pulses** (50µs vs 10µs)
  - **Higher holding current** to keep TRIAC latched
  - **Gate protection** from voltage spikes

### Phase-Angle Control Principle

```
AC Sine Wave (50Hz = 10ms half-period)

      +Vpeak
        |    |
        |    |  Chopped portion
    ----+----+----------------------
    ^   |    |         ^
    |   |    |         |
  Zero  |    |       TRIAC
  Cross |    |       Triggers
        |    |
        -Vpeak

Phase Delay: 0-10ms (50Hz) or 0-8.33ms (60Hz)
Short delay = High power = High speed
Long delay = Low power = Low speed
```

### Why Snubber Circuits are Critical

**Without snubber:**
1. TRIAC switches → Sudden voltage change (high dV/dt)
2. Inductive load generates voltage spike
3. High dV/dt can:
   - **Falsely trigger TRIAC** at wrong time
   - **Damage TRIAC** (junction breakdown)
   - **Create EMI** (electromagnetic interference)
   - **Cause unstable operation**

**With snubber (RC network):**
1. Capacitor absorbs voltage spikes
2. Resistor limits current and prevents ringing
3. Controlled dV/dt protects TRIAC
4. Stable, reliable operation

**Typical values:**
- **Capacitor:** 47nF (X2 rated, 630V)
- **Resistor:** 47Ω (3W wire-wound)

---

## Complete Circuit Schematic

### Full Schematic Diagram

```
                                     ESP32
                                   +-------+
                                   |       |
AC MAINS IN                        | GPIO34| ← Zero-Cross
   L ----+---- FUSE 5A             |       |
         |                         | GPIO25| → TRIAC Trigger
         +---- MOV 275V             |       |
         |                         |  GND  |---+
         |                         +-------+   |
         |                                     |
    +----+----+                                |
    |         |                                |
  [LOAD]    [TRIAC]                            |
  (Fan)     BTA16-600B                         |
    |         |                                |
    |    MT2  |  MT1                           |
    |     |   |   |                            |
    |     |   +---+---- N (Neutral)            |
    |     |       |                            |
    |     |       |                            |
    |     |  +----+----+                       |
    |     |  | 47nF X2 |                       |
    |     |  | Snubber |                       |
    |     |  |   CAP   |                       |
    |     |  +----+----+                       |
    |     |       |                            |
    |     |     [47Ω]                          |
    |     |      3W                            |
    |     |       |                            |
    |     +-------+                            |
    |                                          |
    |     Gate                                 |
    |       |                                  |
    |     [180Ω]                               |
    |      1/2W                                |
    |       |                                  |
    |    +--+--+                               |
    |    |10nF |  (dV/dt protection)          |
    |    |     |                               |
    |    +--+--+                               |
    |       |                                  |
    +----[1kΩ]  (Gate pull-down)              |
           |                                   |
           |                                   |
    +------+------+  MOC3021                   |
    | 6         4 |  (TRIAC Driver)            |
    |             |                            |
    | 1         2 |                            |
    +---+-----+---+                            |
        |     |                                |
      [330Ω]  |                                |
       1/4W   |                                |
        |     |                                |
        +-----+---- GPIO25 (ESP32)             |
              |                                |
              +---- GND (ESP32) ---------------+


ZERO-CROSS DETECTION CIRCUIT:

AC MAINS IN
   L ----+
         |
    +----+----+  Bridge Rectifier
    |  1N4007 |  (4x diodes)
    | +     - |
    +----+----+
         |
      [330Ω]
       2W
         |
    +----+----+  MOC3021
    | 1     2 |  (Zero-Cross Detector)
    |         |
    | 5     4 |
    +----+----+
         |
      [10kΩ]
       1/4W
         |
         +-------- GPIO34 (ESP32)
         |
      [100nF]
       50V
         |
         +-------- GND (ESP32)

```

### Circuit Sections Explained

#### 1. Power Input Protection

```
L (Hot) ----[FUSE 5A]----+----[MOV 275V]---- N (Neutral)
                         |
                      [LOAD]
                      & TRIAC
```

- **Fuse:** 5A fast-blow protects against overcurrent
- **MOV:** 275V metal oxide varistor absorbs voltage spikes
- Both are critical safety components

#### 2. Zero-Cross Detection

**Purpose:** Detect when AC voltage crosses zero (100 times/second at 50Hz)

**Components:**
- **Bridge Rectifier (4× 1N4007):** Converts AC to pulsating DC
- **MOC3021 Optocoupler:** Isolates mains from ESP32
- **330Ω, 2W Resistor:** Limits current through optocoupler LED
- **10kΩ Pull-up:** Ensures clean logic signal
- **100nF Capacitor:** Filters noise

**Operation:**
1. AC voltage rectified to pulsating DC
2. Near zero-cross: voltage low → LED off → output HIGH (3.3V)
3. Away from zero: voltage high → LED on → output LOW (0V)
4. Creates 100Hz pulse train (50Hz AC × 2)

#### 3. TRIAC Trigger Circuit

**Purpose:** Optically isolated trigger signal to TRIAC gate

**Components:**
- **MOC3021 Optocoupler:** Isolates ESP32 from TRIAC/mains
- **330Ω, 1/4W Resistor:** Limits LED current (10mA)
- **180Ω, 1/2W Resistor:** Gate current limiting
- **1kΩ Pull-down:** Prevents false triggering
- **10nF Capacitor:** dV/dt protection

**Operation:**
1. ESP32 GPIO25 goes HIGH → MOC3021 LED on
2. Optocoupler output conducts
3. Current flows through 180Ω → TRIAC gate
4. TRIAC triggers and conducts

#### 4. TRIAC Power Switch

**Purpose:** Switch AC power to fan at precise phase angle

**Components:**
- **BTA16-600B TRIAC:** 16A, 600V bidirectional switch
- **Heatsink:** Dissipates switching losses
- **Thermal paste:** Improves heat transfer

**Operation:**
1. Gate receives trigger pulse
2. TRIAC latches on (conducts MT1 to MT2)
3. Remains on until current drops below holding current
4. Naturally turns off at AC zero-crossing
5. Repeats next half-cycle

#### 5. Snubber Network

**Purpose:** Protect TRIAC from inductive voltage spikes

**Components:**
- **47nF X2 Capacitor (630V):** Absorbs voltage transients
- **47Ω, 3W Resistor:** Damps resonance

**Why X2 rated:**
- X2 capacitors designed for across-mains application
- Self-healing metallized film
- Fail-safe (fail open, not short)
- Rated for continuous AC operation

**Operation:**
1. Inductive kick creates voltage spike
2. Capacitor absorbs spike (dV/dt limiting)
3. Resistor prevents oscillation
4. Protects TRIAC from overvoltage

---

## Component Selection

### Critical Components

#### 1. TRIAC - BTA16-600B

| Parameter | Specification | Notes |
|-----------|--------------|-------|
| Part Number | BTA16-600B | "B" suffix = isolated tab |
| Voltage Rating | 600V | Adequate for 240V AC with margin |
| Current Rating | 16A RMS | Derate to 10A for reliability |
| Package | TO-220 | Requires heatsink |
| Gate Trigger | <50mA | Easily driven by MOC3021 |
| Holding Current | ~50mA | Typical for motor loads |

**Alternatives:**
- **BTA24-600B:** 24A rating for higher power
- **BTA41-600B:** 41A rating for very high power
- **BT138-600:** Lower current (12A), cheaper

**DO NOT use:**
- TRIACs without "B" suffix (non-isolated tab dangerous)
- Logic-level TRIACs (different triggering)
- Lower voltage ratings (<600V)

#### 2. Optocouplers - MOC3021

| Parameter | Specification |
|-----------|--------------|
| Part Number | MOC3021 |
| Type | Zero-cross optocoupler |
| Isolation Voltage | 5000V RMS |
| LED Forward Voltage | ~1.2V |
| LED Forward Current | 10-15mA typical |
| Output Type | Random-phase (no zero-cross detection) |

**Important:** MOC3021 is **random-phase**, NOT zero-cross switching. We detect zero-cross separately and use MOC3021 just for isolation.

**DO NOT confuse with:**
- **MOC3041/MOC3043:** Zero-cross switching (delays trigger)
- **MOC3020:** Lower isolation (400V)

#### 3. Snubber Capacitor - 47nF X2

**Specifications:**
- **Capacitance:** 47nF ±10%
- **Type:** X2 rated (across-mains)
- **Voltage:** 630V AC minimum (250V AC rated is marginal)
- **Technology:** Metallized polypropylene film
- **Pitch:** 15mm or 22.5mm
- **Safety approvals:** UL, VDE, or equivalent

**Why X2:**
- Designed for continuous connection across mains
- Self-healing on dielectric breakdown
- Fail-open mode (safe failure)
- High dV/dt capability

**DO NOT use:**
- Ceramic capacitors (wrong dielectric)
- Electrolytic capacitors (will explode)
- Y-rated capacitors (line-to-ground only)
- Generic film caps without safety rating

#### 4. Snubber Resistor - 47Ω, 3W

**Specifications:**
- **Resistance:** 47Ω ±5%
- **Power Rating:** 3W minimum (5W preferred)
- **Type:** Wire-wound or metal oxide film
- **Temperature Coefficient:** <100 ppm/°C
- **Pulse handling:** Important for inductive loads

**Common packages:**
- Axial ceramic wire-wound
- TO-220 power resistor (heatsink mountable)
- Chassis-mount power resistor

**DO NOT use:**
- Carbon film resistors (inadequate power)
- 1/4W or 1/2W resistors (will burn out)
- High-value inductance wire-wound (defeats purpose)

#### 5. Bridge Rectifier Diodes - 1N4007

| Parameter | Specification |
|-----------|--------------|
| Part Number | 1N4007 |
| Voltage Rating | 1000V PIV |
| Current Rating | 1A average |
| Forward Voltage | ~0.7V |

**Quantity:** 4 diodes (full bridge)

**Connection:**
```
     D1
AC ---+---|>|---+--- DC+
      |          |
     D3         D4
      |          |
AC ---+---|>|---+--- DC-
     D2
```

**Alternative:** Use pre-packaged bridge rectifier (e.g., DB107, 1A 1000V)

#### 6. MOV - 275V

| Parameter | Specification |
|-----------|--------------|
| Type | Metal Oxide Varistor |
| Voltage | 275V AC (for 240V mains) |
| Energy Rating | 200J minimum |
| Diameter | 14mm typical |

**For 120V mains:** Use 150V MOV

**Operation:**
- Normal voltage: High resistance (>1MΩ)
- Surge voltage: Low resistance (clamps spike)
- Absorbs transient energy
- Eventually degrades and fails open

#### 7. Resistors

| Ref | Value | Power | Tolerance | Purpose |
|-----|-------|-------|-----------|---------|
| R1 | 330Ω | 2W | 5% | ZC optocoupler current limit |
| R2 | 10kΩ | 1/4W | 5% | ZC pull-up |
| R3 | 330Ω | 1/4W | 5% | Trigger optocoupler limit |
| R4 | 180Ω | 1/2W | 5% | TRIAC gate current |
| R5 | 1kΩ | 1/4W | 5% | TRIAC gate pull-down |
| R6 | 47Ω | 3W | 5% | Snubber damping |

**All resistors:**
- Metal film preferred for accuracy
- Wire-wound for high power (R1, R6)

#### 8. Capacitors

| Ref | Value | Voltage | Type | Purpose |
|-----|-------|---------|------|---------|
| C1 | 100nF | 50V | Ceramic | ZC noise filtering |
| C2 | 10nF | 630V | Ceramic | Gate dV/dt protection |
| C3 | 47nF | 630V | X2 Film | Snubber |

**X2 capacitor critical:** Must be safety-rated X2 type.

#### 9. Fuse

| Parameter | Specification |
|-----------|--------------|
| Current | 5A |
| Type | Fast-blow (F) |
| Voltage | 250V AC minimum |
| Breaking Capacity | 1500A minimum |

**Holder:** Panel-mount or PCB-mount, finger-safe

#### 10. Heatsink

**Thermal requirements:**

Assuming:
- TRIAC power dissipation: ~10W (conservative)
- Ambient temperature: 40°C
- Maximum junction: 125°C
- Thermal resistance junction-to-case: 1.5°C/W

**Required thermal resistance case-to-ambient:**
```
Rth_ca = (Tj_max - Ta) / P - Rth_jc
Rth_ca = (125 - 40) / 10 - 1.5
Rth_ca = 8.5 - 1.5 = 7°C/W
```

**Heatsink specification:**
- **Thermal resistance:** <7°C/W
- **Material:** Aluminum, black anodized
- **Surface area:** >25 cm²
- **Mounting:** TO-220 compatible
- **Thermal interface:** Thermal paste required

**Good examples:**
- Aavid 577102B00000G (6.2°C/W)
- Fischer SK 129 38 SA (5.5°C/W)
- Wakefield-Vette 403-37ABP (7°C/W)

---

## PCB Layout Guidelines

### Critical Design Rules

#### 1. Clearance and Creepage

**IEC 60950-1 requirements for 250V AC:**

| Parameter | Minimum | Recommended | Purpose |
|-----------|---------|-------------|---------|
| **Clearance** (through air) | 3.0mm | 4.0mm | Prevent arcing |
| **Creepage** (across surface) | 5.0mm | 8.0mm | Prevent tracking |
| **Isolation slot** | 1.0mm | 1.5mm | Break creepage path |

**Apply to:**
- Mains traces to low-voltage traces
- Primary to secondary on optocouplers
- TRIAC to ESP32 circuitry

#### 2. Trace Width

**Current carrying capacity (1oz copper, 10°C rise):**

| Current | Trace Width | Example |
|---------|-------------|---------|
| 1A | 0.4mm | Zero-cross circuit |
| 3A | 1.0mm | Low power loads |
| 5A | 2.0mm | Fan load |
| 10A | 4.0mm | High power loads |

**For TRIAC MT1, MT2 connections:** Use 2-4mm traces or polygon pours.

#### 3. Layer Stack

**2-Layer board (recommended for DIY):**

```
TOP LAYER:
- Mains voltage traces (hot, neutral)
- TRIAC connections
- High-voltage component pads
- Keepout zone for low voltage

BOTTOM LAYER:
- Low-voltage traces (ESP32, 3.3V)
- Ground plane (isolated from mains)
- Signal routing
```

**4-Layer board (professional):**
- Layer 1: Mains voltage routing
- Layer 2: Mains ground plane
- Layer 3: Low-voltage ground plane (ISOLATED)
- Layer 4: Low-voltage routing

#### 4. Component Placement

**Separation zones:**

```
+----------------------------------+
|  MAINS ZONE          |  SAFE ZONE |
|                      |            |
|  - Fuse              |  - ESP32   |
|  - MOV               |  - 3.3V    |
|  - TRIAC             |  - USB     |
|  - Snubber           |            |
|  - Load terminals    |            |
|                      |            |
|  ←--8mm minimum---→  |            |
|     isolation        |            |
+----------------------------------+
```

**Optocouplers:** Straddle the isolation barrier

#### 5. Thermal Management

- **TRIAC on edge of board** for heatsink clearance
- **Thermal relief pads** for TRIAC but NOT optocouplers
- **Copper pour under heatsink** for better heat spreading
- **Ventilation holes** if in enclosure

#### 6. Grounding

**CRITICAL: ESP32 ground MUST BE ISOLATED from mains earth/ground**

- No connection between mains ground and ESP32 ground
- Optocouplers provide isolation
- Test with megger: >10MΩ required

#### 7. Silkscreen

**Mark clearly:**
- ⚡ High voltage warnings
- L (Line), N (Neutral), Earth symbols
- Fuse rating
- Load rating
- Polarity of all components
- Test points
- Board version and date

---

## Assembly Instructions

### Tools Required

- Soldering iron (temperature controlled, 350°C)
- Solder (60/40 or lead-free)
- Wire cutters (flush cut)
- Wire strippers
- Multimeter (CAT III rated)
- Oscilloscope (optional but highly recommended)
- Heat shrink tubing
- Thermal paste
- Screwdriver set (insulated)
- Safety glasses

### Step-by-Step Assembly

#### Step 1: PCB Inspection

1. Inspect bare PCB for:
   - Shorts between traces (use multimeter)
   - Broken traces
   - Proper clearances
   - Manufacturing defects

2. Clean with isopropyl alcohol

#### Step 2: Component Placement Order

**Order is important for easy soldering:**

1. **Lowest profile first:** resistors, diodes
2. **Medium height:** ceramic capacitors, IC sockets (if used)
3. **Taller components:** optocouplers, electrolytic caps
4. **Last:** TRIAC, terminals, wire connections

#### Step 3: Install Resistors

```
Install in order:
1. R1 - 330Ω 2W (zero-cross)
2. R2 - 10kΩ 1/4W
3. R3 - 330Ω 1/4W
4. R4 - 180Ω 1/2W
5. R5 - 1kΩ 1/4W
6. R6 - 47Ω 3W (snubber)
```

**Technique:**
- Bend leads to fit holes
- Insert from top
- Solder on bottom
- Trim leads flush
- Verify value with multimeter BEFORE soldering

#### Step 4: Install Diodes (Bridge Rectifier)

**Polarity is CRITICAL:**

```
1N4007 diode marking:
  [===|>|===]
      ^
    Cathode (banded end)
```

1. Install D1 with cathode to +DC rail
2. Install D2 with anode to +DC rail
3. Install D3 with cathode to AC rail
4. Install D4 with anode to AC rail

**Double-check polarity before soldering!**

Alternatively, use bridge rectifier package (easier, less error-prone).

#### Step 5: Install Capacitors

1. **C1 (100nF ceramic):** No polarity, install near GPIO34
2. **C2 (10nF):** No polarity, install at TRIAC gate
3. **C3 (47nF X2):** No polarity, install across MT1-MT2

**Verify X2 rating on C3 - critical for safety**

#### Step 6: Install Optocouplers

**MOC3021 pinout (top view):**
```
    +---U---+
  1 |       | 6
  2 |       | 5
  3 |       | 4
    +-------+

Pin 1: Anode (marked with dot)
Pin 2: Cathode
Pin 3: NC
Pin 4: MT1 equivalent
Pin 5: NC
Pin 6: MT2 equivalent (gate driver)
```

**Install both MOC3021s with pin 1 toward marked indicator on PCB**

**DO NOT apply heat too long:** Optocouplers are heat-sensitive (3 seconds max per pin).

#### Step 7: Install TRIAC

**BTA16-600B pinout (facing front):**
```
    +-------+
    |  [B]  |  ← Metal tab (MT1 connection)
    |       |
    +-------+
    |  |  |
   MT2 G MT1
```

1. Apply thermal paste to metal tab
2. Insert leads through PCB
3. Solder all 3 pins
4. Mount heatsink with screw
5. Ensure good thermal contact

**Insulation:** Check if insulating washer needed (BTA16-600B has isolated tab, but verify).

#### Step 8: Install Terminals

**Screw terminal blocks:**
1. AC Input (L, N) - 2 position
2. Load Output - 2 position
3. ESP32 connections - 2 position (GPIO34, GPIO25)

**Wire gauge:** 18 AWG minimum for AC mains

#### Step 9: Install Fuse Holder and MOV

1. **Fuse holder:** Panel-mount or PCB-mount
2. **Insert fuse:** 5A fast-blow
3. **MOV:** Polarity doesn't matter, across L-N

#### Step 10: Final Solder Inspection

**Check every joint:**
- ✅ Shiny, concave meniscus
- ✅ Solder flows onto both pad and lead
- ✅ No cold solder (dull, grainy)
- ✅ No bridges between pads
- ✅ No flux residue (clean if needed)

#### Step 11: Install ESP32 Connections

**Option A: Screw terminals**
- More professional
- Easy to disconnect

**Option B: Pin headers**
- Direct ESP32 mounting
- More compact

**Connections:**
```
PCB Terminal → ESP32 Pin
ZC_OUT       → GPIO34
TRIAC_IN     → GPIO25
GND          → GND
(Optional: 3.3V or 5V power)
```

---

## Testing Procedures

### PRE-POWER TESTING (Power Must Be OFF)

#### Test 1: Visual Inspection

✅ All components installed
✅ Correct polarity on all polarized parts
✅ No solder bridges
✅ No missing components
✅ Clean solder joints
✅ Proper clearances maintained

#### Test 2: Continuity Tests

**Using multimeter in continuity mode:**

1. **AC input to load:** Should be open (infinite resistance)
   - If continuity present, TRIAC shorted (replace)

2. **Mains ground to ESP32 ground:** Should be open (>1MΩ)
   - If continuity, isolation compromised (DANGEROUS)

3. **Fuse continuity:** Should beep (low resistance)

4. **Bridge rectifier:**
   - AC to +DC: Diode drop one direction (~0.6V)
   - AC to -DC: Diode drop one direction (~0.6V)

#### Test 3: Resistance Checks

**Measure and verify:**

| Component | Expected Resistance |
|-----------|-------------------|
| R1 (330Ω, 2W) | 320-340Ω |
| R2 (10kΩ) | 9.5-10.5kΩ |
| R3 (330Ω) | 320-340Ω |
| R4 (180Ω) | 175-185Ω |
| R5 (1kΩ) | 950-1050Ω |
| R6 (47Ω, 3W) | 45-50Ω |

#### Test 4: Capacitance Checks

**Using capacitance meter:**

| Component | Expected Capacitance |
|-----------|---------------------|
| C1 (100nF) | 90-110nF |
| C2 (10nF) | 9-11nF |
| C3 (47nF) | 42-52nF |

#### Test 5: Isolation Test

**Using megger (500V DC):**

**Test points:**
1. Mains input to ESP32 ground: **>10MΩ required**
2. TRIAC MT2 to ESP32 ground: **>10MΩ required**
3. Optocoupler pin 1 to pin 6: **>10MΩ required**

**If any test <10MΩ:** DO NOT PROCEED. Find and fix isolation failure.

### POWER-ON TESTING (Use Isolation Transformer!)

#### Test 6: Low Voltage Test (Use Variac)

**Equipment needed:**
- Isolation transformer (1:1, 500VA)
- Variac (variable transformer)
- Multimeter
- Oscilloscope (highly recommended)

**Procedure:**

1. **Setup:**
   ```
   Wall AC → Isolation Transformer → Variac → Circuit
   ```

2. **ESP32 NOT CONNECTED yet** (test power circuit only)

3. **Set variac to 0V, power on**

4. **Slowly increase to 24V AC:**
   - Watch for smoke, sparks, unusual sounds
   - Feel components for excessive heat
   - STOP if anything abnormal

5. **Measure zero-cross output:**
   - Should see pulses at ZC_OUT terminal
   - Oscilloscope: 100Hz pulses (50Hz AC)
   - Voltage swings 0-3.3V (if ZC has pull-up to 3.3V)

6. **Increase to 48V AC:**
   - Repeat observations
   - Zero-cross pulses should continue

7. **Gradually increase to full voltage (120V or 240V):**
   - Monitor temperature of all components
   - TRIAC should remain cool (not triggered)
   - No excessive heating anywhere

8. **Power OFF, let cool**

#### Test 7: ESP32 Connection Test

1. **Power OFF**
2. **Connect ESP32:**
   - ZC_OUT → GPIO34
   - TRIAC_IN → GPIO25
   - GND → GND

3. **Flash firmware to ESP32** (via USB)

4. **Monitor serial output:**
   ```
   ESP32 AC Dimmer Fan Controller
   WARNING: HIGH VOLTAGE DEVICE
   GPIO initialized successfully
   Timer initialized successfully
   Fan OFF
   System ready
   ```

5. **Verify initialization** before applying AC power

#### Test 8: Lamp Load Test (MANDATORY)

**DO NOT test with fan first! Use lamp!**

**Why lamp:**
- Resistive load (forgiving)
- Visual feedback
- Safer for initial testing
- Easy to diagnose problems

**Equipment:**
- 40-60W incandescent lamp (NOT LED, NOT CFL)
- Isolation transformer
- Oscilloscope (recommended)

**Procedure:**

1. **Power OFF**
2. **Connect lamp to load output**
3. **Ensure ESP32 powered and firmware running**
4. **Apply AC power via isolation transformer**

5. **Observe serial monitor:**
   ```
   === Setting speed to 0% ===
   Fan OFF
   (Lamp should be OFF)
   ```

6. **Wait for automatic speed changes:**
   ```
   === Setting speed to 30% ===
   Fan speed set to 30% (delay: 7000 us)
   (Lamp should dim to ~30%)
   ```

7. **Verify all speed steps:**
   - 0% → Lamp OFF
   - 30% → Dim
   - 50% → Medium
   - 75% → Bright
   - 100% → Full brightness

8. **Check with oscilloscope:**
   - **CH1:** Zero-cross signal (GPIO34)
   - **CH2:** TRIAC trigger (GPIO25)
   - **Trigger:** CH1 falling edge

   **Expected:**
   - Trigger pulses occur at calculated delay after zero-cross
   - Pulse width: ~50µs
   - Higher speed → shorter delay

9. **Monitor TRIAC temperature:**
   - Should remain <60°C
   - If >80°C, check heatsink installation

**If lamp test successful → Proceed to fan test**
**If lamp test fails → See TROUBLESHOOTING.md**

#### Test 9: Fan Load Test

**Only proceed after successful lamp test**

1. **Power OFF**
2. **Disconnect lamp, connect fan**
3. **Use small fan first** (50-100W)
4. **Apply AC power**

5. **Observe operation:**
   - Fan should run at various speeds
   - Minimal humming acceptable
   - No excessive vibration
   - TRIAC temperature <80°C
   - Fan starts reliably at minimum speed (20%)

6. **Long-term test:**
   - Run for 30 minutes at various speeds
   - Monitor temperature continuously
   - Check for stability

7. **Emergency stop test:**
   - Unplug power immediately
   - Verify fan stops
   - No sparks or arcing

**Common issues:**
- **Fan hums:** Check snubber circuit (C3 + R6)
- **Won't start at low speed:** Increase MIN_SPEED_PERCENT
- **TRIAC overheats:** Check heatsink, verify load current

---

## Final Installation

### Enclosure Requirements

**MANDATORY: Circuit must be in proper enclosure**

**Specifications:**
- **Material:** ABS, polycarbonate, metal (grounded)
- **Rating:** IP20 minimum (IP54 for damp locations)
- **Flame rating:** V-0 or V-1
- **Ventilation:** Required for heat dissipation
- **Access:** Removable cover for service
- **Mounting:** Secure to wall or fixture

**Internal layout:**
```
+---------------------------+
|  [AC IN]  [FUSE]          |
|                           |
|  [TRIAC + Heatsink]       |
|                           |
|  [PCB with Components]    |
|                           |
|  [ESP32]                  |
|                           |
|  [AC OUT to Load]         |
+---------------------------+
```

### Wiring

**Wire specifications:**
- **Gauge:** 18 AWG minimum (14 AWG for >5A loads)
- **Insulation:** 600V rated, 90°C minimum
- **Type:** Stranded copper
- **Color coding:**
  - Hot (Line): Black or Red
  - Neutral: White
  - Ground: Green or bare

**Strain relief:**
- Cable glands or grommets at entry points
- Secure cables to prevent pulling on terminals
- Minimum bend radius: 5× wire diameter

### Grounding

**If metal enclosure:**
1. Connect earth ground to enclosure
2. Do NOT connect to ESP32 ground
3. Use ground lug with star washer
4. Paint removal at contact point

**Ground symbols:**
```
⏚  Earth ground (enclosure)
⏚  Chassis ground (enclosure)
⏚  Signal ground (ESP32) - ISOLATED
```

### Mounting

**Wall mounting:**
- Use appropriate anchors for wall type
- Support weight of enclosure + components
- Accessible for service
- Away from moisture sources

**DIN rail mounting:**
- Use DIN-rail compatible enclosure
- Secure latching

### Labeling

**Required labels:**
- ⚡ HIGH VOLTAGE - DO NOT OPEN
- Voltage rating (120V or 240V AC)
- Current/power rating
- Fuse rating
- Load type (FAN ONLY - NOT FOR USE WITH...)
- Contact information for service

### Testing After Installation

1. **Power OFF final test:**
   - Verify all connections tight
   - Check earth ground continuity
   - Isolation test (megger)

2. **Power ON final test:**
   - Use isolation transformer initially
   - Test all functions
   - Monitor temperature for 1 hour
   - Verify safety shutdown works

3. **Documentation:**
   - Photograph installation
   - Record serial number, date
   - Keep testing records
   - Provide user instructions

---

## Troubleshooting Flowchart

```
START
  |
  ├─ Power LED on? ──NO──> Check power supply
  |       |
  |      YES
  |       |
  ├─ Serial output? ──NO──> Check ESP32 USB connection
  |       |
  |      YES
  |       |
  ├─ "GPIO initialized"? ──NO──> Firmware upload failed
  |       |
  |      YES
  |       |
  ├─ Oscilloscope shows ZC pulses? ──NO──> Check zero-cross circuit
  |       |                                  - Bridge rectifier
  |      YES                                 - MOC3021
  |       |                                  - Resistors
  |       |
  ├─ Oscilloscope shows trigger pulses? ──NO──> Check trigger circuit
  |       |                                       - GPIO25 output
  |      YES                                      - MOC3021
  |       |                                       - Connections
  |       |
  ├─ Lamp dims? ──NO──> Check TRIAC circuit
  |       |              - Gate connections
  |      YES             - TRIAC itself
  |       |              - Load connections
  |       |
  ├─ Fan runs? ──NO──> Check:
  |       |            - Fan compatibility
  |      YES           - Snubber circuit
  |       |            - Minimum speed setting
  |       |
  ├─ Excessive humming? ──YES──> - Verify snubber
  |       |                       - Increase min speed
  |       NO                      - Try different fan
  |       |
  ├─ TRIAC overheats? ──YES──> - Check heatsink
  |       |                     - Verify thermal paste
  |       NO                    - Reduce load
  |       |
  ├─ Erratic behavior? ──YES──> - Check power supply
  |       |                      - Verify isolation
  |       NO                     - Add filtering
  |       |
SUCCESS!
```

---

## Safety Reminders

Before closing enclosure and deploying:

- [ ] All connections secure and insulated
- [ ] Isolation verified (>10MΩ)
- [ ] Fuse installed and correct rating
- [ ] Heatsink properly attached
- [ ] All components within temperature limits
- [ ] Earth ground connected (if metal enclosure)
- [ ] No exposed mains voltage
- [ ] Warning labels applied
- [ ] Testing complete and successful
- [ ] Documentation complete

**IF ANY ITEM UNCHECKED: DO NOT DEPLOY**

---

## User Instructions (Include with Device)

### Safe Operation

1. **Only use with compatible fans:**
   - Standard AC induction motor fans
   - NOT for electronic fans or VFDs

2. **Rated load:**
   - Maximum: 200W (inductive)
   - Do not exceed

3. **Installation:**
   - Must be installed by qualified electrician
   - Requires proper grounding
   - Adequate ventilation required

4. **Maintenance:**
   - Annual inspection
   - Check for loose connections
   - Verify proper operation
   - Replace fuse with same rating only

5. **Warning signs:**
   - Burning smell → DISCONNECT IMMEDIATELY
   - Excessive heat → DISCONTINUE USE
   - Erratic operation → SERVICE REQUIRED
   - Sparking or arcing → EMERGENCY SHUTDOWN

6. **Emergency:**
   - Know location of circuit breaker
   - Have fire extinguisher nearby (Class C)
   - Do not attempt repair unless qualified

---

**Congratulations on completing your build!**

**Remember: This is a learning project. Continuous monitoring and maintenance are essential for safe, reliable operation.**

---

*Build Guide Version 1.0 - 2025-01-11*
