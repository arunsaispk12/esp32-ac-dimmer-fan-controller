# Complete Circuit Connections

This document provides detailed point-to-point wiring connections for the ESP32 AC Dimmer Fan Controller.

⚡ **DANGER: HIGH VOLTAGE** - Only qualified personnel should build this circuit.

---

## Power Input and Protection

### AC Mains Input (J1)

**Terminal J1:**
- **Pin 1 (LINE)**: Brown/Black wire from AC plug
- **Pin 2 (NEUTRAL)**: Blue/White wire from AC plug
- **Pin 3 (EARTH)**: Green/Yellow wire to enclosure ground (NOT connected to ESP32)

### Fuse F1

- **Input**: J1 Pin 1 (LINE)
- **Output**: Node "AC_LINE_FUSED"

### MOV1 (Surge Protection)

- **Pin 1**: AC_LINE_FUSED
- **Pin 2**: J1 Pin 2 (NEUTRAL)

---

## Zero-Cross Detection Circuit

### Bridge Rectifier (D1-D4)

**Configuration**: Full-wave bridge

```
D1: AC1 to +DC
D2: AC2 to +DC
D3: AC1 to -DC (GND)
D4: AC2 to -DC (GND)
```

**Connections:**

| Component | Pin | Connects To |
|-----------|-----|-------------|
| D1 | Anode (A) | AC_LINE_FUSED |
| D1 | Cathode (K) | Bridge +DC |
| D2 | Anode (A) | J1 Pin 2 (NEUTRAL) |
| D2 | Cathode (K) | Bridge +DC |
| D3 | Anode (A) | Bridge -DC (GND_MAINS) |
| D3 | Cathode (K) | AC_LINE_FUSED |
| D4 | Anode (A) | GND_MAINS |
| D4 | Cathode (K) | J1 Pin 2 (NEUTRAL) |

**Notes:**
- Bridge +DC goes to R1
- Bridge -DC = GND_MAINS (mains ground reference)

### Resistor R1 (330Ω, 2W)

- **Pin 1**: Bridge +DC
- **Pin 2**: U1 Pin 1 (MOC3021 anode)

### Optocoupler U1 (MOC3021) - Zero-Cross Detector

**Pinout (viewed from top, notch left):**

```
  1 ┌───U───┐ 6
  2 │       │ 5
  3 │MOC3021│ 4
    └───────┘
```

**Pin Connections:**

| Pin | Function | Connects To |
|-----|----------|-------------|
| 1 | Anode | R1 Pin 2 |
| 2 | Cathode | GND_MAINS |
| 3 | NC | Not connected |
| 4 | Emitter | GND_ESP32 |
| 5 | Collector | R2 Pin 1 and C1 Pin 1 |
| 6 | NC | Not connected |

### Resistor R2 (10kΩ, 1/4W) - Pull-up

- **Pin 1**: U1 Pin 5 (Collector)
- **Pin 2**: +3.3V (from ESP32)

### Capacitor C1 (100nF, 50V) - Noise Filter

- **Pin 1**: U1 Pin 5 (Collector)
- **Pin 2**: GND_ESP32

### Zero-Cross Output to ESP32

- **Signal**: U1 Pin 5 (Collector)
- **Destination**: ESP32 GPIO34
- **Logic**: HIGH when near zero-cross, LOW otherwise

---

## TRIAC Trigger Circuit

### ESP32 Trigger Output

- **Signal**: ESP32 GPIO25
- **Goes to**: R3 Pin 1

### Resistor R3 (330Ω, 1/4W) - LED Current Limit

- **Pin 1**: ESP32 GPIO25
- **Pin 2**: U2 Pin 1 (MOC3021 anode)

### Optocoupler U2 (MOC3021) - TRIAC Driver

**Pinout (viewed from top, notch left):**

```
  1 ┌───U───┐ 6
  2 │       │ 5
  3 │MOC3021│ 4
    └───────┘
```

**Pin Connections:**

| Pin | Function | Connects To |
|-----|----------|-------------|
| 1 | Anode | R3 Pin 2 |
| 2 | Cathode | GND_ESP32 |
| 3 | NC | Not connected |
| 4 | Emitter | Q1 MT1 (TRIAC) |
| 5 | NC | Not connected |
| 6 | Collector | R4 Pin 1 |

### Resistor R4 (180Ω, 1/2W) - Gate Current Limit

- **Pin 1**: U2 Pin 6 (Collector)
- **Pin 2**: Q1 Gate

### Resistor R5 (1kΩ, 1/4W) - Gate Pull-down

- **Pin 1**: Q1 Gate
- **Pin 2**: Q1 MT1

### Capacitor C2 (10nF, 630V) - dV/dt Protection

- **Pin 1**: Q1 Gate
- **Pin 2**: Q1 MT1

**Purpose**: Prevents false TRIAC triggering from fast voltage transients

---

## TRIAC Power Switching

### TRIAC Q1 (BTA16-600B)

**Pinout (viewed from front, tab up):**

```
   TAB (MT1)
   ┌───┐
   │   │
   └───┘
   │ │ │
  MT2 G MT1
```

**Pin Connections:**

| Pin | Function | Connects To |
|-----|----------|-------------|
| MT2 | Main Terminal 2 | AC_LINE_FUSED |
| G | Gate | R4 Pin 2, R5 Pin 1, C2 Pin 1 |
| MT1 | Main Terminal 1 | J2 Pin 1 (Load output) |
| TAB | Connected to MT1 | Heatsink (isolated) |

**Heatsink:**
- Thermal paste between TRIAC tab and heatsink
- Heatsink rating: <7°C/W
- Mounting screw through tab hole

---

## Snubber Network (Inductive Load Protection)

### Capacitor C3 (47nF, X2 Rated, 630V)

- **Pin 1**: Q1 MT2
- **Pin 2**: R6 Pin 1

### Resistor R6 (47Ω, 3W)

- **Pin 1**: C3 Pin 2
- **Pin 2**: Q1 MT1

**Complete Snubber Path:** MT2 → C3 → R6 → MT1

**Purpose**:
- Limits dV/dt across TRIAC
- Absorbs inductive voltage spikes
- Prevents false triggering
- Reduces EMI

---

## Load Output

### Terminal J2 (Load Connection)

**Terminal J2:**
- **Pin 1**: Q1 MT1
- **Pin 2**: J1 Pin 2 (NEUTRAL)

**Load (Fan) Connection:**
- Connect fan between J2 Pin 1 and J2 Pin 2
- Maximum load: 200W inductive, 400W resistive
- Do NOT exceed TRIAC rating

---

## ESP32 Interface Connections

### Terminal J3 (ESP32 Connection)

**Terminal J3:**
- **Pin 1 (GND)**: GND_ESP32 (U1 Pin 4, U2 Pin 2, C1 Pin 2)
- **Pin 2 (ZC)**: Zero-cross signal from U1 Pin 5
- **Pin 3 (TRIG)**: Trigger signal to R3 Pin 1

### ESP32 GPIO Connections

| ESP32 Pin | Function | Connects To | Direction |
|-----------|----------|-------------|-----------|
| GPIO34 | Zero-cross input | J3 Pin 2 | Input |
| GPIO25 | TRIAC trigger | J3 Pin 3 | Output |
| GND | Ground (isolated) | J3 Pin 1 | - |
| 3.3V | Power | R2 Pin 2 | - |

**CRITICAL**: ESP32 GND must be isolated from mains ground

---

## Ground Connections

### Two Separate Grounds (NOT Connected)

#### Mains Ground (GND_MAINS)

**Connected to:**
- Bridge rectifier -DC (D3 anode, D4 anode)
- U1 Pin 2 (MOC3021 cathode)
- AC neutral reference

**NOT connected to:**
- ESP32 ground
- Enclosure ground
- Earth ground

#### ESP32 Ground (GND_ESP32)

**Connected to:**
- U1 Pin 4 (MOC3021 emitter)
- U2 Pin 2 (MOC3021 cathode)
- C1 Pin 2
- ESP32 GND pin
- J3 Pin 1

**NOT connected to:**
- Mains ground
- Enclosure ground
- Earth ground

**Isolation Test:** Measure resistance between GND_MAINS and GND_ESP32. Should be >10MΩ.

---

## Power Supply Connections

### ESP32 Power

**Option 1: Separate USB Power (Recommended for testing)**
- Connect ESP32 USB port to computer or USB charger
- Provides isolated power and programming

**Option 2: Integrated Power Supply (For deployment)**
- Add isolated AC-DC converter (e.g., HLK-PM01, 5V output)
- Input: AC_LINE_FUSED and NEUTRAL
- Output: +5V and GND_ESP32
- **MUST** be isolated converter (not just rectifier)

---

## Complete Wire List

### High Voltage Wiring (18 AWG minimum)

| From | To | Wire Color | Notes |
|------|----|-----------| ------|
| J1 Pin 1 (LINE) | F1 Input | Brown/Black | Hot wire |
| F1 Output | MOV1 Pin 1 | Brown/Black | Fused hot |
| F1 Output | D1 Anode | Brown/Black | To bridge |
| F1 Output | D3 Cathode | Brown/Black | To bridge |
| F1 Output | Q1 MT2 | Brown/Black | To TRIAC |
| J1 Pin 2 (NEUTRAL) | MOV1 Pin 2 | Blue/White | Neutral |
| J1 Pin 2 (NEUTRAL) | D2 Anode | Blue/White | To bridge |
| J1 Pin 2 (NEUTRAL) | D4 Cathode | Blue/White | To bridge |
| J1 Pin 2 (NEUTRAL) | J2 Pin 2 | Blue/White | To load |
| Q1 MT1 | J2 Pin 1 | Brown/Red | Switched hot |

### Low Voltage Wiring (22-26 AWG)

| From | To | Wire Color | Notes |
|------|----|-----------|-------|
| ESP32 GPIO34 | J3 Pin 2 | Yellow | Zero-cross |
| ESP32 GPIO25 | J3 Pin 3 | Green | Trigger |
| ESP32 GND | J3 Pin 1 | Black | Ground |
| ESP32 3.3V | R2 Pin 2 | Red | Pull-up |

---

## Assembly Sequence

**For correct assembly order, see [BUILD_GUIDE.md](../../docs/guides/BUILD_GUIDE.md)**

### Quick Reference

1. ✓ Install bridge rectifier diodes (D1-D4) - CHECK POLARITY
2. ✓ Install resistors (R1-R6)
3. ✓ Install capacitors (C1, C2, C3) - C3 must be X2 rated
4. ✓ Install optocouplers (U1, U2) - CHECK PIN 1 ORIENTATION
5. ✓ Install TRIAC (Q1) with heatsink and thermal paste
6. ✓ Install fuse holder and MOV
7. ✓ Install terminal blocks (J1, J2, J3)
8. ✓ Wire all connections per wire list above
9. ✓ Visual inspection - no shorts, correct polarity
10. ✓ Continuity test - verify connections
11. ✓ Isolation test - verify >10MΩ between grounds

---

## Testing Points

### Voltage Measurement Points (POWER OFF)

| Test Point | Other Point | Expected Resistance |
|------------|-------------|---------------------|
| J1 Pin 1 | J1 Pin 2 | >1MΩ (open circuit) |
| Q1 MT2 | Q1 MT1 | >100kΩ (TRIAC off) |
| GND_MAINS | GND_ESP32 | >10MΩ (isolated) |

### Voltage Measurement Points (POWER ON - USE ISOLATION TRANSFORMER)

| Test Point | Other Point | Expected Voltage | Notes |
|------------|-------------|------------------|-------|
| J1 Pin 1 | J1 Pin 2 | 120V or 240V AC | Mains voltage |
| Bridge +DC | GND_MAINS | ~170V or ~340V DC | Pulsating DC |
| U1 Pin 5 | GND_ESP32 | 0-3.3V | Pulses at 100Hz |
| R2 Pin 2 | GND_ESP32 | 3.3V | ESP32 power |

**WARNING**: Do not measure high voltage points unless qualified and using proper equipment.

---

## Common Wiring Mistakes

### ❌ DANGEROUS Errors

1. **Connecting ESP32 GND to mains ground** → Electric shock hazard
2. **Wrong diode polarity in bridge** → Circuit won't work, diodes may fail
3. **Swapping U1/U2 pin 1 and 2** → Reverse polarity, damaged optocoupler
4. **Using non-X2 capacitor for C3** → Fire hazard
5. **Missing fuse** → No overcurrent protection
6. **No heatsink on TRIAC** → Thermal failure

### ⚠️ Common Errors

1. **Swapping GPIO34 and GPIO25** → No operation
2. **Wrong resistor values** → Incorrect operation
3. **Missing C2 (10nF)** → False TRIAC triggering
4. **Missing snubber (C3, R6)** → Unreliable with fans
5. **Insufficient wire gauge** → Overheating

---

## Troubleshooting Connection Issues

### No Zero-Cross Signal

**Check:**
- D1-D4 polarity correct
- U1 Pin 1 is anode (marked dot or notch)
- R1 connected to U1 Pin 1
- R2 connected to 3.3V
- C1 connected to GND_ESP32

**Test:**
- Measure voltage at U1 Pin 1: should see pulsating ~1.2V
- Measure U1 Pin 5: should toggle 0-3.3V

### TRIAC Won't Trigger

**Check:**
- R3 connected to GPIO25
- U2 Pin 1 is anode
- R4 connected to TRIAC gate
- R5 gate pull-down present
- C2 gate-MT1 capacitor present

**Test:**
- Measure GPIO25 output: should pulse to 3.3V
- Measure TRIAC gate voltage when triggered: should see ~1-2V

---

## Verification Checklist

Before applying power:

- [ ] All component values verified against BOM
- [ ] All polarities correct (diodes, optocouplers)
- [ ] All connections match wire list
- [ ] No shorts between mains and low-voltage
- [ ] Fuse installed, correct rating
- [ ] Heatsink on TRIAC with thermal paste
- [ ] Wire gauge adequate (18 AWG for mains)
- [ ] Strain relief on all cables
- [ ] Enclosure properly closed
- [ ] Isolation tested (>10MΩ)

**Only proceed to power-on testing after ALL items checked**

---

## CAD Symbol Reference

For creating schematics in other CAD tools:

### Generic Symbols Needed

- Resistor (×6)
- Capacitor unpolarized (×3)
- Diode (×4)
- DIP-6 IC (×2) for optocouplers
- TO-220 transistor for TRIAC
- Terminal blocks (×3)
- Fuse
- Varistor/MOV

### Custom Symbols

Create or download:
- MOC3021 (DIP-6 optocoupler)
- BTA16-600B (TRIAC, TO-220)
- Generic bridge rectifier

---

**This completes the circuit connection documentation. Always prioritize safety and double-check all connections before applying power.**

⚡ **HIGH VOLTAGE - QUALIFIED PERSONNEL ONLY** ⚡

*Circuit Connections Document Version 1.0 - 2025-01-11*
