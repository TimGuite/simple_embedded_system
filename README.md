# Simple Embedded System

[![ESP32-C3-DevKitC-02 Rust Bare Metal CI](https://github.com/TimGuite/simple_embedded_system/actions/workflows/esp32-c3-devkitc-02-rust-bm.yml/badge.svg)](https://github.com/TimGuite/simple_embedded_system/actions/workflows/esp32-c3-devkitc-02-rust-bm.yml)
[![STM32 Nucleo F446RE Zephyr CI Build](https://github.com/TimGuite/simple_embedded_system/actions/workflows/stm32_nucleo_f446re_zephyr.yml/badge.svg)](https://github.com/TimGuite/simple_embedded_system/actions/workflows/stm32_nucleo_f446re_zephyr.yml)

# A complete simple embedded system using as many tools as possible.

An embedded system is a combination of electronics and software designed for a specific purpose.
This describes almost every modern electronic device, so here are some examples of what people normally mean:

```
✅ A computer keyboard is an embedded system
✅ A pacemaker is an embedded system
✅ A satellite is made up of lots of embedded systems
⚠️ A mobile phone is on the edge
❌ A computer is not an embedded system
```

This project looks at different ways to program, test and simulate a **Simple Embedded System**, which has a few key elements:

- Measures **input** data from the real world - temperature, humidity and user input
- **Processes** the data - converting units, averaging, changing settings
- Provides an **output** - lights, displays, notifications

The initial aim is to measure temperature and humidity from a digital sensor, display the readings on a display, and use a button to switch between temperature, humidity and units.
The fun part is trying to do this using different hardware, languages and frameworks in a professional manner.
Therefore, each implementation of the **Simple Embedded System** must contain:

- Documentation for building and flashing the firmware
- Automated builds in CI
- Automated tests in CI, either unit testing or simulation

![Diagram of a simple embedded system shwoing sensors and inputs directed to an MCU, which is direct to an output, on top of build and test foundations](/assets/simple_embedded_system_diagram.png)

# Implementations

| Name  | Software | Hardware | Testing | Simulation |
| :-------------: | :-------------: | :-------------: | :-------------: | :-------------: |
| ESP32 Rust Baremetal  | Embedded Rust | ESP32C3 DevkitC | ✅ Wokwi CI | ✅ Wokwi CI |
| STM32 Zephyr Base  | Zephyr | Nucleo-F446RE  | ❌ | ❌ |
| MicroPython ESP32 | MicroPython | ESP32C3 DevkitC | ✅ Pytest | ✅ Wokwi |

# License

Licensed under GPLv3.

Commercial licenses available upon request.

# Contributing

Contributions welcome!
