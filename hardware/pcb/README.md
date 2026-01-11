# PCB Layout Files

This directory contains PCB layout files for the ESP32 AC Dimmer Fan Controller.

⚡ **HIGH VOLTAGE WARNING:** This PCB handles mains AC voltage. Only qualified personnel should fabricate and assemble. See [SAFETY.md](../../docs/SAFETY.md).

---

## Files

| File | Description |
|------|-------------|
| `ac_dimmer.kicad_pcb` | KiCad 7.0+ PCB layout file |
| `README.md` | This file - PCB documentation |
| `FABRICATION_NOTES.md` | Manufacturing specifications |
| `gerbers/` | Gerber files for manufacturing (generated) |

---

## PCB Specifications

### Board Dimensions

| Parameter | Value |
|-----------|-------|
| **Length** | 150mm |
| **Width** | 100mm |
| **Thickness** | 1.6mm (standard FR-4) |
| **Layers** | 2 (Top and Bottom) |
| **Copper Weight** | 1oz (35µm) standard, 2oz recommended for high current |

### Material Requirements

| Parameter | Specification |
|-----------|--------------|
| **Base Material** | FR-4, Tg 130°C minimum (Tg 150°C recommended) |
| **Surface Finish** | HASL lead-free, ENIG, or OSP |
| **Soldermask** | Both sides, green/black/blue |
| **Silkscreen** | Both sides, white color |
| **Min Hole Size** | 0.3mm |
| **Min Via Diameter** | 0.6mm (0.3mm drill) |

### Electrical Specifications

| Parameter | Value | Notes |
|-----------|-------|-------|
| **Maximum Voltage** | 250V AC | Between mains traces |
| **Maximum Current** | 10A continuous | On main power traces |
| **Isolation Voltage** | >1000V | Between mains and low voltage |
| **Dielectric Strength** | >500V DC | Through PCB thickness |

---

## Design Rules

### Trace Width Requirements

| Net/Current | Minimum Width | Recommended Width |
|-------------|---------------|-------------------|
| **AC Mains (5A)** | 1.5mm | 2.0mm |
| **TRIAC Power (10A)** | 2.0mm | 3.0mm |
| **Low Voltage Signals** | 0.3mm | 0.4mm |
| **Power Signals (3.3V)** | 0.5mm | 0.8mm |

### Clearance Requirements (Critical Safety)

| Between | Minimum Clearance | Minimum Creepage |
|---------|------------------|------------------|
| **Mains to Mains** | 3.0mm | 5.0mm |
| **Mains to Low Voltage** | 4.0mm | 8.0mm |
| **Low Voltage to Low Voltage** | 0.2mm | 0.3mm |
| **Pad to Pad** | 0.2mm | 0.3mm |

**CRITICAL:** The 8mm creepage distance between mains and low voltage sections is a safety requirement and must not be violated.

### Via Specifications

| Parameter | Minimum | Recommended |
|-----------|---------|-------------|
| **Via Diameter** | 0.6mm | 0.8mm |
| **Via Drill** | 0.3mm | 0.4mm |
| **Via Plating** | Tented | Tented or plugged |
| **Thermal Vias** | - | Under TRIAC for heat dissipation |

---

## Layer Stack-Up

### 2-Layer Board (Standard)

```
┌─────────────────────────────┐
│  Top Silkscreen (White)     │
├─────────────────────────────┤
│  Top Soldermask (Green)     │
├─────────────────────────────┤
│  F.Cu (Copper - 1oz)        │  ← High voltage traces
│                             │  ← Component placement
├─────────────────────────────┤
│  Core (FR-4, 1.6mm)         │
├─────────────────────────────┤
│  B.Cu (Copper - 1oz)        │  ← Ground planes
│                             │  ← Return paths
├─────────────────────────────┤
│  Bottom Soldermask (Green)  │
├─────────────────────────────┤
│  Bottom Silkscreen (White)  │
└─────────────────────────────┘
```

---

## PCB Layout Overview

### Board Sections (Left to Right)

```
┌────────────────────────────────────────┐
│                                        │
│  HIGH VOLTAGE SECTION  │  LOW VOLTAGE │
│  (Left side)           │  (Right)     │
│                        │              │
│  ┌─────────────────┐  │  ┌──────────┐│
│  │ AC Input        │  │  │ ESP32    ││
│  │ Fuse, MOV       │  │  │ GPIO     ││
│  │                 │  │  │ Connector││
│  │ Bridge Rect.    │  │  └──────────┘│
│  │ Zero-Cross      │  │              │
│  │                 │  │  ┌──────────┐│
│  │ TRIAC           │ I│  │ Pull-up  ││
│  │ (on edge)       │ S│  │ Filter   ││
│  │                 │ O│  │          ││
│  │ Snubber         │ L│  └──────────┘│
│  │                 │ A│              │
│  │ Load Output     │ T│              │
│  └─────────────────┘ I│              │
│                      O│              │
│                      N│              │
│                       │              │
└────────────────────────────────────────┘
     145mm mark (isolation barrier)
```

### Critical Layout Features

1. **Isolation Barrier** at 145mm
   - Dashed line on Dwgs.User layer
   - 8mm minimum clear zone
   - No traces cross this boundary except through optocouplers

2. **TRIAC Placement**
   - Located at PCB edge for heatsink clearance
   - Tab extends beyond board edge
   - Large copper pour for heat dissipation

3. **Ground Planes**
   - **Left side**: Mains ground reference (GND_MAINS)
   - **Right side**: ESP32 ground (GND_ESP32)
   - **NOT connected** - isolated by isolation barrier

4. **Component Placement**
   - High voltage components: Top side, left
   - Low voltage components: Top side, right
   - Optocouplers: Straddle isolation boundary

---

## Component Placement Guidelines

### High Voltage Section (Left Side)

**Components:**
- J1 (AC input terminal)
- F1 (Fuse holder)
- MOV1 (Varistor)
- D1-D4 (Bridge rectifier)
- U1 (Zero-cross optocoupler)
- Q1 (TRIAC with heatsink)
- C3, R6 (Snubber network)
- J2 (Load output terminal)

**Layout considerations:**
- Terminal blocks near board edges
- TRIAC at edge for heatsink mounting
- Snubber components close to TRIAC
- Bridge rectifier in compact square layout
- Fuse accessible for replacement

### Low Voltage Section (Right Side)

**Components:**
- J3 (ESP32 connector)
- U2 (Trigger optocoupler)
- R2 (Pull-up resistor)
- R3, R4, R5 (Trigger circuit)
- C1, C2 (Filter and protection caps)

**Layout considerations:**
- J3 connector near board edge
- Optocouplers straddle isolation boundary
- Short traces to minimize noise
- Ground plane for noise immunity

---

## Copper Pours and Zones

### Top Layer (F.Cu)

**High Voltage Zone:**
- Net: AC_LINE
- Area: Left section (55mm to 140mm, 35mm to 65mm)
- Purpose: Power distribution, heat dissipation
- Clearance: 0.5mm from other nets

**TRIAC Heat Zone:**
- Net: AC_LINE_FUSED or TRIAC_MT1
- Area: Around Q1
- Purpose: Heat spreading
- Thermal vias to bottom layer

### Bottom Layer (B.Cu)

**ESP32 Ground Plane:**
- Net: GND_ESP32
- Area: Right section (155mm to 195mm, full height)
- Purpose: Clean ground reference, noise reduction
- Clearance: 0.25mm from other nets

**Mains Ground Reference:**
- Net: GND_MAINS
- Area: Left section
- Purpose: Return path for mains circuitry
- **NOT connected to GND_ESP32**

---

## Silkscreen Markings

### Safety Warnings (F.SilkS)

**Required text:**
- "⚡ HIGH VOLTAGE ⚡" (large, bold, at top)
- "DANGER: 110-240V AC" (near mains section)
- "ESP32 AC DIMMER v1.0" (board identification)
- "ISOLATION BARRIER - 8mm MINIMUM" (at boundary)

### Component References

**All components labeled:**
- Reference designators (R1, C1, U1, etc.)
- Polarity indicators (diodes, optocouplers)
- Pin 1 markers (dots or squares)
- Component values (optional)

### Terminal Block Labels

**J1 (AC Input):**
- "L" (Line/Hot)
- "N" (Neutral)
- "⚡" warning symbol

**J2 (Load Output):**
- "LOAD +"
- "LOAD -"

**J3 (ESP32 Interface):**
- "GND"
- "ZC" (GPIO34)
- "TRIG" (GPIO25)

---

## Thermal Management

### TRIAC Heatsink Design

**PCB Features:**
- Large copper pour under TRIAC (F.Cu)
- Thermal vias array (5×5 grid, 1mm spacing)
- Bottom copper pour (B.Cu) for heat spreading
- Clearance around TRIAC for airflow

**Heatsink Mounting:**
- TRIAC tab extends beyond board edge
- M3 mounting hole in tab
- Heatsink bolted to tab with thermal paste
- Heatsink oriented vertically for natural convection

### Heat Dissipation Calculations

**Worst-case scenario:**
- Load current: 8A
- TRIAC voltage drop: ~1.5V
- Power dissipation: 12W

**Thermal path:**
1. TRIAC junction to case: 1.5°C/W
2. Case to heatsink: 0.5°C/W (with paste)
3. Heatsink to ambient: 7°C/W (required)
4. PCB contribution: ~2°C/W reduction

**Total:** ~9°C/W → 108°C rise → Junction temp: 148°C
**Safe:** Tj(max) = 125°C, so need better heatsink (<5°C/W for margin)

---

## Mounting Holes

### PCB Mounting

| Parameter | Specification |
|-----------|--------------|
| **Hole Diameter** | 3.2mm (for M3 screw) |
| **Hole Quantity** | 4 (one in each corner) |
| **Distance from Edge** | 5mm |
| **Clearance Zone** | 6mm diameter (no copper/traces) |

**Locations:**
- Top-left: (55mm, 35mm)
- Top-right: (195mm, 35mm)
- Bottom-left: (55mm, 125mm)
- Bottom-right: (195mm, 125mm)

**Standoff height:** 10mm recommended (for airflow under board)

---

## Design Rule Check (DRC) Settings

### KiCad DRC Configuration

```
Clearance Rules:
- Default net clearance: 0.2mm
- High voltage net clearance: 4.0mm
- Mains to low voltage: 8.0mm (creepage)

Track Width Rules:
- Minimum track width: 0.2mm
- Default track width: 0.4mm
- Power track width: 2.0mm (mains)

Via Rules:
- Minimum via diameter: 0.6mm
- Minimum via drill: 0.3mm
- Via to via clearance: 0.4mm

Copper to Edge:
- Minimum clearance: 0.5mm

Silkscreen:
- Minimum line width: 0.15mm
- Minimum text size: 0.8mm height
```

---

## Manufacturing Files Generation

### Gerber Files Required

**Plot layers:**
1. F.Cu (Top copper)
2. B.Cu (Bottom copper)
3. F.SilkS (Top silkscreen)
4. B.SilkS (Bottom silkscreen)
5. F.Mask (Top soldermask)
6. B.Mask (Bottom soldermask)
7. Edge.Cuts (Board outline)
8. F.Fab (Top fabrication layer - optional)
9. B.Fab (Bottom fabrication layer - optional)

**Drill files:**
1. PTH (Plated through-holes)
2. NPTH (Non-plated holes) - if any

**Additional files:**
1. Gerber job file (.gbrjob)
2. Pick and place file (.pos) - if using assembly service
3. BOM (CSV format)
4. Assembly drawing (PDF)

### Generating Gerbers in KiCad

1. Open PCB in KiCad
2. File → Plot
3. Select output directory: `gerbers/`
4. Select all required layers
5. Options:
   - Use Protel filename extensions: No
   - Generate Gerber job file: Yes
   - Subtract soldermask from silkscreen: Yes
   - Plot footprint values: Yes
   - Plot reference designators: Yes
6. Click "Plot"
7. Click "Generate Drill Files"
8. Drill file format: Excellon
9. Generate

---

## PCB Manufacturer Recommendations

### Recommended Manufacturers

**Professional Quality:**
- **JLCPCB** (China) - Low cost, fast turnaround
  - 2-layer: $2 for 5 boards (100×100mm)
  - 4-5 day production + shipping
  - Website: jlcpcb.com

- **PCBWay** (China) - Good quality
  - 2-layer: ~$5 for 10 boards
  - 3-5 day production
  - Website: pcbway.com

- **OSH Park** (USA) - High quality, purple PCBs
  - 2-layer: ~$10 per board
  - 12-day production
  - Made in USA
  - Website: oshpark.com

**Premium Quality:**
- **Eurocircuits** (Europe)
- **Advanced Circuits** (USA)
- **Seeed Studio** (China)

### Upload Instructions

1. Generate Gerber files (see above)
2. Compress all gerbers into ZIP file
3. Upload ZIP to manufacturer's website
4. Specify:
   - Board thickness: 1.6mm
   - Copper weight: 1oz (or 2oz for better current handling)
   - Surface finish: HASL lead-free
   - Soldermask color: Green (or black/blue)
   - Silkscreen color: White
   - Remove order number: Yes (if option available)

5. Review preview
6. Check dimensions (150mm × 100mm)
7. Order quantity (minimum 5 recommended for prototypes)

---

## Assembly Guidelines

### Assembly Order

**See [BUILD_GUIDE.md](../../docs/guides/BUILD_GUIDE.md) for complete instructions.**

**Quick reference:**
1. Solder resistors
2. Solder diodes (check polarity!)
3. Solder capacitors
4. Solder IC sockets (optional) or optocouplers (check pin 1!)
5. Solder TRIAC with heatsink
6. Solder terminal blocks
7. Solder fuse holder, MOV
8. Inspect all joints
9. Clean flux residue
10. Test isolation (>10MΩ)

### Soldering Notes

**For high-current traces:**
- Increase soldering iron temperature (350°C)
- Use more solder for larger pads
- Ensure good wetting on both pad and lead
- Preheat pad with iron before adding solder

**For optocouplers:**
- Limit heat exposure (<3 seconds per pin)
- Use lower temperature (320°C)
- Allow cooling between pins

**For TRIAC:**
- Apply thermal paste to tab before mounting
- Tighten heatsink mounting screw firmly
- Check for good thermal contact

---

## Testing and Inspection

### Visual Inspection

**Check for:**
- [ ] No solder bridges between pads
- [ ] All joints shiny and concave (not dull/grainy)
- [ ] All components correct orientation
- [ ] No missing components
- [ ] Proper clearances maintained
- [ ] No cracks in PCB
- [ ] No lifted pads

### Electrical Testing (POWER OFF)

**Continuity tests:**
- [ ] AC_LINE to AC_NEUTRAL: Open (>1MΩ)
- [ ] GND_MAINS to GND_ESP32: Open (>10MΩ)
- [ ] TRIAC MT2 to MT1: Open (>100kΩ)
- [ ] Verify all net connections per schematic

**Isolation test:**
- [ ] Megger test: >10MΩ between mains and ESP32 sections

### Functional Testing

**See [BUILD_GUIDE.md](../../docs/guides/BUILD_GUIDE.md) section "Testing Procedures"**

---

## Modifications and Variants

### Higher Current Version

**For loads >200W:**

Changes needed:
1. Increase trace widths to 3-4mm
2. Use 2oz copper (instead of 1oz)
3. Larger thermal vias array under TRIAC
4. Bigger heatsink mounting area
5. May need larger board size

### Dual Channel Version

**Two independent dimmers:**

Changes needed:
1. Wider board (200mm instead of 150mm)
2. Duplicate TRIAC section
3. Duplicate trigger optocoupler section
4. Share zero-cross detection
5. Additional ESP32 GPIO connection

### With Integrated Power Supply

**Add isolated AC-DC converter:**

Changes needed:
1. Add footprint for HLK-PM01 or similar
2. Additional clearances for converter
3. 5V output to ESP32 connector
4. May need larger board

---

## Troubleshooting PCB Issues

### Common Manufacturing Defects

| Issue | Cause | Solution |
|-------|-------|----------|
| **Traces too thin** | DRC violation | Increase trace width in design |
| **Clearance violation** | Insufficient spacing | Increase clearances, redesign |
| **Registration error** | Layer misalignment | Use better manufacturer |
| **Copper shorts** | Manufacturing defect | Request replacement |
| **Missing copper** | Under-etching | Request replacement |

### Common Assembly Issues

| Issue | Cause | Solution |
|-------|-------|----------|
| **Cold solder joints** | Insufficient heat/solder | Reheat with proper technique |
| **Solder bridges** | Too much solder | Remove with solder wick |
| **Lifted pads** | Excessive heat | Replace PCB or wire repair |
| **Component damage** | ESD or heat | Replace component |
| **Wrong orientation** | Assembly error | Desolder and correct |

---

## Design Verification Checklist

Before ordering PCBs:

- [ ] DRC passes with zero errors
- [ ] All clearances meet safety requirements (8mm mains-to-LV)
- [ ] All trace widths adequate for current
- [ ] All component footprints verified against datasheets
- [ ] Mounting holes present and correctly sized
- [ ] Board outline correct (150mm × 100mm)
- [ ] Silkscreen warnings present and legible
- [ ] Polarity indicators on all polarized components
- [ ] Thermal vias under TRIAC
- [ ] Isolation barrier clearly marked
- [ ] Ground planes assigned to correct nets
- [ ] 3D view checked for component collisions
- [ ] Gerbers visually inspected before upload

---

## Version History

### Version 1.0 (2025-01-11)

**Initial release:**
- 2-layer board, 150mm × 100mm
- Supports up to 10A load current
- Dual optocoupler isolation
- TRIAC on board edge for heatsink
- Proper creepage/clearance maintained
- Silkscreen safety warnings
- Designed in KiCad 7.0

---

## Support

**For PCB-related questions:**
1. Review this README
2. Check [BUILD_GUIDE.md](../../docs/guides/BUILD_GUIDE.md)
3. See [TROUBLESHOOTING.md](../../docs/TROUBLESHOOTING.md)
4. Open GitHub issue

**For KiCad help:**
- KiCad Docs: https://docs.kicad.org/
- KiCad Forum: https://forum.kicad.info/

---

**⚡ HIGH VOLTAGE PCB - BUILD RESPONSIBLY ⚡**

*PCB Documentation Version 1.0 - 2025-01-11*
