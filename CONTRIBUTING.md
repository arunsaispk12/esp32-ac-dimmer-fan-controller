# Contributing to ESP32 AC Dimmer Fan Controller

Thank you for your interest in contributing to this project!

⚡ **SAFETY FIRST:** This is a high-voltage project. All contributions must maintain or improve safety standards.

---

## Table of Contents

1. [Code of Conduct](#code-of-conduct)
2. [How Can I Contribute?](#how-can-i-contribute)
3. [Reporting Bugs](#reporting-bugs)
4. [Suggesting Enhancements](#suggesting-enhancements)
5. [Pull Request Process](#pull-request-process)
6. [Coding Standards](#coding-standards)
7. [Hardware Contributions](#hardware-contributions)
8. [Documentation Contributions](#documentation-contributions)
9. [Testing Requirements](#testing-requirements)
10. [Community Guidelines](#community-guidelines)

---

## Code of Conduct

### Our Standards

- **Safety First:** Never compromise safety for features or convenience
- **Be Respectful:** Treat all contributors with respect and professionalism
- **Be Constructive:** Provide helpful feedback and suggestions
- **Be Patient:** Remember that contributors have varying skill levels
- **Be Inclusive:** Welcome contributors from all backgrounds

### Unacceptable Behavior

- Harassment or discriminatory language
- Deliberate safety violations or encouraging unsafe practices
- Trolling, insulting, or derogatory comments
- Publishing others' private information
- Other conduct which could reasonably be considered inappropriate

---

## How Can I Contribute?

### Reporting Bugs

Found a bug? Help us fix it!

**Before Submitting:**
1. Check existing [Issues](https://github.com/yourusername/esp32-ac-dimmer-fan-controller/issues)
2. Try the latest version from main branch
3. Review [TROUBLESHOOTING.md](docs/TROUBLESHOOTING.md)

**Bug Report Should Include:**
- Clear, descriptive title
- Steps to reproduce the issue
- Expected behavior vs. actual behavior
- ESP-IDF version
- ESP32 variant (ESP32, ESP32-S3, etc.)
- Hardware configuration (schematic if modified)
- Serial monitor output (use code blocks)
- Oscilloscope captures if relevant
- Any modifications made to the design

**Template:**

```markdown
## Bug Description
Brief description of the issue

## Steps to Reproduce
1. Set up hardware as...
2. Flash firmware with...
3. Set fan speed to...
4. Observe...

## Expected Behavior
Fan should...

## Actual Behavior
Fan instead...

## Environment
- ESP-IDF Version: v5.1
- ESP32 Variant: ESP32-DevKitC
- AC Frequency: 50Hz
- Load: 100W ceiling fan
- Modifications: None

## Serial Output
```
[Paste serial monitor output here]
```

## Additional Context
- Oscilloscope shows...
- This happens when...
```

### Suggesting Enhancements

Have an idea? We'd love to hear it!

**Enhancement Suggestions Should Include:**
- Clear, descriptive title
- Detailed description of the proposed feature
- Use cases and benefits
- Implementation suggestions (if applicable)
- Potential safety implications
- Backward compatibility considerations

**Template:**

```markdown
## Feature Description
Brief description of the feature

## Motivation
Why is this feature needed?
What problem does it solve?

## Proposed Solution
How would this work?
What would the API/interface look like?

## Alternatives Considered
What other approaches did you consider?

## Safety Implications
Does this affect high-voltage components?
Are there safety risks to consider?

## Additional Context
Links, examples, diagrams, etc.
```

---

## Pull Request Process

### Before You Start

1. **Open an Issue First:** Discuss major changes before investing time
2. **Check Existing PRs:** Avoid duplicate work
3. **Fork the Repository:** Work in your own fork
4. **Create a Branch:** Use descriptive branch names

### Branch Naming

```
feature/add-temperature-sensor
bugfix/triac-overheating
docs/improve-safety-guide
hardware/pcb-revision-2
```

### Commit Messages

Follow the [Conventional Commits](https://www.conventionalcommits.org/) specification:

```
type(scope): Brief description

Longer description if needed

Fixes #issue_number
```

**Types:**
- `feat`: New feature
- `fix`: Bug fix
- `docs`: Documentation only
- `style`: Formatting, no code change
- `refactor`: Code restructuring
- `test`: Adding tests
- `chore`: Maintenance tasks
- `hardware`: Hardware design changes

**Examples:**

```
feat(firmware): Add temperature monitoring via NTC thermistor

Implement ADC reading of NTC thermistor mounted on TRIAC heatsink.
Automatic shutdown if temperature exceeds 80°C.

Fixes #23
```

```
fix(firmware): Correct zero-cross detection timing at 60Hz

AC_HALF_PERIOD was incorrect for 60Hz operation.
Changed from 8000us to 8333us.

Fixes #45
```

```
docs(safety): Add section on MOV replacement procedure

Clarify when and how to replace degraded MOV components.
Include testing procedure.
```

### Pull Request Guidelines

**Your PR Should:**
1. Have a clear, descriptive title
2. Reference related issues (`Fixes #123`, `Relates to #456`)
3. Include a detailed description of changes
4. Pass all checks (if CI/CD is set up)
5. Include tests if applicable
6. Update documentation if needed
7. Maintain backward compatibility (or document breaking changes)

**PR Template:**

```markdown
## Description
Brief description of changes

## Type of Change
- [ ] Bug fix (non-breaking change which fixes an issue)
- [ ] New feature (non-breaking change which adds functionality)
- [ ] Breaking change (fix or feature that would cause existing functionality to not work as expected)
- [ ] Documentation update
- [ ] Hardware design change

## Testing
How has this been tested?
- [ ] Tested with incandescent lamp
- [ ] Tested with 100W fan
- [ ] Tested at all speed ranges
- [ ] Thermal testing completed
- [ ] Verified isolation (>10MΩ)
- [ ] Oscilloscope verification

## Checklist
- [ ] My code follows the project's coding standards
- [ ] I have commented my code where necessary
- [ ] I have updated the documentation
- [ ] My changes generate no new warnings
- [ ] I have tested my changes thoroughly
- [ ] I have considered safety implications
- [ ] I have updated CHANGELOG.md

## Safety Considerations
Does this change affect:
- [ ] Isolation between mains and ESP32
- [ ] TRIAC triggering or timing
- [ ] High-voltage circuitry
- [ ] PCB clearances/creepage
- [ ] Thermal management

If yes, describe safety testing performed:

## Screenshots/Oscilloscope Captures
If applicable, add screenshots or scope captures

## Additional Notes
Any other information
```

### Review Process

1. **Maintainer Review:** At least one maintainer will review
2. **Safety Review:** Changes affecting safety require extra scrutiny
3. **Testing:** May request additional testing for critical changes
4. **Feedback:** Address review comments
5. **Approval:** Once approved, maintainer will merge

---

## Coding Standards

### C Language (Firmware)

**Style Guidelines:**

```c
// Use 4 spaces for indentation (NO TABS)
void function_name(uint8_t parameter)
{
    if (condition) {
        // Code here
    }
}

// Maximum line length: 100 characters
// Break long lines at logical points

// Use descriptive variable names
uint32_t trigger_delay_us;  // Good
uint32_t td;                // Bad

// Constants in UPPER_CASE
#define MAX_SPEED_PERCENT 100
#define TRIAC_PULSE_WIDTH 50

// Function names in snake_case
static void init_gpio(void);
static uint32_t speed_to_delay(uint8_t speed_percent);

// Comments: Explain WHY, not WHAT
// Good:
// Use longer pulse for inductive loads to ensure TRIAC latches
#define TRIAC_PULSE_WIDTH 50

// Bad:
// Set pulse width to 50
#define TRIAC_PULSE_WIDTH 50
```

**Documentation:**

```c
/**
 * Brief function description
 *
 * More detailed description if needed.
 * Can span multiple lines.
 *
 * @param param1 Description of parameter
 * @param param2 Description of parameter
 * @return Description of return value
 */
uint32_t example_function(uint8_t param1, uint32_t param2)
{
    // Implementation
}
```

**Safety-Critical Code:**

```c
// Mark ISRs and time-critical functions
static void IRAM_ATTR zero_cross_isr_handler(void *arg)
{
    // ISR code - keep minimal
}

// Use critical sections for shared variables
portENTER_CRITICAL(&spinlock);
shared_variable = new_value;
portEXIT_CRITICAL(&spinlock);

// Always validate inputs for safety
static void set_fan_speed(uint8_t speed_percent)
{
    if (speed_percent > MAX_SPEED_PERCENT) {
        ESP_LOGE(TAG, "Invalid speed: %d", speed_percent);
        return;
    }

    // Enforce minimum speed
    if (speed_percent > 0 && speed_percent < MIN_SPEED_PERCENT) {
        speed_percent = MIN_SPEED_PERCENT;
    }

    // Implementation...
}
```

### Python (Tools/Scripts)

Follow [PEP 8](https://www.python.org/dev/peps/pep-0008/):

```python
# Use 4 spaces for indentation
# Maximum line length: 100 characters
# Function and variable names in snake_case
# Class names in PascalCase

def calculate_delay(speed_percent, ac_frequency):
    """
    Calculate phase delay for given speed.

    Args:
        speed_percent: Fan speed (0-100)
        ac_frequency: AC frequency in Hz (50 or 60)

    Returns:
        Delay in microseconds
    """
    # Implementation
    pass
```

### Markdown (Documentation)

```markdown
# Use ATX-style headers

## Prefer descriptive section titles

- Use bullet points for lists
- Keep lines under 100 characters where possible
- Use blank lines between sections for readability

**Bold for emphasis**
*Italic for terms*
`Code in backticks`

```code
Use fenced code blocks with language specifiers
```

[Links with descriptive text](https://example.com)

⚠️ Use emoji sparingly (safety warnings OK)
```

---

## Hardware Contributions

### Schematic Changes

**Requirements:**
- Provide KiCad schematic files
- Include PDF export
- Document all changes in commit message
- Explain rationale for changes
- Consider safety implications

**Checklist:**
- [ ] Proper isolation maintained (>8mm creepage)
- [ ] Component ratings verified
- [ ] Safety features preserved
- [ ] Backward compatibility considered
- [ ] BOM updated

### PCB Layout Changes

**Requirements:**
- Provide KiCad PCB files
- Include Gerber files
- Document design rule changes
- Verify clearances/creepage

**Checklist:**
- [ ] Clearance rules met (4mm+ for HV)
- [ ] Creepage rules met (8mm+ for HV)
- [ ] Thermal management adequate
- [ ] Mounting holes compatible
- [ ] Silkscreen updated

### BOM Updates

**Format:**
```csv
Reference,Quantity,Description,Value,Part Number,Manufacturer,Package,Notes,Supplier,Cost
```

**Requirements:**
- Maintain CSV format
- Include all fields
- Verify component availability
- Update costs if known
- Note any substitutions

---

## Documentation Contributions

### What Needs Documentation

- New features
- Configuration changes
- Hardware modifications
- Troubleshooting procedures
- Safety considerations
- Build procedures

### Documentation Standards

**Structure:**
- Clear headings and sections
- Table of contents for long documents
- Code examples where helpful
- Diagrams and schematics
- Safety warnings prominently displayed

**Language:**
- Clear, concise English
- Define technical terms
- Assume reader has basic knowledge
- Include links to related sections

**Safety Documentation:**
- ALWAYS include relevant warnings
- Use ⚡ emoji for high voltage warnings
- Explain WHY safety rules exist
- Provide emergency procedures

---

## Testing Requirements

### Firmware Testing

**Before Submitting PR:**

1. **Compilation:**
   ```bash
   idf.py build
   # Must compile without errors or warnings
   ```

2. **Lamp Test:**
   - Test with 40-60W incandescent lamp
   - Verify all speed ranges (0%, 25%, 50%, 75%, 100%)
   - Monitor for 10 minutes minimum

3. **Fan Test:**
   - Test with actual fan load
   - Verify smooth operation
   - Check thermal performance
   - Run for 30 minutes minimum

4. **Isolation Test:**
   - Verify >10MΩ isolation
   - Test with megger if available

5. **Oscilloscope Verification:**
   - Verify zero-cross timing
   - Verify trigger pulses
   - Check for noise/interference

### Hardware Testing

**For Hardware Changes:**

1. **Simulation:** LTSpice or similar (if applicable)
2. **Breadboard Prototype:** Test on breadboard first
3. **PCB Prototype:** Order small batch (5-10 boards)
4. **Component Testing:** Verify all components
5. **Thermal Testing:** Run at full load for 1 hour
6. **Safety Testing:** Isolation, fuse operation, fault modes

### Documentation Testing

**Before Submitting:**

1. **Spelling and Grammar:** Use spell checker
2. **Link Verification:** All links work
3. **Code Examples:** Test all code snippets
4. **Build Instructions:** Follow your own instructions
5. **Readability:** Have someone else review

---

## Community Guidelines

### Communication Channels

- **GitHub Issues:** Bug reports, feature requests
- **GitHub Discussions:** Questions, ideas, general discussion
- **Pull Requests:** Code/hardware/doc contributions

### Response Times

- Maintainers aim to respond within 7 days
- Community members encouraged to help each other
- Patience appreciated - this is a volunteer project

### Getting Help

**If you're stuck:**

1. Read the documentation thoroughly
2. Check existing issues and discussions
3. Try [TROUBLESHOOTING.md](docs/TROUBLESHOOTING.md)
4. Open a new issue with detailed information
5. Be patient and respectful

### Recognition

**Contributors will be:**
- Listed in the project contributors
- Credited in release notes for significant contributions
- Thanked publicly (if desired)

---

## Safety-First Development

### Critical Safety Rules for Contributors

1. **Never compromise isolation:** Mains voltage must never reach ESP32
2. **Always include safety warnings:** If your change affects safety, document it
3. **Test thoroughly:** More testing for safety-critical changes
4. **Question everything:** If unsure about safety, ask
5. **Document failure modes:** What happens if your code/circuit fails?

### Safety Review Process

**Changes affecting these areas require extra review:**

- Optocoupler circuits
- TRIAC triggering
- Timing-critical code (ISRs)
- PCB clearances/creepage
- Power supply design
- Isolation barriers

### Red Flags (These will be rejected)

❌ Reducing isolation distances
❌ Removing safety features
❌ Bypassing minimum speed enforcement
❌ Inadequate testing for safety changes
❌ Missing safety documentation
❌ Encouraging unsafe practices

---

## License

By contributing, you agree that your contributions will be licensed under the MIT License.

You also acknowledge that:
- Your contribution does not infringe on others' rights
- You have the right to contribute the code/design
- Your contribution maintains the project's safety standards

---

## Questions?

Not sure about something? Don't hesitate to ask!

- Open a [Discussion](https://github.com/yourusername/esp32-ac-dimmer-fan-controller/discussions)
- Ask in an existing issue
- Contact the maintainers

---

## Thank You!

Your contributions make this project better and safer. We appreciate your time and effort!

**Remember: Safety first, always.**

⚡ **When in doubt, ask. It's better to be safe than sorry.** ⚡

---

*Last Updated: 2025-01-11*
