/**
 * @file common_functions.c
 * @brief Implementation of common functions used across multiple pattern files
 */

#include "common_functions.h"
#include "board.h"
#include "ADC.h"
#include <stdlib.h>

// External declarations for led_map
extern const uint16_t led_map[CUBE_SIZE][CUBE_SIZE][CUBE_SIZE];

// Global variables
uint8_t testBuffer[NUM_LEDS * 3] = {0};
uint8_t currentPattern = 0;
uint8_t currentPosition = 0;

// Known dead LEDs (0-based buffer indices)
static const uint16_t mainDeadLEDs[] = {0, 108, 156, 157, 206, 213, 221};
static const uint8_t mainNumDeadLEDs = 7;

// Function to check if an LED is in the dead LED list
static int mainIsDeadLED(uint16_t ledNum) {
    for (uint8_t i = 0; i < mainNumDeadLEDs; i++) {
        if (mainDeadLEDs[i] == ledNum) {
            return 1;
        }
    }
    return 0;
}

// Count dead LEDs before a given index
static int countDeadLEDsBefore(uint16_t ledIndex) {
    int count = 0;
    for (uint8_t i = 0; i < mainNumDeadLEDs; i++) {
        if (mainDeadLEDs[i] < ledIndex) {
            count++;
        }
    }
    return count;
}

// Read and update brightness from potentiometer
float updateBrightness(void) {
    // Read the ADC value from the potentiometer
    uint16_t rawValue = ADC0_ReadChannel(0);
    
    // Convert to a 0.0-1.0 range
    float globalBrightness = (float)rawValue / 4095.0f;
    
    // Apply a square curve to make brightness control feel more natural
    globalBrightness = globalBrightness * globalBrightness;
    
    // Limit maximum brightness to prevent power issues
    if (globalBrightness > 0.5f) globalBrightness = 0.5f;
    
    return globalBrightness;
}

// Set all LEDs to OFF
void clearAllLeds(void) {
    for (uint16_t i = 0; i < NUM_LEDS * 3; i++) {
        testBuffer[i] = 0;
    }
}

// Set a specific LED to a color - accounting for dead LED shifts
void setLedColor(uint16_t targetIndex, rgb_t color) {
    if (targetIndex >= NUM_LEDS) {
        return; // Out of range
    }
    
    // Skip if this LED is dead
    if (mainIsDeadLED(targetIndex)) {
        return;
    }
    
    // Calculate the actual buffer position accounting for dead LEDs
    int deadBefore = countDeadLEDsBefore(targetIndex);
    uint16_t actualBufferIdx = targetIndex - deadBefore;
    
    if (actualBufferIdx < NUM_LEDS) {
        testBuffer[actualBufferIdx * 3 + 0] = color.g;  // WS2812 order is GRB
        testBuffer[actualBufferIdx * 3 + 1] = color.r;
        testBuffer[actualBufferIdx * 3 + 2] = color.b;
    }
}

// Helper function to set a voxel by coordinates
void setVoxel(uint8_t x, uint8_t y, uint8_t z, rgb_t color) {
    if (x < CUBE_SIZE && y < CUBE_SIZE && z < CUBE_SIZE) {
        uint16_t led_idx = led_map[x][y][z];
        if (led_idx > 0 && led_idx <= NUM_LEDS) {
            setLedColor(led_idx - 1, color);  // Convert 1-based to 0-based index
        }
    }
}

// Apply brightness scaling to a color
rgb_t scaleBrightness(rgb_t color, float brightness) {
    rgb_t scaled;
    scaled.r = (uint8_t)((float)color.r * brightness);
    scaled.g = (uint8_t)((float)color.g * brightness);
    scaled.b = (uint8_t)((float)color.b * brightness);
    return scaled;
}