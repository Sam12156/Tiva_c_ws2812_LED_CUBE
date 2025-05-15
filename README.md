# Tiva_c_ws2812_LED_CUBE

## Introduction
This project implements an 8x8x8 LED cube using WS2812 individually addressable LEDs controlled by a Tiva C LaunchPad microcontroller. The system generates various 3D light animations and patterns, providing an interactive visual display through embedded systems programming.

## Results and Video Demonstration Links
- [Demo Video 1: Basic Animations](#) <!-- Replace with actual link -->
- [Demo Video 2: Advanced Patterns](#) <!-- Replace with actual link -->

## Background and Methodology

### Embedded Systems Concepts Applied
- GPIO configuration and control for peripheral interfacing.
- Timer-based microsecond and millisecond delay generation.
- SPI communication protocol to emulate the WS2812 signal timing.
- Modular code structure for pattern generation and cube control.
- Real-time pattern rendering using lookup tables and multiplexing.
- Low-level bit manipulation for efficient LED control.

### Achieving Project Goals
- Developed custom drivers for WS2812 LEDs using SPI at 2.4 MHz to meet timing requirements.
- Designed a flexible framework to define and render custom LED animations.
- Utilized Tiva C timers to replace SysTick for precise delay control under PLL operation.
- Verified axis orientation and cube addressing through test patterns.
- Managed power and signal integrity using appropriate level shifters and power design considerations.

## Functional Block Diagram
![Functional Block Diagram](#) <!-- Replace with actual image link -->

## Table of Components Used

| Component           | Quantity | Description                                    |
|---------------------|----------|------------------------------------------------|
| Tiva C LaunchPad    | 1        | TM4C123GXL microcontroller development board   |
| WS2812 LEDs         | 512      | Individually addressable RGB LEDs (8x8x8 cube) |
| Level Shifter       | 1        | 3.3V to 5V signal level conversion             |
| Power Supply        | 1        | 5V 20A power supply for LED cube               |
| Custom PCB / Wiring | 1        | For LED cube interconnections and power rails  |

## Table of Pinout Used

| Tiva C Pin  | Function                  | Connection                            |
|-------------|---------------------------|----------------------------------------|
| PA2 (SSI0Clk) | SPI Clock for WS2812 signal | Data In to first LED strip              |
| PF1         | Onboard Debug LED (Optional) | For debug indication                   |
| Timer 0     | Microsecond Delay          | WS2812 Reset and timing generation     |
| Timer 1     | Millisecond Delay          | General purpose delays                 |
| VBUS        | 5V Power Input             | Powering cube control logic            |
| GND         | Ground                     | Common ground for power and logic      |
