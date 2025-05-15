/**
 * @file pattern_functions.c
 * @brief Contains updatePattern and showSelectedPattern functions
 */

#include "board.h"
#include "GPIO.h"
#include "new_patterns.h"
#include "common_functions.h"
#include "TM4C123GH6PM.h"

// Pattern types - add these at the top
#define PATTERN_PLANES_X     0  // Planes moving along X axis (left to right)
#define PATTERN_PLANES_Y     1  // Planes moving along Y axis (front to back)
#define PATTERN_PLANES_Z     2  // Planes moving along Z axis (bottom to top)
#define PATTERN_COUNTDOWN    3  // Countdown from 9 to 0

// Forward declarations for main.c functions
void updateXPlanes(uint8_t position, float brightness);
void updateYPlanes(uint8_t position, float brightness);
void updateZPlanes(uint8_t position, float brightness);
void updateCountdownPattern(uint8_t position, float brightness);

// Update the current pattern based on selection
void updatePattern(uint8_t pattern, uint8_t position, float brightness) {
    switch (pattern) {
        case PATTERN_PLANES_X:
            updateXPlanes(position, brightness);
            break;
        case PATTERN_PLANES_Y:
            updateYPlanes(position, brightness);
            break;
        case PATTERN_PLANES_Z:
            updateZPlanes(position, brightness);
            break;
        case PATTERN_COUNTDOWN:
            updateCountdownPattern(position, brightness);
            break;
        case PATTERN_RAIN:
            updateRainPattern(position, brightness);
            break;
        case PATTERN_SPHERE:
            updateSpherePattern(position, brightness);
            break;
        case PATTERN_SPIRAL:
            updateSpiralPattern(position, brightness);
            break;
        case PATTERN_FIREWORKS:
            updateFireworksPattern(position, brightness);
            break;
        case PATTERN_WAVE:
            updateWavePattern(position, brightness);
            break;
        case PATTERN_SPINNING_PLANE:
            updateSpinningPlanePattern(position, brightness);
            break;
        case PATTERN_RAIN_RGB:
            updateRainRGBPattern(position, brightness);
            break;
        case PATTERN_DNA:
            updateDNAPattern(position, brightness);
            break;
        case PATTERN_GAME_OF_LIFE_3D:
            updateGameOfLife3D(position, brightness);
            break;
        case PATTERN_CUBE_IN_CUBE:
            updateCubeInCubePattern(position, brightness);
            break;
        case PATTERN_SNAKE_3D:
            updateSnake3DPattern(position, brightness);
            break;
        case PATTERN_TEXT_SCROLLER:
            updateTextScrollerPattern(position, brightness);
            break;
        case PATTERN_PLASMA:
            updatePlasmaPattern(position, brightness);
            break;
        default:
            updateXPlanes(position, brightness);
            break;
    }
}

// Display the selected pattern on the onboard RGB LED
void showSelectedPattern(uint8_t pattern) {
    // Define the GPIO port F data register for direct access to the RGB LED
    #define GPIO_PORTF_DATA_R    (*((volatile unsigned long *)0x400253FC))
    
    // Clear any previous LED
    GPIO_PORTF_DATA_R &= ~0x0E;  // All RGB LEDs off
    
    // Show which pattern is active using different color combinations
    switch (pattern) {
        case PATTERN_PLANES_X:
            GPIO_PORTF_DATA_R |= 0x02;  // Red for X planes
            break;
        case PATTERN_PLANES_Y:
            GPIO_PORTF_DATA_R |= 0x08;  // Green for Y planes
            break;
        case PATTERN_PLANES_Z:
            GPIO_PORTF_DATA_R |= 0x04;  // Blue for Z planes
            break;
        case PATTERN_COUNTDOWN:
            GPIO_PORTF_DATA_R |= 0x0A;  // Yellow for Countdown
            break;
        case PATTERN_RAIN:
            GPIO_PORTF_DATA_R |= 0x0C;  // Cyan for Rain
            break;
        case PATTERN_SPHERE:
            GPIO_PORTF_DATA_R |= 0x06;  // Magenta for Sphere
            break;
        case PATTERN_SPIRAL:
            GPIO_PORTF_DATA_R |= 0x0E;  // White for Spiral
            break;
        case PATTERN_FIREWORKS:
            // Simple red for fireworks
            GPIO_PORTF_DATA_R |= 0x02;
            break;
        case PATTERN_WAVE:
            // Cyan for wave
            GPIO_PORTF_DATA_R |= 0x0C;
            break;
        case PATTERN_SPINNING_PLANE:
            // Blue for spinning plane
            GPIO_PORTF_DATA_R |= 0x04;
            break;
        case PATTERN_RAIN_RGB:
            // Magenta for RGB rain
            GPIO_PORTF_DATA_R |= 0x06;
            break;
        case PATTERN_DNA:
            // Yellow for DNA
            GPIO_PORTF_DATA_R |= 0x0A;
            break;
        case PATTERN_GAME_OF_LIFE_3D:
            // Green for Game of Life
            GPIO_PORTF_DATA_R |= 0x08;
            break;
        case PATTERN_CUBE_IN_CUBE:
            // White for cube in cube
            GPIO_PORTF_DATA_R |= 0x0E;
            break;
        case PATTERN_SNAKE_3D:
            // Green for snake
            GPIO_PORTF_DATA_R |= 0x08;
            break;
        case PATTERN_TEXT_SCROLLER:
            // Yellow for text
            GPIO_PORTF_DATA_R |= 0x0A;
            break;
        case PATTERN_PLASMA:
            // Magenta for plasma
            GPIO_PORTF_DATA_R |= 0x06;
            break;
    }
}