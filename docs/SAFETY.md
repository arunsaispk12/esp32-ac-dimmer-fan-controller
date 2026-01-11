# Safety Information

# ⚡ DANGER: HIGH VOLTAGE ⚡

## THIS DOCUMENT CAN SAVE YOUR LIFE - READ IT COMPLETELY

This project involves **MAINS AC VOLTAGE** (110-240V AC) which is **LETHAL**.

**Improper construction, testing, or use can result in:**
- ⚠️ **ELECTRIC SHOCK**
- ⚠️ **SEVERE BURNS**
- ⚠️ **FIRE**
- ⚠️ **DEATH**

---

## Who Should Build This

### ✅ You SHOULD build this if:

- You have formal electrical training or education
- You have prior experience with mains voltage circuits
- You understand AC power theory and phase control
- You own proper safety equipment (see below)
- You have access to isolation transformers
- You can read and understand electrical schematics
- You know how to use a multimeter safely
- You understand TRIAC operation and characteristics
- You are comfortable with risk and take safety seriously

### ❌ You should NOT build this if:

- This is your first electronics project
- You have never worked with mains voltage
- You don't own/can't afford proper safety equipment
- You are under 18 years old (in most jurisdictions)
- You don't understand the circuit schematic
- You feel uncomfortable or unsure
- You don't have proper workspace and tools
- You cannot follow safety procedures meticulously

**When in doubt: DON'T BUILD IT.**

---

## Required Safety Equipment

You **MUST** have the following before starting:

### Essential Equipment

| Item | Purpose | Example |
|------|---------|---------|
| **Isolation Transformer** | Isolates you from ground | 1:1, 500VA+ rating |
| **Multimeter** | Testing (CAT III rated minimum) | Fluke 115, UNI-T UT139C |
| **Insulated Tools** | Safe manipulation | VDE/1000V rated screwdrivers |
| **Safety Glasses** | Eye protection | ANSI Z87.1 rated |
| **Non-conductive Mat** | Work surface | Rubber/silicone mat |
| **Fused Power Strip** | Overcurrent protection | 10A rated with fuse |
| **Fire Extinguisher** | Class C (electrical fires) | ABC dry chemical |
| **First Aid Kit** | Emergency response | Include burn treatment |
| **Proper Enclosure** | UL/CE rated enclosure | ABS/polycarbonate, IP rated |

### Highly Recommended

- **Variac/Variable Transformer** - Gradual voltage testing
- **Oscilloscope** (CAT III) - Waveform analysis and debugging
- **Megger/Insulation Tester** - Verify isolation (500V minimum)
- **Thermal Camera** - Hot spot detection
- **Non-contact Voltage Tester** - Quick safety checks

---

## Safety Rules - NO EXCEPTIONS

### Rule #1: ALWAYS Disconnect Power

- **ALWAYS** unplug from mains before touching the circuit
- **NEVER** work on a live circuit
- **NEVER** assume power is off - always verify with meter
- Use lockout/tagout if possible

### Rule #2: Isolation is Sacred

- **NEVER** bypass optocoupler isolation
- **NEVER** connect ESP32 ground to mains ground
- **ALWAYS** verify isolation with megger (>10 MΩ minimum)
- Maintain minimum 5mm creepage distance on PCB

### Rule #3: One Hand Rule

- Keep one hand in pocket or behind back when working near live circuits
- Prevents current path across your heart
- Even when using isolation transformer

### Rule #4: Proper Enclosure Required

- **NEVER** operate circuit outside of proper enclosure
- **NEVER** use open breadboard for final installation
- All mains connections must be fully shrouded
- Use strain relief on all cables

### Rule #5: Fusing is Mandatory

- **ALWAYS** use appropriate fuse (5A fast-blow recommended)
- Fuse must be in hot (line) conductor
- Replace blown fuse with same rating only
- Blown fuse = investigate cause before replacing

### Rule #6: Testing Protocol

- **ALWAYS** test with lamp before connecting fan
- Start with low voltage using variac
- Gradually increase voltage while monitoring
- Never skip testing steps

### Rule #7: Know Your Limits

- **STOP** immediately if you don't understand something
- **ASK** for help from qualified electrician if unsure
- **NEVER** rush or take shortcuts
- **NEVER** work while tired, distracted, or impaired

---

## Workspace Safety

### Environment Requirements

✅ **Dry location** - No moisture, no damp conditions
✅ **Well-lit** - Bright overhead lighting required
✅ **Clutter-free** - Clear workspace, organized tools
✅ **Well-ventilated** - Proper air circulation
✅ **Stable surface** - Solid workbench, no wobbling
✅ **Non-conductive floor** - Rubber mat or dry wood

❌ **No carpet** - Static discharge risk
❌ **No metal bench** - Use insulated work surface
❌ **No liquids nearby** - Coffee, water bottles, etc.
❌ **No distractions** - TV, loud music, interruptions

### Personal Safety

- Remove jewelry (rings, watches, bracelets)
- Wear insulated footwear (rubber soles)
- Dry hands - no moisture
- No loose clothing or long sleeves
- Tie back long hair
- Safety glasses at all times

---

## Construction Safety

### PCB Requirements

- **Minimum track spacing:** 5mm for mains voltage traces
- **Minimum creepage distance:** 5mm (8mm preferred)
- **Clearance through air:** 3mm minimum
- Use isolation slots between mains and low voltage
- No exposed mains traces on top layer
- Conformal coating on finished PCB

### Component Safety

| Component | Safety Requirement |
|-----------|-------------------|
| **TRIAC** | Must be on heatsink, thermally rated |
| **Optocouplers** | 5000V isolation minimum (MOC3021 meets this) |
| **Resistors** | Proper wattage rating with 50% derating |
| **Capacitors** | X2 rated for snubber, correct voltage rating |
| **Fuse Holder** | Rated for 250V AC minimum |
| **MOV** | Voltage rating matches mains (275V for 240V AC) |
| **Wire** | 18 AWG minimum, stranded, rated for temperature |
| **Terminals** | Screw terminals, 10A+ rating |

### Assembly Practices

1. **Solder quality:** Clean, shiny joints - no cold solder
2. **Component orientation:** Verify polarity before soldering
3. **Insulation:** Heat shrink on all connections
4. **Strain relief:** All cables properly clamped
5. **No shorts:** Visual inspection + continuity test
6. **Clean board:** Remove flux residue

---

## Testing Safety Procedures

### Pre-Power Checks (Power OFF)

1. ✅ Visual inspection - no obvious shorts or errors
2. ✅ Component orientation - all polarities correct
3. ✅ Continuity test - verify connections
4. ✅ Resistance test - check for shorts to ground
5. ✅ Isolation test - megger test >10 MΩ
6. ✅ Fuse installed - correct rating
7. ✅ Enclosure closed - all covers in place

### Initial Power-Up (Use Isolation Transformer)

1. ✅ ESP32 disconnected (test power circuit only first)
2. ✅ Connect isolation transformer
3. ✅ Connect variac (start at 0V)
4. ✅ Connect circuit to variac output
5. ✅ Slowly increase voltage to 24V
6. ✅ Check for smoke, heat, or unusual sounds
7. ✅ Measure zero-cross signal with scope
8. ✅ Gradually increase to full voltage
9. ✅ Monitor temperature of all components

### Lamp Test (MANDATORY Before Fan)

1. ✅ Connect 40-60W incandescent lamp as load
2. ✅ Power up with isolation transformer
3. ✅ Flash ESP32 firmware
4. ✅ Verify dimming from 0-100%
5. ✅ No flickering or instability
6. ✅ TRIAC temperature acceptable (<60°C)
7. ✅ No unusual sounds from circuit

### Fan Test (Only After Successful Lamp Test)

1. ✅ Use small fan first (50W or less)
2. ✅ Observe for 10 minutes at various speeds
3. ✅ Check for vibration or humming
4. ✅ Verify temperature of TRIAC and heatsink
5. ✅ Test emergency stop functionality
6. ✅ Only proceed to full load if all tests pass

---

## Emergency Procedures

### Electric Shock

**If someone is being shocked:**

1. **DO NOT TOUCH THEM** - you will be shocked too
2. **DISCONNECT POWER** - pull plug or trip breaker
3. **If not breathing:** Begin CPR immediately
4. **CALL EMERGENCY SERVICES** (911/112/etc.)
5. Keep victim warm and calm
6. Do not move victim if spinal injury suspected

**Prevention:**
- Always use isolation transformer
- One hand rule
- Non-conductive footwear
- Work with another person present

### Fire

**If circuit catches fire:**

1. **DISCONNECT POWER** if safe to do so
2. **USE CLASS C EXTINGUISHER** (CO₂ or dry chemical)
3. **NEVER USE WATER** on electrical fire
4. **EVACUATE** if fire spreads
5. **CALL FIRE DEPARTMENT**

**Prevention:**
- Proper fusing
- Component derating
- Temperature monitoring
- No combustibles near circuit

### Component Failure

**If you see/smell:**
- Smoke
- Sparks
- Burning smell
- Unusual sounds

**Action:**
1. **IMMEDIATELY DISCONNECT POWER**
2. Do not reconnect until failure is found and fixed
3. Inspect all components
4. Replace any damaged parts
5. Re-test completely before use

---

## Long-Term Safety

### Installation Requirements

- Mount in UL/CE rated enclosure (IP20 minimum)
- Proper ventilation for heat dissipation
- Accessible emergency disconnect
- Fuse or circuit breaker protection
- Ground fault protection (GFCI) recommended
- Appropriate wire gauge for current
- Secure mounting (vibration resistant)

### Regular Maintenance

**Monthly:**
- Visual inspection for damage
- Check for loose connections
- Verify proper operation
- Listen for unusual sounds

**Annually:**
- Thermal imaging inspection
- Re-torque all screw terminals
- Clean dust from enclosure
- Test emergency stop
- Verify isolation resistance

### Signs to Replace/Repair

🔴 **IMMEDIATE SHUTDOWN:**
- Burning smell
- Smoke
- Sparks or arcing
- Excessive heat (>80°C on TRIAC)
- Melted components
- Erratic operation

🟡 **Schedule Maintenance:**
- Discoloration on PCB
- Corroded connections
- Worn wire insulation
- Noisy operation
- Intermittent issues

---

## Legal and Liability

### Building This Device

- You assume **ALL RISK** for injury, death, or property damage
- Authors provide **NO WARRANTY** of any kind
- This design is for **EDUCATIONAL PURPOSES**
- You are responsible for compliance with local electrical codes
- Professional installation may be required by law

### Regulatory Compliance

This device may require:
- ✅ UL/CE certification for commercial use
- ✅ Licensed electrician for installation
- ✅ Building permit and inspection
- ✅ Homeowner's insurance disclosure
- ✅ Compliance with NEC/IEC electrical codes

**Consult local authorities before installation.**

### Insurance Implications

- Homemade electrical devices may void insurance
- Fire caused by DIY device may not be covered
- Inform your insurance company before installation
- Consider liability insurance if building for others

---

## When NOT to Proceed

### ❌ STOP if you answer YES to any:

- [ ] Is this your first high-voltage project?
- [ ] Do you lack required safety equipment?
- [ ] Are you unsure about any part of the circuit?
- [ ] Do you plan to use this commercially without certification?
- [ ] Will you skip any testing steps?
- [ ] Are you building this for someone else?
- [ ] Do you lack access to proper tools?
- [ ] Are you uncomfortable with the risks?
- [ ] Do you plan to leave circuit exposed (no enclosure)?
- [ ] Will others (children, pets) have access?

**If you checked ANY box above: DO NOT BUILD THIS PROJECT**

---

## Final Reminder

### This is NOT a toy. This is NOT for beginners.

**ONLY** proceed if you:
1. Understand every safety rule in this document
2. Have necessary equipment and expertise
3. Accept full responsibility for all risks
4. Will follow ALL safety procedures without exception
5. Have read the complete build guide
6. Understand AC power theory and TRIAC operation

---

## Emergency Contacts

**Keep these numbers accessible:**

- Emergency Services: _______________
- Electrician: _______________
- Poison Control: _______________
- Insurance Company: _______________

---

# IF IN DOUBT - DON'T DO IT

**Your life is worth more than a fan controller.**

---

*This safety document must be read and understood before proceeding with construction. Print and post in your workspace.*

**Last Updated:** 2025-01-11
