/**
 * @file common_functions.h
 * @brief Common functions and variables used across multiple pattern files
 */
#ifndef COMMON_FUNCTIONS_H
#define COMMON_FUNCTIONS_H

#include "led_cube.h"
#include <stdint.h>

// Status codes
#define STATUS_OK       1
#define STATUS_ERROR    0

// External buffer for LED control
extern uint8_t testBuffer[]; 

// Pattern state variables
extern uint8_t currentPattern;
extern uint8_t currentPosition;

// Helper functions
void clearAllLeds(void);
void setVoxel(uint8_t x, uint8_t y, uint8_t z, rgb_t color);
rgb_t scaleBrightness(rgb_t color, float brightness);
float updateBrightness(void);
void setLedColor(uint16_t targetIndex, rgb_t color);

#endif // COMMON_FUNCTIONS_H