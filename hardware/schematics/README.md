# Circuit Schematics

This directory contains circuit schematics for the ESP32 AC Dimmer Fan Controller.

⚡ **HIGH VOLTAGE WARNING:** This circuit handles mains AC voltage. See [SAFETY.md](../../docs/SAFETY.md).

---

## Files

### KiCad Project Files

| File | Description |
|------|-------------|
| `ac_dimmer.kicad_pro` | KiCad 7.0+ project file |
| `ac_dimmer.kicad_sch` | Main schematic file |
| `ac_dimmer_schematic.pdf` | PDF export for easy viewing |

### Opening in KiCad

1. **Install KiCad 7.0 or later**: https://www.kicad.org/download/
2. **Open project**: File → Open Project → Select `ac_dimmer.kicad_pro`
3. **View schematic**: Double-click schematic in project manager

### KiCad Version

- **Created with**: KiCad 7.0
- **Compatibility**: KiCad 7.0+
- **Libraries**: Uses standard KiCad symbol libraries

---

## Circuit Overview

The schematic is organized into the following sections:

### 1. AC Input and Protection
- **J1**: AC mains input terminal (Line, Neutral)
- **F1**: 5A fast-blow fuse
- **MOV1**: 275V metal oxide varistor (surge protection)

### 2. Zero-Cross Detection Circuit
- **D1-D4**: Bridge rectifier (4× 1N4007)
- **R1**: 330Ω, 2W current limiting resistor
- **U1**: MOC3021 optocoupler (isolation)
- **R2**: 10kΩ pull-up resistor
- **C1**: 100nF noise filter capacitor

**Output**: GPIO34 (ESP32) receives zero-crossing pulses

### 3. TRIAC Trigger Circuit
- **U2**: MOC3021 optocoupler (isolation)
- **R3**: 330Ω current limiting resistor
- **R4**: 180Ω gate resistor
- **R5**: 1kΩ gate pull-down
- **C2**: 10nF dV/dt protection capacitor

**Input**: GPIO25 (ESP32) provides trigger signal

### 4. TRIAC Power Switching
- **Q1**: BTA16-600B TRIAC (16A, 600V)
- **Heatsink**: TO-220 heatsink (<7°C/W)

### 5. Snubber Network (Critical for Inductive Loads)
- **C3**: 47nF X2-rated capacitor (630V)
- **R6**: 47Ω, 3W resistor

**Purpose**: Protects TRIAC from inductive voltage spikes

### 6. Load Output
- **J2**: Output terminal to fan load

### 7. ESP32 Connections
- **J3**: ESP32 interface connector
  - Pin 1: GND (isolated from mains ground)
  - Pin 2: GPIO34 (zero-cross input)
  - Pin 3: GPIO25 (TRIAC trigger output)

---

## Circuit Diagram (ASCII)

```
AC LINE ──┬── [FUSE 5A] ──┬── [MOV] ── AC NEUTRAL
          │               │
          │           [BRIDGE]
          │          RECTIFIER
          │               │
          │            [330Ω]
          │              2W
          │               │
          │           ┌───┴───┐
          │           │ MOC   │  Zero-Cross
          │           │ 3021  │  Detection
          │           │  U1   │
          │           └───┬───┘
          │               │
          │            [10kΩ] ── [100nF] ── GND_ESP32
          │               │
          │          GPIO34 ← (ESP32)
          │
          ├── [Q1] ── FAN LOAD ── AC NEUTRAL
          │   BTA16
          │   TRIAC
          │     │
          │  ┌──┴──┐
          │  │ 47nF│ Snubber
          │  │  X2 │ Circuit
          │  └──┬──┘
          │     │
          │   [47Ω]
          │    3W
          │     │
          └─────┴─────────────────────────┘

TRIAC Trigger Circuit:

GPIO25 (ESP32) ──[330Ω]── MOC3021 ──[180Ω]── TRIAC Gate
                           U2          │
                                    [1kΩ]── TRIAC MT1
                                       │
                                    [10nF]
                                       │
                                   TRIAC MT1
```

---

## Component Reference Designators

| Ref | Component | Value | Package |
|-----|-----------|-------|---------|
| **Power Input** |
| J1 | Terminal Block | 2-pos | 5.08mm pitch |
| F1 | Fuse | 5A Fast | 5×20mm |
| MOV1 | Varistor | 275V AC | 14mm disc |
| **Zero-Cross Detection** |
| D1-D4 | Diode | 1N4007 | DO-41 |
| U1 | Optocoupler | MOC3021 | DIP-6 |
| R1 | Resistor | 330Ω 2W | Axial |
| R2 | Resistor | 10kΩ 1/4W | Axial |
| C1 | Capacitor | 100nF 50V | Radial |
| **TRIAC Trigger** |
| U2 | Optocoupler | MOC3021 | DIP-6 |
| R3 | Resistor | 330Ω 1/4W | Axial |
| R4 | Resistor | 180Ω 1/2W | Axial |
| R5 | Resistor | 1kΩ 1/4W | Axial |
| C2 | Capacitor | 10nF 630V | Radial |
| **Power Switching** |
| Q1 | TRIAC | BTA16-600B | TO-220AB |
| C3 | Capacitor | 47nF X2 630V | Radial |
| R6 | Resistor | 47Ω 3W | Axial |
| **Output** |
| J2 | Terminal Block | 2-pos | 5.08mm pitch |
| **ESP32 Interface** |
| J3 | Terminal Block | 3-pos | 2.54mm pitch |

---

## Voltage and Current Ratings

### High Voltage Section (Mains)

| Net | Voltage | Current | Notes |
|-----|---------|---------|-------|
| AC_LINE | 110-240V AC | 5A max | Fused |
| AC_NEUTRAL | 110-240V AC | 5A max | Return path |
| TRIAC_MT2 | 0-240V AC | 16A max | Chopped waveform |

### Low Voltage Section (Isolated)

| Net | Voltage | Current | Notes |
|-----|---------|---------|-------|
| GPIO34 | 0-3.3V DC | <1mA | Zero-cross input |
| GPIO25 | 0-3.3V DC | ~10mA | Trigger output |
| GND_ESP32 | 0V | - | Isolated from mains |

**CRITICAL**: Minimum 8mm creepage between high and low voltage sections on PCB.

---

## Safety Features in Schematic

### 1. Isolation

- **Dual optocouplers**: 5000V isolation rating each
- **Separate grounds**: Mains ground ≠ ESP32 ground
- **No galvanic connection**: Between high and low voltage

### 2. Protection

- **Fuse**: Overcurrent protection (5A fast-blow)
- **MOV**: Surge protection (275V for 240V mains)
- **Snubber**: dV/dt protection for TRIAC
- **Gate capacitor**: False trigger prevention

### 3. Safety Margins

- **TRIAC rating**: 600V (2.5× safety margin for 240V AC)
- **Diode rating**: 1000V (4× safety margin)
- **Capacitor voltage**: 630V (2.6× safety margin)

---

## Design Rules for PCB Layout

When creating PCB from this schematic:

### Clearance Requirements

| Voltage Difference | Minimum Clearance | Minimum Creepage |
|-------------------|-------------------|------------------|
| Mains to Mains | 3mm | 5mm |
| Mains to Low Voltage | 4mm | 8mm |
| Low Voltage to Low Voltage | 0.2mm | 0.3mm |

### Trace Width

| Net | Current | Minimum Width (1oz Cu) |
|-----|---------|------------------------|
| AC_LINE, AC_NEUTRAL | 5A | 2.0mm |
| TRIAC connections | 5A | 2.0mm |
| GPIO signals | <100mA | 0.4mm |

### Component Placement

- **High voltage components**: One side of PCB
- **Low voltage components**: Opposite side
- **Optocouplers**: Straddle isolation barrier
- **TRIAC**: Edge of board for heatsink clearance

---

## Schematic Annotations

### Warning Labels on Schematic

The following warnings are printed on the schematic:

1. **"⚡ HIGH VOLTAGE SECTION ⚡"** - Marks mains voltage area
2. **"DANGER: 110-240V AC MAINS"** - Critical warning
3. **"Maintain 8mm clearance"** - PCB design rule
4. **"ISOLATION BARRIER"** - Marks boundary between domains
5. **"X2 RATED REQUIRED"** - Critical component specification

### Net Classes

| Net Class | Purpose | Color on Schematic |
|-----------|---------|-------------------|
| HighVoltage | Mains AC nets | Red |
| LowVoltage | ESP32 nets | Blue |
| Default | Other nets | Black |

---

## Modifications and Variants

### For 60Hz AC Operation

No schematic changes required. Update firmware only:
```c
#define AC_FREQUENCY 60
#define AC_HALF_PERIOD 8333
```

### For Higher Power (>200W)

**Changes needed:**
- **Q1**: Replace BTA16-600B with BTA24-600B or BTA41-600B
- **F1**: Increase to 10A fuse
- **All AC traces**: Increase to 3-4mm width
- **Heatsink**: Increase size (<5°C/W thermal resistance)

### For Dual Channel

**Duplicate these sections:**
- TRIAC trigger circuit (U2, R3, R4, R5, C2)
- TRIAC switching (Q1, C3, R6)
- Load output (J2)

**Share:**
- AC input and protection (J1, F1, MOV1)
- Zero-cross detection (U1 and related components)

**Add:**
- Second ESP32 GPIO for trigger (e.g., GPIO26)

---

## Exporting from KiCad

### Generate PDF

1. Open schematic in KiCad
2. File → Plot
3. Select output directory
4. Check "PDF" format
5. Click "Plot All Pages"

### Generate Netlist

1. Tools → Generate Netlist File
2. Select format (KiCad, Spice, etc.)
3. Click "Generate Netlist"

### Generate BOM

1. Tools → Generate Bill of Materials
2. Select BOM generator script
3. Configure output format (CSV recommended)
4. Click "Generate"

---

## Simulation

### LTSpice Simulation

The circuit can be simulated in LTSpice:

**Simulation setup:**
1. AC voltage source: 240V RMS, 50Hz
2. Load: 200W resistive (for initial testing)
3. Trigger pulse: Variable delay 0-10ms

**What to observe:**
- Zero-cross timing
- TRIAC triggering behavior
- Load voltage waveform
- Power delivery vs. trigger delay

**Caution:** Simulation doesn't capture all real-world effects:
- Optocoupler switching speed
- TRIAC turn-on jitter
- Inductive load reactance
- EMI and noise

---

## Schematic Review Checklist

Before building hardware:

- [ ] All components have correct values
- [ ] Polarity of D1-D4 correct (bridge rectifier)
- [ ] MOC3021 pin 1 marked (anode indicator)
- [ ] BTA16-600B has 'B' suffix (isolated tab)
- [ ] C3 is X2 rated (not regular capacitor)
- [ ] MOV voltage matches mains (275V for 240V AC)
- [ ] Fuse is fast-blow type
- [ ] All resistor power ratings correct
- [ ] Heatsink specified for TRIAC
- [ ] ESP32 ground isolated from mains ground

---

## Questions and Support

**For schematic-related questions:**
1. Review [BUILD_GUIDE.md](../../docs/guides/BUILD_GUIDE.md)
2. Check component datasheets
3. Open an issue on GitHub

**For KiCad help:**
- KiCad Documentation: https://docs.kicad.org/
- KiCad Forums: https://forum.kicad.info/
- KiCad Discord: https://discord.gg/kicad

---

## License

Schematic files are licensed under MIT License (see [LICENSE](../../LICENSE)).

**Disclaimer**: Building this circuit is at your own risk. The schematic is provided for educational purposes. No warranty is provided regarding safety or fitness for any purpose.

---

**⚡ HIGH VOLTAGE - QUALIFIED PERSONNEL ONLY ⚡**

*Schematic Documentation Version 1.0 - 2025-01-11*
