# Tiva_c_ws2812_LED_CUBE

## Introduction
This project implements an 7x7x7 LED cube using WS2812 individually addressable LEDs controlled by a Tiva C LaunchPad microcontroller. The system generates various 3D light animations and patterns, providing an interactive visual display through embedded systems programming.

## Results and Video Demonstration Links
- [Demo Video 1: Basic Animations](https://youtube.com/shorts/4x9WRMf69NA?si=lCLfO1PvJxkT0AR9)


## Background and Methodology

### Embedded Systems Concepts Applied
- GPIO configuration and control for peripheral interfacing.
- SPI communication protocol to emulate the WS2812 signal timing.
- Real-time pattern rendering using lookup tables and multiplexing.
- Low-level bit manipulation for efficient LED control.

### Achieving Project Goals
- Developed custom drivers for WS2812 LEDs using SPI at 2.4 MHz to meet timing requirements.
- Designed a flexible framework to define and render custom LED animations.

## Functional Block Diagram
![led cube drawio](https://github.com/user-attachments/assets/431955e1-1a02-463c-a326-4bfb2a4c1693)

## Table of Components Used

| Component           | Quantity | Description                                    |
|---------------------|----------|------------------------------------------------|
| Tiva C LaunchPad    | 1        | TM4C123GXL microcontroller development board   |
| WS2812 LEDs         | 343      | Individually addressable RGB LEDs (7x7x7 cube) |
| Level Shifter       | 1        | 3.3V to 5V signal level conversion             |
| Power Supply        | 1        | 5V 30A power supply for LED cube               |


## Table of Pinout Used

| Tiva C Pin  | Function                  | Purpose                           |
|-------------|---------------------------|----------------------------------------|
| PA5         | SPI for WS2812 signal      | Data In to first LED                   |
| PF1         | Onboard Debug LED          | For debug indication                   |
| PF4         | SW1                        | Cycle through patterns                 |
| PF0         | SW2                        | Cycle Within patterns                  |
| PE3         | Pot input                  | Adjust brightness                      |

