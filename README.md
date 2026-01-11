# ESP32 AC Dimmer Fan Controller

![High Voltage Warning](https://img.shields.io/badge/⚡-HIGH_VOLTAGE-red?style=for-the-badge)
![License](https://img.shields.io/badge/license-MIT-blue.svg)
![ESP-IDF](https://img.shields.io/badge/ESP--IDF-v5.0+-green.svg)
![Platform](https://img.shields.io/badge/platform-ESP32-blue.svg)

A professional-grade AC phase-control dimmer for controlling fan speed using ESP32 and ESP-IDF framework. Designed specifically for inductive loads with proper snubber circuits and safety features.

## ⚠️ CRITICAL SAFETY WARNING

**THIS PROJECT INVOLVES POTENTIALLY LETHAL MAINS VOLTAGE**

- This device controls AC mains voltage (110-240V AC)
- Improper construction or use can result in **ELECTRIC SHOCK, FIRE, OR DEATH**
- Only qualified personnel with electrical experience should build this
- See [SAFETY.md](docs/SAFETY.md) for complete safety information
- **IF IN DOUBT, DO NOT BUILD THIS PROJECT**

## Features

- ✅ **Phase-angle dimming** for smooth fan speed control (0-100%)
- ✅ **Zero-crossing detection** for precise AC synchronization
- ✅ **Inductive load optimized** with proper snubber circuits
- ✅ **TRIAC-based switching** with optical isolation
- ✅ **Configurable AC frequency** (50Hz/60Hz)
- ✅ **Minimum speed enforcement** to prevent motor stalling
- ✅ **Surge protection** with MOV (Metal Oxide Varistor)
- ✅ **dV/dt protection** to prevent false TRIAC triggering
- ✅ **ESP-IDF framework** for professional development
- ✅ **Real-time control** using FreeRTOS and ESP timers

## Hardware Specifications

| Specification | Value |
|--------------|-------|
| Microcontroller | ESP32 (240 MHz dual-core) |
| AC Input | 110-240V AC, 50/60 Hz |
| Maximum Load | 400W (resistive), 200W (inductive) |
| TRIAC | BTA16-600B (16A, 600V) |
| Isolation | Dual MOC3021 optocouplers |
| Zero-Cross Detection | Bridge rectifier + optocoupler |
| Trigger Pulse Width | 50 µs (optimized for inductive loads) |
| Timing Resolution | 1 µs (ESP timer) |
| Control Range | 20-100% (minimum enforced for stability) |

## Quick Start

### Prerequisites

- ESP-IDF v5.0 or later
- ESP32 development board
- USB cable for programming
- **Completed and tested hardware circuit** (see [BUILD_GUIDE.md](docs/guides/BUILD_GUIDE.md))

### Build and Flash

```bash
# Clone the repository
git clone https://github.com/yourusername/esp32-ac-dimmer-fan-controller.git
cd esp32-ac-dimmer-fan-controller/firmware

# Set up ESP-IDF environment
. $HOME/esp/esp-idf/export.sh

# Build the project
idf.py build

# Flash to ESP32
idf.py -p /dev/ttyUSB0 flash

# Monitor output
idf.py -p /dev/ttyUSB0 monitor
```

## Pin Configuration

| Function | GPIO Pin | Direction | Description |
|----------|----------|-----------|-------------|
| Zero-Cross Detection | GPIO34 | Input | Receives zero-crossing pulses from optocoupler |
| TRIAC Trigger | GPIO25 | Output | Sends trigger pulses to TRIAC driver optocoupler |

**Note:** GPIO34 is input-only on ESP32. GPIO25 supports output.

## Project Structure

```
esp32-ac-dimmer-fan-controller/
├── firmware/              # ESP-IDF firmware source code
│   ├── main/
│   │   ├── ac_dimmer.c   # Main application code
│   │   └── CMakeLists.txt
│   ├── CMakeLists.txt    # Top-level CMake configuration
│   └── sdkconfig.defaults # Default SDK configuration
├── hardware/             # Hardware design files
│   ├── schematics/       # Circuit schematics (KiCad)
│   ├── pcb/              # PCB design files
│   └── bom/              # Bill of materials
│       └── bom.csv
├── docs/                 # Documentation
│   ├── SAFETY.md         # Critical safety information
│   ├── TROUBLESHOOTING.md # Common issues and solutions
│   ├── guides/
│   │   └── BUILD_GUIDE.md # Complete build instructions
│   └── images/           # Diagrams and photos
├── examples/             # Example projects
│   ├── basic/            # Basic speed control
│   └── advanced/         # Advanced features
├── tools/                # Utility scripts
├── README.md             # This file
├── LICENSE               # MIT License
├── CHANGELOG.md          # Version history
└── CONTRIBUTING.md       # Contribution guidelines
```

## How It Works

### Phase-Angle Control

The dimmer uses **phase-angle control** (also called leading-edge dimming):

1. **Zero-Crossing Detection**: Circuit detects when AC voltage crosses zero
2. **Phase Delay**: ESP32 waits for calculated delay (0-10ms for 50Hz)
3. **TRIAC Trigger**: ESP32 triggers TRIAC to conduct
4. **Power Delivery**: TRIAC conducts until next zero-crossing
5. **Repeat**: Process repeats every half-cycle (100 times/second at 50Hz)

**Speed Control:**
- **100% speed**: Short delay (~0.5ms) → TRIAC on almost entire half-cycle
- **50% speed**: Medium delay (~5ms) → TRIAC on for half the half-cycle
- **20% speed**: Long delay (~9ms) → TRIAC on for minimal time
- **0% speed**: No trigger → Fan off

### Circuit Overview

```
AC Mains → Fuse → MOV → TRIAC → Fan
              ↓
         Zero-Cross    ← Optocoupler ← ESP32 GPIO34
         Detector

         TRIAC Gate    ← Optocoupler ← ESP32 GPIO25

         Snubber Circuit (C + R) across TRIAC for inductive load protection
```

## Code Example

```c
// Initialize the dimmer
init_gpio();
init_timer();

// Set fan to 75% speed
set_fan_speed(75);

// Turn fan off
set_fan_speed(0);

// Minimum speed (20%) - enforced automatically
set_fan_speed(10);  // Will be clamped to 20%
```

## Key Features Explained

### Minimum Speed Enforcement

Fans cannot run reliably below ~20% speed. The firmware enforces this:

```c
#define MIN_SPEED_PERCENT 20
```

Any speed below 20% is automatically increased to 20%.

### Inductive Load Optimization

Fans are inductive loads requiring:
- **Longer trigger pulse** (50µs vs 10µs for lamps)
- **Snubber circuit** (47nF + 47Ω) for dV/dt protection
- **Gate capacitor** (10nF) to prevent false triggering

### High-Precision Timing

- CPU @ 240 MHz for maximum performance
- ESP timer with 1µs resolution
- FreeRTOS @ 1000 Hz tick rate
- IRAM-placed ISR for zero latency

## Configuration Options

Edit `firmware/main/ac_dimmer.c` to customize:

```c
// Change for 60Hz AC (US/Japan)
#define AC_FREQUENCY 60
#define AC_HALF_PERIOD 8333  // 1000000 / 60 / 2

// Adjust minimum speed
#define MIN_SPEED_PERCENT 15  // More aggressive minimum

// Adjust trigger pulse width
#define TRIAC_PULSE_WIDTH 100  // Longer pulse for difficult loads
```

## Safety Features

- ✅ **Optical isolation** between mains and ESP32
- ✅ **MOV surge protection**
- ✅ **Fuse protection**
- ✅ **Snubber circuits** for safe TRIAC switching
- ✅ **Minimum speed enforcement** prevents motor damage
- ✅ **Watchdog timers** for system reliability

## Common Applications

- Ceiling fan speed control
- Exhaust fan dimming
- Workshop ventilation control
- Temperature-controlled cooling
- Smart home fan integration

**NOT suitable for:**
- Electronic fans with built-in controllers
- Variable frequency drives (VFDs)
- Brushless DC fans
- Fans with electronic speed control

## Testing

**ALWAYS test with an incandescent lamp before connecting a fan!**

1. Build circuit on breadboard/PCB
2. Connect incandescent lamp (40-60W)
3. Power on and verify dimming works
4. Test all speed ranges
5. Only after successful lamp test, connect fan

See [BUILD_GUIDE.md](docs/guides/BUILD_GUIDE.md) for complete testing procedures.

## Troubleshooting

| Problem | See Section |
|---------|-------------|
| Zero-cross not detected | [TROUBLESHOOTING.md](docs/TROUBLESHOOTING.md#zero-cross-issues) |
| Fan hums or vibrates | [TROUBLESHOOTING.md](docs/TROUBLESHOOTING.md#humming) |
| TRIAC overheats | [TROUBLESHOOTING.md](docs/TROUBLESHOOTING.md#thermal-issues) |
| Erratic behavior | [TROUBLESHOOTING.md](docs/TROUBLESHOOTING.md#erratic-operation) |

## Documentation

- **[SAFETY.md](docs/SAFETY.md)** - **READ THIS FIRST** - Critical safety information
- **[BUILD_GUIDE.md](docs/guides/BUILD_GUIDE.md)** - Complete build instructions with schematics
- **[TROUBLESHOOTING.md](docs/TROUBLESHOOTING.md)** - Common problems and solutions

## Contributing

Contributions are welcome! Please read [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines.

**Important:** All contributions must maintain safety standards. Code changes affecting timing or power control require extra scrutiny.

## License

This project is licensed under the MIT License - see [LICENSE](LICENSE) file for details.

**Disclaimer:** This license applies to software only. Building this device is at your own risk. The authors assume no liability for damages, injury, or death resulting from construction or use of this device.

## Changelog

See [CHANGELOG.md](CHANGELOG.md) for version history.

## Support

- **Issues:** Report bugs via [GitHub Issues](https://github.com/yourusername/esp32-ac-dimmer-fan-controller/issues)
- **Discussions:** Ask questions in [GitHub Discussions](https://github.com/yourusername/esp32-ac-dimmer-fan-controller/discussions)

## Acknowledgments

- ESP-IDF framework by Espressif Systems
- Phase-control dimming techniques from power electronics literature
- Safety guidelines from IEC 60950-1 and electrical safety standards

---

**Remember: Safety First. If you don't understand the circuit, don't build it.**

**⚡ HIGH VOLTAGE - PROCEED WITH EXTREME CAUTION ⚡**
