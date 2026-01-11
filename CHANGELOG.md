# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Planned Features
- Web interface for remote fan control
- MQTT integration for home automation
- Temperature-based automatic speed control
- Current sensing for load monitoring
- Multi-channel support (control multiple fans)
- Mobile app (Android/iOS)
- Over-the-air (OTA) firmware updates
- Data logging and analytics

## [1.0.0] - 2025-01-11

### Added - Initial Release

#### Firmware
- ESP-IDF based firmware for ESP32
- Phase-angle AC dimming control (0-100%)
- Zero-crossing detection with interrupt handling
- Precise timing using ESP high-resolution timer
- TRIAC trigger control with configurable pulse width
- Minimum speed enforcement (20% default) for motor stability
- Support for 50Hz and 60Hz AC frequencies
- Automatic demo mode cycling through speed settings
- Comprehensive error handling and logging
- Watchdog timer protection
- Optimized for 240MHz CPU operation
- IRAM-placed ISR for minimal latency

#### Hardware Design
- Complete circuit schematics (ASCII format)
- Bill of Materials (BOM) with detailed specifications
- PCB layout guidelines (creepage/clearance requirements)
- Dual optocoupler isolation (5000V rating)
- TRIAC-based power switching (BTA16-600B)
- Snubber circuit for inductive load protection
- MOV surge protection
- Zero-cross detection circuit
- Thermal management with heatsink
- Support for 110-240V AC operation
- Maximum load: 400W resistive, 200W inductive

#### Documentation
- Comprehensive README with project overview
- Complete SAFETY.md with critical safety information
- Detailed BUILD_GUIDE.md with step-by-step instructions
- TROUBLESHOOTING.md covering common issues
- Hardware documentation
- Code comments and inline documentation
- Circuit diagrams and explanations

#### Safety Features
- Optical isolation between mains and ESP32
- Minimum speed enforcement to prevent motor damage
- dV/dt protection on TRIAC gate
- Surge protection (MOV)
- Overcurrent protection (fuse)
- Thermal considerations and heatsink requirements
- Comprehensive safety warnings throughout documentation

#### Examples
- Basic speed control demonstration
- Configuration examples for different AC frequencies
- Comments showing usage patterns

### Notes

This is the initial release of the ESP32 AC Dimmer Fan Controller project.

**Safety Notice:** This release has been designed with safety as the top priority,
but it remains a high-voltage DIY project. Only qualified individuals should
attempt to build this device. See SAFETY.md for complete safety information.

**Testing Status:**
- Firmware compiled and verified on ESP-IDF v5.0+
- Circuit design based on proven phase-control techniques
- Extensive documentation and safety guidelines provided
- Real-world testing required before deployment

**Known Limitations:**
- No current sensing (planned for future release)
- No remote control capability (planned for future release)
- Manual configuration required (no web interface yet)
- Single channel only (multi-channel planned)

### Technical Specifications

- **Microcontroller:** ESP32 (240 MHz dual-core)
- **AC Input:** 110-240V AC, 50/60 Hz
- **Maximum Load:** 400W (resistive), 200W (inductive)
- **Control Range:** 20-100% (minimum enforced)
- **Timing Resolution:** 1 microsecond
- **Isolation:** 5000V (dual optocouplers)
- **TRIAC Rating:** 16A, 600V
- **Trigger Pulse:** 50 microseconds (optimized for inductive loads)

### Compatibility

- **ESP-IDF:** v5.0 or later
- **ESP32 Variants:** ESP32, ESP32-S3 (with pin changes)
- **AC Frequency:** 50Hz or 60Hz (configurable)
- **Load Types:** Induction motor fans (NOT compatible with electronic fans)

---

## Version Numbering

This project uses Semantic Versioning:

- **MAJOR version:** Incompatible API/hardware changes
- **MINOR version:** New functionality, backward-compatible
- **PATCH version:** Bug fixes, backward-compatible

### What Triggers a Version Bump?

**MAJOR (2.0.0):**
- Breaking changes to firmware API
- Hardware redesign requiring new PCB
- Incompatible configuration changes

**MINOR (1.1.0):**
- New features (web interface, MQTT, sensors)
- Additional supported hardware variants
- New configuration options
- Performance improvements

**PATCH (1.0.1):**
- Bug fixes
- Documentation improvements
- Code cleanup without functional changes
- Build system fixes

---

## How to Upgrade

### Firmware Updates

**From source:**
```bash
git pull
cd firmware
idf.py build
idf.py flash
```

**Check for breaking changes in changelog before upgrading.**

### Hardware Updates

Hardware changes between versions will be clearly documented.
PCB revisions may be required for major version changes.

---

## Contributing

See [CONTRIBUTING.md](CONTRIBUTING.md) for information on:
- Reporting bugs
- Suggesting features
- Submitting pull requests
- Development guidelines

---

## Links

- **Repository:** https://github.com/yourusername/esp32-ac-dimmer-fan-controller
- **Issues:** https://github.com/yourusername/esp32-ac-dimmer-fan-controller/issues
- **Discussions:** https://github.com/yourusername/esp32-ac-dimmer-fan-controller/discussions
- **Releases:** https://github.com/yourusername/esp32-ac-dimmer-fan-controller/releases

---

**⚡ Remember: Always prioritize safety when working with high voltage ⚡**
