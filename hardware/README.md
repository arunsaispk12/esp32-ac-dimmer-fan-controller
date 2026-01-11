# Hardware Documentation

This directory contains hardware design files for the ESP32 AC Dimmer Fan Controller.

⚠️ **HIGH VOLTAGE PROJECT** - See [SAFETY.md](../docs/SAFETY.md) before proceeding.

---

## Directory Contents

### `/schematics`

Circuit schematics in various formats:
- KiCad schematic files (.kicad_sch)
- PDF exports for easy viewing
- Printable versions for assembly reference

**Note:** Schematics are also included in [BUILD_GUIDE.md](../docs/guides/BUILD_GUIDE.md) as ASCII diagrams.

### `/pcb`

PCB design files:
- KiCad PCB layout files (.kicad_pcb)
- Gerber files for manufacturing
- Drill files
- PDF plots (top, bottom, silkscreen)

**Design specifications:**
- **Board size:** ~80mm × 50mm (typical)
- **Layers:** 2-layer (suitable for DIY fabrication)
- **Copper weight:** 1oz (35µm) minimum
- **Finish:** HASL or ENIG
- **Soldermask:** Required
- **Silkscreen:** Required (safety markings)
- **Minimum trace/space:** 0.3mm / 0.3mm
- **Minimum drill:** 0.3mm

### `/bom`

Bill of Materials (BOM):
- `bom.csv` - Complete component list with specifications
- Excel/Google Sheets compatible
- Includes part numbers and suppliers

---

## Circuit Overview

### Block Diagram

```
AC Mains Input
      |
      ├─── Fuse (5A)
      |
      ├─── MOV (Surge Protection)
      |
      ├─── Zero-Cross Detection ──→ Optocoupler ──→ ESP32 GPIO34
      |
      └─── TRIAC + Snubber ←── Optocoupler ←── ESP32 GPIO25
                |
                └─── Fan Load Output
```

### Key Circuits

#### 1. Zero-Cross Detection
- Bridge rectifier (4× 1N4007)
- MOC3021 optocoupler
- Current limiting resistor (330Ω, 2W)
- Pull-up resistor (10kΩ)
- Noise filter capacitor (100nF)

#### 2. TRIAC Power Control
- BTA16-600B TRIAC with heatsink
- Gate drive circuit (180Ω resistor)
- Gate pull-down (1kΩ)
- dV/dt protection (10nF capacitor)

#### 3. Snubber Network (Critical for Inductive Loads)
- 47nF X2 capacitor (630V rated)
- 47Ω, 3W resistor
- Connected across TRIAC MT1-MT2

#### 4. Optical Isolation
- Two MOC3021 optocouplers
- 5000V isolation rating
- Separate mains and low-voltage grounds

---

## Safety-Critical Design Elements

### Isolation

**Primary Side (Mains Voltage):**
- AC input terminals
- Fuse and MOV
- Bridge rectifier
- TRIAC and snubber
- Load output terminals
- Optocoupler primary sides

**Secondary Side (Low Voltage):**
- ESP32 module
- USB programming interface
- Optocoupler secondary sides
- All signal traces

**Isolation barrier maintained:**
- Minimum 8mm creepage distance
- Minimum 4mm clearance
- Isolation tested at >10MΩ

### PCB Safety Features

1. **Creepage/Clearance:**
   - 8mm minimum between mains and low voltage
   - Isolation slots cut in PCB where needed
   - Wide spacing around TRIAC

2. **Thermal Management:**
   - TRIAC on heatsink (>7°C/W rating)
   - Thermal relief on high-current pads
   - Adequate copper area for heat spreading

3. **Mechanical:**
   - Strain relief for all wire connections
   - Secure mounting holes
   - Component positioning for assembly safety

4. **Marking:**
   - HIGH VOLTAGE warnings
   - Line/Neutral/Ground markings
   - Component reference designators
   - Polarity indicators
   - Test points labeled

---

## PCB Manufacturing Notes

### Specifications for Fab House

```
Board Parameters:
- Dimensions: [TBD based on final design]
- Layers: 2
- Copper Weight: 1oz (35µm)
- Material: FR-4, Tg 130°C minimum
- Thickness: 1.6mm standard
- Surface Finish: HASL lead-free or ENIG
- Soldermask: Both sides (green, black, or blue)
- Silkscreen: Both sides (white)
- Minimum Trace Width: 0.4mm
- Minimum Trace Spacing: 0.4mm
- Minimum Hole Size: 0.3mm
- Via Process: Tented
- Edge Connector: None
- Castellated Holes: None
- Impedance Control: Not required
```

### Recommended Manufacturers

**Professional (Low Cost):**
- JLCPCB
- PCBWay
- ALLPCB
- Seeed Studio

**Premium Quality:**
- OSH Park (USA)
- Eurocircuits (EU)
- Advanced Circuits (USA)

**Prototype Services:**
- OSH Park (purple PCBs)
- Aisler (Germany)
- PCBCart

### DIY Fabrication

**This PCB CAN be etched at home, but:**
- Requires precise alignment for 2-layer
- Creepage/clearance critical - measure carefully
- Through-hole plating recommended (via wires)
- Conformal coating recommended after assembly
- Professional fab recommended for safety reasons

**If DIY etching:**
1. Use photoresist method (not toner transfer)
2. Double-check all clearances with calipers
3. Tin all traces for corrosion protection
4. Wire all vias for layer connection
5. Apply conformal coating
6. Test isolation with megger before assembly

---

## Component Sourcing

### Critical Components (Don't Substitute)

| Component | Part Number | Why Critical |
|-----------|-------------|--------------|
| TRIAC | BTA16-600B | Voltage/current rating, isolated tab |
| Optocouplers | MOC3021 | Isolation voltage, random-phase |
| Snubber Cap | 47nF X2 rated | Safety rating for across-mains |
| MOV | 275V rating | Surge protection, voltage match |

### Generic Components (Substitution OK)

| Component | Specification | Notes |
|-----------|--------------|-------|
| Diodes | 1N4007 or equivalent | 1A, 1000V rating |
| Resistors | Standard values | Match wattage rating |
| Ceramic Caps | Standard values | Match voltage rating |

### Supplier Recommendations

**Worldwide:**
- Mouser Electronics
- Digi-Key
- Farnell/Newark
- RS Components

**Budget Options:**
- LCSC (China)
- AliExpress (longer shipping)
- eBay (verify authenticity)

**Local:**
- Prefer local distributors for faster shipping
- Support local electronics stores
- Build relationships for future projects

### Cost Estimate

**Per-unit cost (single quantity):**

| Category | Typical Cost (USD) |
|----------|-------------------|
| PCB (qty 5) | $2-5 per board |
| TRIAC + Heatsink | $2-3 |
| Optocouplers (2×) | $1-2 |
| Passive Components | $2-3 |
| Terminals | $1-2 |
| Enclosure | $5-10 |
| **Total** | **$15-25** |

**Volume pricing (qty 100):**
- Component cost drops ~50%
- PCB cost drops significantly
- Total: ~$8-12 per unit

---

## Assembly Notes

### Recommended Assembly Order

1. **Resistors** (lowest profile)
2. **Diodes** (check polarity)
3. **Ceramic capacitors**
4. **IC socket** (if using sockets)
5. **Optocouplers** (check orientation, avoid excessive heat)
6. **Film capacitor** (snubber)
7. **TRIAC** (heatsink with thermal paste)
8. **Terminal blocks**
9. **Fuse holder**
10. **MOV**

### Soldering Tips

- **Temperature:** 350°C for lead-free, 320°C for leaded
- **Time:** <3 seconds per joint
- **Flux:** Use flux pen for difficult joints
- **Cleaning:** Isopropyl alcohol after assembly
- **Inspection:** Magnifier or microscope for QC

### Testing Points

**Test points should be provided for:**
- AC input voltage
- Zero-cross signal
- TRIAC trigger signal
- ESP32 ground (isolated)
- TRIAC MT1, MT2, Gate

**Marking:** Label all test points on silkscreen.

---

## Modifications and Variants

### Higher Power Version

**For loads >200W:**
1. Use BTA24-600B or BTA41-600B TRIAC
2. Increase PCB trace width (2-4mm)
3. Larger heatsink (>5°C/W thermal resistance)
4. Higher current fuse (10A)
5. Heavier wire gauge (14 AWG)

### Dual Channel Version

**Two independent fan controllers:**
- Duplicate TRIAC + trigger circuits
- Single zero-cross detection shared
- Separate ESP32 GPIOs for each channel
- Larger enclosure required

### With Current Sensing

**Add current monitoring:**
- ACS712 current sensor module
- Connect in series with load
- ESP32 ADC input for feedback
- Enable over-current protection

### With Temperature Monitoring

**Add thermal shutdown:**
- NTC thermistor on TRIAC heatsink
- ESP32 ADC input
- Automatic shutdown if >80°C
- LED indicator for fault condition

---

## Design Files Format

### KiCad Version

All design files created with:
- **KiCad Version:** 7.0 or later
- **Library:** Standard KiCad libraries
- **Custom symbols:** Included in project
- **3D models:** Step files for major components

### Compatibility

**Older KiCad versions:**
- May need to regenerate symbol/footprint cache
- Check for compatibility warnings

**Other EDA tools:**
- Import Gerber files for viewing
- Schematic available as PDF
- Manual recreation may be needed

---

## Support Files

### 3D Models

**Included for:**
- TRIAC (TO-220 package)
- Optocouplers (DIP-6)
- Terminal blocks
- Heatsink

**Use for:**
- Mechanical clearance checking
- Enclosure fit verification
- Assembly visualization

### Assembly Drawings

**PDF exports include:**
- Top assembly view
- Bottom assembly view
- Silkscreen reference
- Dimension drawing

---

## Certification Notes

### Regulatory Compliance

**This is a DIY project and NOT certified for:**
- UL listing
- CE marking
- FCC compliance
- Commercial sale

**If certification needed:**
1. Professional PCB design review
2. Safety testing (hi-pot, isolation)
3. EMC testing (emissions, immunity)
4. Submit to certification body
5. Significant cost ($5,000-20,000+)

**For personal use:**
- Build at your own risk
- Follow all safety guidelines
- Comply with local electrical codes
- Professional installation recommended

---

## Contributing Hardware Designs

See [CONTRIBUTING.md](../CONTRIBUTING.md) for:
- How to submit design improvements
- PCB layout best practices
- Testing requirements
- Documentation standards

---

## License

Hardware designs licensed under MIT License (see [LICENSE](../LICENSE)).

**Note:** While the license is permissive, building mains-voltage devices carries serious safety responsibilities. Use good judgment and prioritize safety over convenience.

---

## Questions?

For hardware-specific questions:
1. Check [BUILD_GUIDE.md](../docs/guides/BUILD_GUIDE.md)
2. Review [TROUBLESHOOTING.md](../docs/TROUBLESHOOTING.md)
3. Open an issue on GitHub
4. Consult with qualified electrician

---

**⚡ HIGH VOLTAGE - BUILD RESPONSIBLY ⚡**

*Hardware Documentation Version 1.0 - 2025-01-11*
