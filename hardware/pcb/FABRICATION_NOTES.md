# PCB Fabrication Notes

Manufacturing specifications and requirements for ESP32 AC Dimmer Fan Controller PCB.

⚡ **HIGH VOLTAGE PCB** - Special fabrication requirements for safety.

---

## Manufacturing Specification Sheet

### PCB Parameters

```
BOARD SPECIFICATIONS
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

Board Name:           ESP32 AC Dimmer Fan Controller
Version:              1.0
Date:                 2025-01-11
Designer:             [Your Name/Company]

DIMENSIONS
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
Length:               150.00 mm  ±0.2mm
Width:                100.00 mm  ±0.2mm
Thickness:            1.60 mm    ±0.16mm
Tolerance:            ±0.15mm

LAYERS
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
Layer Count:          2 (Top and Bottom)
Copper Weight:        1 oz (35 μm) standard
                      2 oz (70 μm) recommended for high current

MATERIAL
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
Base Material:        FR-4
Glass Transition:     Tg 130°C minimum
                      Tg 150°C or higher recommended
Flammability:         UL94 V-0
Dielectric Constant:  4.5 @ 1MHz
```

---

## Copper Specifications

### Copper Weight Options

| Option | Thickness | Current Capacity | Cost | Recommendation |
|--------|-----------|------------------|------|----------------|
| **1 oz** | 35 μm | ~7A (2mm trace) | Standard | OK for <5A loads |
| **2 oz** | 70 μm | ~12A (2mm trace) | +20% | Recommended for safety margin |
| **3 oz** | 105 μm | ~16A (2mm trace) | +50% | Only if >10A loads |

**Recommended:** 2oz copper for better current handling and thermal performance.

### Minimum Trace/Space

| Feature | Minimum | Recommended |
|---------|---------|-------------|
| Trace Width | 0.15mm | 0.2mm |
| Trace Spacing | 0.15mm | 0.2mm |
| High Voltage Spacing | 4.0mm | 8.0mm (creepage) |

**Critical:** High voltage traces must maintain 8mm creepage distance to low voltage sections.

---

## Drill Specifications

### Hole Sizes

| Hole Type | Diameter | Plating | Quantity | Purpose |
|-----------|----------|---------|----------|---------|
| Mounting | 3.2mm | No | 4 | M3 screw holes |
| Via | 0.3mm | Yes | ~50 | Electrical connection |
| Component | 0.8-1.1mm | Yes | ~60 | Through-hole components |
| Thermal | 0.6mm | Yes | ~25 | Heat dissipation |

### Drill Tolerances

- Drill tolerance: ±0.05mm
- Hole position tolerance: ±0.1mm
- Plating thickness: 25 μm minimum

### Drill File Format

- Format: Excellon
- Units: Millimeters
- Zero suppression: Leading
- Coordinates: Absolute

---

## Surface Finish Options

### Recommended Finishes

| Finish | Shelf Life | Solderability | Cost | Best For |
|--------|-----------|---------------|------|----------|
| **HASL Lead-Free** | 12 months | Excellent | Standard | Prototype/production |
| **ENIG** | 12 months | Excellent | +50% | Fine pitch, gold contact |
| **OSP** | 6 months | Good | -10% | Budget builds |
| **Immersion Silver** | 12 months | Excellent | +30% | High frequency |

**Recommended:** HASL Lead-Free for best balance of cost and performance.

**Notes:**
- HASL suitable for hand soldering
- ENIG better for fine pitch (not needed here)
- OSP cheaper but shorter shelf life
- All finishes RoHS compliant

---

## Soldermask Specifications

### Soldermask Requirements

| Parameter | Specification |
|-----------|--------------|
| **Type** | Liquid photoimageable (LPI) |
| **Color** | Green (standard), Black/Blue optional |
| **Thickness** | 10-30 μm |
| **Coverage** | Both sides |
| **Finish** | Matte or glossy |

### Soldermask Clearance

- Pad clearance: 0.05mm (swell)
- Via tenting: Yes (cover vias with mask)
- Dam between pads: 0.15mm minimum

### Color Options

| Color | Visibility | Inspection | Cost | Notes |
|-------|-----------|------------|------|-------|
| Green | Good | Excellent | Standard | Best for inspection |
| Black | Excellent | Poor | +$5 | Professional look |
| Blue | Good | Good | +$2 | Good compromise |
| Red | Good | Good | +$2 | High visibility |
| White | Poor | Fair | +$5 | Hard to read silkscreen |

**Recommended:** Green for best inspection visibility.

---

## Silkscreen Specifications

### Silkscreen Requirements

| Parameter | Specification |
|-----------|--------------|
| **Color** | White (standard on dark soldermask) |
| **Sides** | Both (F.SilkS and B.SilkS) |
| **Min Line Width** | 0.15mm |
| **Min Text Height** | 0.8mm (1.0mm recommended) |
| **Min Text Thickness** | 0.15mm |

### Silkscreen Content

**Must include:**
- Component reference designators (R1, C1, U1, Q1, etc.)
- Polarity markers (diodes, optocouplers)
- Pin 1 indicators (dots, squares)
- Board name and version
- High voltage warnings
- Terminal labels

**Silkscreen warnings required:**
```
⚡ HIGH VOLTAGE ⚡
DANGER: 110-240V AC
ESP32 AC DIMMER v1.0
ISOLATION BARRIER - 8mm MINIMUM
```

**Text size:**
- Warnings: 2.0mm height, bold
- Board ID: 1.5mm height
- Component refs: 1.0mm height
- Notes: 0.8mm height

---

## Special Requirements

### Creepage and Clearance (CRITICAL SAFETY)

**IEC 60950-1 Compliance:**

| Voltage | Working | Minimum Clearance | Minimum Creepage | Applied |
|---------|---------|------------------|------------------|---------|
| 110V AC | 155V peak | 1.5mm | 2.5mm | Not used (240V design) |
| 240V AC | 340V peak | 3.0mm | 5.0mm | Mains-to-mains |
| 240V AC | 340V peak | 4.0mm | 8.0mm | Mains-to-low voltage |

**Our design uses 8mm creepage for maximum safety margin.**

### Isolation Slots

**Purpose:** Break creepage path between high and low voltage.

**Specification:**
- Width: 1.0mm minimum (2.0mm recommended)
- Depth: Through PCB (routed slot)
- Location: Along isolation barrier at 145mm X position
- Length: Full board height minus mounting holes

**Manufacturing:**
- Use router or V-groove cutter
- Clean edges (no copper burrs)
- Maintain soldermask over edges

**Optional:** May be omitted if 8mm clearance maintained by copper pours.

---

## Quality Control Requirements

### Electrical Testing

**Manufacturer should perform:**

1. **Continuity Test:**
   - Verify all nets connected
   - Check for opens

2. **Isolation Test:**
   - Test between all nets
   - Minimum isolation: >10MΩ

3. **Hi-Pot Test (Optional, recommended):**
   - 500V DC between mains and low voltage
   - Duration: 1 second
   - Leakage: <1 μA

### Visual Inspection

**Check for:**
- [ ] Soldermask complete coverage
- [ ] No exposed copper in isolation zone
- [ ] Silkscreen legible and complete
- [ ] No scratches or contamination
- [ ] Drill holes clean and plated
- [ ] Board outline smooth
- [ ] Mounting holes correct size

### Dimensional Inspection

**Verify:**
- [ ] Board dimensions: 150mm × 100mm ±0.2mm
- [ ] Mounting hole positions: ±0.1mm
- [ ] Board thickness: 1.6mm ±0.16mm
- [ ] Hole diameters: Within ±0.05mm

---

## Gerber File Checklist

Before sending to manufacturer:

### Files to Include

- [ ] F.Cu.gbr (Top copper layer)
- [ ] B.Cu.gbr (Bottom copper layer)
- [ ] F.SilkS.gbr (Top silkscreen)
- [ ] B.SilkS.gbr (Bottom silkscreen)
- [ ] F.Mask.gbr (Top soldermask)
- [ ] B.Mask.gbr (Bottom soldermask)
- [ ] Edge.Cuts.gbr (Board outline)
- [ ] PTH.drl (Plated through-holes drill file)
- [ ] NPTH.drl (Non-plated holes drill file, if any)
- [ ] *.gbrjob (Gerber job file)

### File Verification

**Use Gerber viewer to check:**
- [ ] All layers present and correct
- [ ] Board outline correct (150×100mm)
- [ ] Mounting holes at correct positions
- [ ] No missing copper pours
- [ ] Isolation barrier visible
- [ ] Silkscreen not over pads
- [ ] High voltage warnings present
- [ ] No overlapping pads or traces

**Recommended viewers:**
- KiCad built-in Gerber viewer
- gerbv (Linux/Mac/Windows)
- ViewMate (Windows)
- Online: https://www.gerber-viewer.com/

---

## Manufacturing Process Flow

### Standard PCB Fabrication Process

```
1. Gerber File Review
   ↓
2. Engineering Questions (EQ)
   → Manufacturer may ask clarifications
   ↓
3. Film Generation
   → Photomasks created from Gerbers
   ↓
4. Copper Clad Lamination
   → FR-4 + copper sheets
   ↓
5. Drilling
   → Holes drilled per drill files
   ↓
6. Plating
   → Copper plated in holes
   ↓
7. Imaging
   → Pattern transferred to copper
   ↓
8. Etching
   → Unwanted copper removed
   ↓
9. Soldermask Application
   → Green mask applied and cured
   ↓
10. Silkscreen Printing
    → White text/graphics printed
    ↓
11. Surface Finish
    → HASL or ENIG applied
    ↓
12. Routing/V-Scoring
    → Board cut to final shape
    ↓
13. Electrical Testing
    → Continuity and isolation tested
    ↓
14. Visual Inspection
    → Quality check
    ↓
15. Packaging and Shipping
```

**Typical timeline:** 2-5 days production + shipping

---

## Manufacturer Upload Instructions

### JLCPCB Upload

1. Go to https://cart.jlcpcb.com/quote
2. Click "Add gerber file"
3. Upload ZIP containing all Gerber files
4. Wait for automatic detection
5. Verify specifications:
   - **Dimensions:** 150×100mm
   - **PCB Qty:** 5 (minimum)
   - **Layers:** 2
   - **PCB Thickness:** 1.6mm
   - **PCB Color:** Green
   - **Silkscreen:** White
   - **Surface Finish:** HASL(with lead) or LeadFree HASL
   - **Copper Weight:** 1oz or 2oz
   - **Gold Fingers:** No
   - **Confirm Production file:** Yes
   - **Remove Order Number:** Yes (specify location if No)
6. Review rendered preview
7. Add to cart and checkout

### PCBWay Upload

1. Go to https://www.pcbway.com/orderonline.aspx
2. Upload Gerber ZIP file
3. Fill in specifications:
   - Same as JLCPCB above
   - **Build Time:** 3-5 days
   - **Different Design:** 1
4. Review and calculate
5. Add to cart

### OSH Park Upload

1. Go to https://oshpark.com/
2. Click "Get Started Now"
3. Upload .kicad_pcb file OR Gerber ZIP
4. Review automatically detected dimensions
5. Specifications (automatic):
   - 2-layer
   - 1.6mm thickness
   - Purple soldermask
   - ENIG finish
6. Enter quantity
7. Add to cart

**Note:** OSH Park prices per board, others price per design.

---

## Cost Estimation

### Price Comparison (5 boards, 2-layer, 150×100mm)

| Manufacturer | Base Price | Shipping (Express) | Total | Lead Time |
|--------------|------------|-------------------|-------|-----------|
| JLCPCB | $2 | $18-30 | $20-32 | 3-4 days + ship |
| PCBWay | $5 | $20-35 | $25-40 | 3-5 days + ship |
| OSH Park | $50 | $8 (USPS) | $58 | 12 days total |
| Seeed | $5 | $15-25 | $20-30 | 5-7 days + ship |

**Notes:**
- Prices for standard options (1oz copper, HASL, green)
- 2oz copper adds ~$10-20
- ENIG finish adds ~$20-30
- Expedited production adds ~$30-50

### Quantity Discounts

| Quantity | JLCPCB | PCBWay | Notes |
|----------|--------|--------|-------|
| 5 | $2 | $5 | Minimum order |
| 10 | $2 | $8 | Sweet spot |
| 30 | $15 | $40 | Small production |
| 100 | $60 | $120 | Production run |

**Recommendation:** Order 10 boards for prototyping (low cost, enough for testing and spares).

---

## Common Manufacturing Issues

### Issues and Solutions

| Issue | Cause | Prevention |
|-------|-------|-----------|
| **Thin traces** | Width below capability | Follow design rules, min 0.2mm |
| **Spacing violation** | Clearance too small | DRC check, min 0.2mm (8mm HV) |
| **Drill breakage** | Hole too small | Min 0.3mm drill size |
| **Misalignment** | Poor registration | Use reputable manufacturer |
| **Copper short** | Manufacturing defect | Request replacement |
| **Missing features** | Gerber generation error | Review Gerbers before upload |

### Rejection Reasons

**Manufacturer may reject if:**
- Clearances too small for their capability
- Holes too small to drill reliably
- Trace widths below minimum
- Missing required layers
- Gerber file format errors
- Board size outside limits

**Solution:** Fix issues and re-upload, or choose manufacturer with better capabilities.

---

## Post-Fabrication

### Incoming Inspection

When boards arrive:

1. **Visual Check:**
   - [ ] Correct quantity received
   - [ ] No visible damage
   - [ ] Soldermask complete
   - [ ] Silkscreen legible
   - [ ] Correct color

2. **Dimensional Check:**
   - [ ] Measure board size
   - [ ] Check mounting holes
   - [ ] Verify thickness (calipers)

3. **Electrical Check:**
   - [ ] Test isolation (multimeter)
   - [ ] Check for shorts (continuity mode)

### Storage

**Before assembly:**
- Store in anti-static bags
- Keep in dry environment (<60% humidity)
- Avoid extreme temperatures
- Shelf life: 12 months (HASL), 6 months (OSP)

**If storage >6 months:**
- May need rebaking (125°C, 4 hours)
- Check surface finish condition
- Test solderability on scrap board

---

## Certifications and Compliance

### Standards Compliance

This PCB design considers:

- **IEC 60950-1:** Safety requirements for electrical equipment
- **UL94 V-0:** Flammability rating for PCB material
- **RoHS:** Restriction of Hazardous Substances (lead-free)
- **REACH:** European chemicals regulation

**Note:** PCB fabrication meets standards, but complete product certification requires full device testing.

### UL Recognition

**For UL listing:**
- PCB manufacturer should be UL recognized
- Material must be UL94 V-0 rated
- Spacing must meet UL standards
- Additional testing required

**Check manufacturer UL number before ordering for commercial products.**

---

## Technical Support

### Manufacturer Engineering Questions (EQ)

**Common EQ from manufacturers:**

1. **"Trace spacing violation"**
   - Response: "Please maintain 8mm spacing between mains and low voltage sections as designed. This is safety-critical."

2. **"Unusual board size"**
   - Response: "150mm × 100mm is intentional for component layout and heatsink clearance."

3. **"Remove order number?"**
   - Response: "Yes, remove order number. Specify location if mandatory: bottom side, near edge."

4. **"Copper weight confirmation"**
   - Response: "2oz copper preferred for better current handling, but 1oz acceptable for prototypes."

### Contact Information

**For fabrication questions:**
- Email manufacturer tech support
- Include board name and order number
- Reference this fabrication notes document

---

## Appendix: Manufacturer Comparison

### Feature Comparison

| Feature | JLCPCB | PCBWay | OSH Park | Seeed |
|---------|--------|--------|----------|-------|
| **Min Price** | $$$ | $$$ | $$$$$ | $$$ |
| **Quality** | Good | Good | Excellent | Good |
| **Speed** | Fast | Fast | Slow | Medium |
| **Support** | Good | Excellent | Good | Good |
| **Min Qty** | 5 | 5 | 3 | 5 |
| **Max Layers** | 14 | 14 | 4 | 14 |
| **Min Trace** | 0.09mm | 0.09mm | 0.13mm | 0.1mm |
| **Assembly** | Yes | Yes | No | Yes |

**Recommendation:** JLCPCB or PCBWay for best balance of cost, quality, and speed.

---

**This completes the fabrication notes. Always verify specifications with manufacturer before ordering.**

⚡ **HIGH VOLTAGE PCB - MANUFACTURE WITH CARE** ⚡

*Fabrication Notes Version 1.0 - 2025-01-11*
