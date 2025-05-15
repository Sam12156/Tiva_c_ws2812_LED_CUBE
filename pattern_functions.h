/**
 * @file pattern_functions.h
 * @brief Declarations for main pattern selection functions
 */
#ifndef PATTERN_FUNCTIONS_H
#define PATTERN_FUNCTIONS_H

#include <stdint.h>
#include "led_cube.h"

// Pattern types - basic patterns
#define PATTERN_PLANES_X     0  // Planes moving along X axis (left to right)
#define PATTERN_PLANES_Y     1  // Planes moving along Y axis (front to back)
#define PATTERN_PLANES_Z     2  // Planes moving along Z axis (bottom to top)
#define PATTERN_COUNTDOWN    3  // Countdown from 9 to 0

// Pattern functions
void updatePattern(uint8_t pattern, uint8_t position, float brightness);
void showSelectedPattern(uint8_t pattern);

// Basic plane patterns (implemented in main.c)
void updateXPlanes(uint8_t position, float brightness);
void updateYPlanes(uint8_t position, float brightness);
void updateZPlanes(uint8_t position, float brightness);
void updateCountdownPattern(uint8_t position, float brightness);

// Functions to reset complex pattern states
void resetGameOfLife3D(void);
void resetSnake3D(void);

#endif // PATTERN_FUNCTIONS_H