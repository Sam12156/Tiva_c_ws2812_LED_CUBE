/**
 * @file new_patterns.h
 * @brief Header for all new animation patterns for 7x7x7 LED cube.
 */
#ifndef NEW_PATTERNS_H
#define NEW_PATTERNS_H

#include <stdint.h>
#include "led_cube.h"
#include "pattern_functions.h"  // To get the basic pattern definitions

// Pattern indices (adding to existing patterns in pattern_functions.h)
#define PATTERN_RAIN         4   // Falling rain drops animation
#define PATTERN_SPHERE       5   // Expanding & contracting sphere
#define PATTERN_SPIRAL       6   // Spiral animation
#define PATTERN_FIREWORKS    7   // Fireworks exploding
#define PATTERN_WAVE         8   // Sine wave animation
#define PATTERN_SPINNING_PLANE 9 // Plane that rotates around Z axis
#define PATTERN_RAIN_RGB     10  // RGB rain animation
#define PATTERN_DNA          11  // DNA double helix animation
#define PATTERN_GAME_OF_LIFE_3D 12 // 3D version of Conway's Game of Life
#define PATTERN_CUBE_IN_CUBE 13  // Cube within cube animation
#define PATTERN_SNAKE_3D     14  // 3D snake that moves around the cube
#define PATTERN_TEXT_SCROLLER 15 // Scrolling text animation
#define PATTERN_PLASMA       16  // 3D plasma animation

// Pattern 4: Rain
void initRainPattern(void);
void updateRainPattern(uint8_t position, float brightness);

// Pattern 5: Sphere
void updateSpherePattern(uint8_t position, float brightness);

// Pattern 6: Spiral
void updateSpiralPattern(uint8_t position, float brightness);

// Pattern 7: Fireworks
void initFireworksPattern(void);
void updateFireworksPattern(uint8_t position, float brightness);

// Pattern 8: Wave
void updateWavePattern(uint8_t position, float brightness);

// Pattern 9: Spinning Plane
void updateSpinningPlanePattern(uint8_t position, float brightness);

// Pattern 10: RGB Rain
void initRainRGBPattern(void);
void updateRainRGBPattern(uint8_t position, float brightness);

// Pattern 11: DNA Double Helix
void updateDNAPattern(uint8_t position, float brightness);

// Pattern 12: 3D Game of Life
void initGameOfLife3D(void);
void updateGameOfLife3D(uint8_t position, float brightness);
uint8_t countNeighbors3D(uint8_t x, uint8_t y, uint8_t z);
void resetGameOfLife3D(void);  // Add this function

// Pattern 13: Cube in Cube
void updateCubeInCubePattern(uint8_t position, float brightness);
void drawLine3D(float start[3], float end[3], rgb_t color);

// Pattern 14: 3D Snake
void initSnake3D(void);
void updateSnake3DPattern(uint8_t position, float brightness);
void placeFood(void);
void resetSnake3D(void);  // Add this function

// Pattern 15: Text Scroller
void updateTextScrollerPattern(uint8_t position, float brightness);

// Pattern 16: 3D Plasma
void updatePlasmaPattern(uint8_t position, float brightness);

#endif // NEW_PATTERNS_H