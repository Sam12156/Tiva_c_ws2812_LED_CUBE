/**
 * @file updated_main_function.c
 * @brief Updated main function that includes all new patterns
 * This should replace the existing main() function in main.c
 */
#include "board.h"
#include "GPIO.h"
#include "SysTick_Delay.h"
#include "WS2812.h"
#include "TM4C123GH6PM.h"
#include "ADC.h"
#include "led_cube.h"
#include "new_patterns.h"
#include "common_functions.h"
#include <stddef.h>
#include <stdlib.h>
#include <math.h>
#include "pattern_functions.h"

/**
 * @file corrected_patterns.c
 * @brief Pattern functions with correctly oriented patterns for your cube
 * 
 * Based on your latest description:
 * - Blue pattern: Moves bottom to top (Z-axis)
 * - Green pattern: Moves front to back (Y-axis)
 * - Red pattern: Moves left to right (X-axis)
 * - Countdown: Digits need to be sideways with bottom of number at right side
 */

// X Planes pattern - planes moving left to right
// RED planes in your orientation
void updateXPlanes(uint8_t position, float brightness) {
    clearAllLeds();
    
    // Current x plane (0-6)
    uint8_t x = position % CUBE_SIZE;
    
    // Color for this pattern (red)
    rgb_t planeColor = {40, 0, 0};
    
    // Scale the color based on current brightness
    planeColor = scaleBrightness(planeColor, brightness);
    
    // Set all LEDs in this X plane
    for (uint8_t y = 0; y < CUBE_SIZE; y++) {
        for (uint8_t z = 0; z < CUBE_SIZE; z++) {
            setVoxel(x, y, z, planeColor);
        }
    }
}

// Y Planes pattern - planes moving front to back
// GREEN planes in your orientation
void updateYPlanes(uint8_t position, float brightness) {
    clearAllLeds();
    
    // Current y plane (0-6)
    uint8_t y = position % CUBE_SIZE;
    
    // Color for this pattern (green)
    rgb_t planeColor = {0, 40, 0};
    
    // Scale the color based on current brightness
    planeColor = scaleBrightness(planeColor, brightness);
    
    // Set all LEDs in this Y plane
    for (uint8_t x = 0; x < CUBE_SIZE; x++) {
        for (uint8_t z = 0; z < CUBE_SIZE; z++) {
            setVoxel(x, y, z, planeColor);
        }
    }
}

// Z Planes pattern - planes moving bottom to top
// BLUE planes in your orientation
void updateZPlanes(uint8_t position, float brightness) {
    clearAllLeds();
    
    // Current z plane (0-6)
    uint8_t z = position % CUBE_SIZE;
    
    // Color for this pattern (blue)
    rgb_t planeColor = {0, 0, 40};
    
    // Scale the color based on current brightness
    planeColor = scaleBrightness(planeColor, brightness);
    
    // Set all LEDs in this Z plane
    for (uint8_t x = 0; x < CUBE_SIZE; x++) {
        for (uint8_t y = 0; y < CUBE_SIZE; y++) {
            setVoxel(x, y, z, planeColor);
        }
    }
}



/**
 * @file final_coordinate_fix.c
 * @brief Let's directly adjust the coordinates to the exact position needed
 */

/**
 * @file positioned_countdown.c
 * @brief Countdown with digits positioned at X=1, Z=0 (bottom)
 */

/**
 * @file exact_position_countdown.c
 * @brief Countdown with digits precisely at X=1, Z=0
 */

/**
 * @file rotated_countdown.c
 * @brief Countdown with digits at X=1, Z=0, properly rotated
 */

/**
 * @file full_grid_numbers.c
 * @brief Countdown using full 7x7 grid digits
 */

// Global variables for the countdown pattern
static uint8_t countdownValue = 9;
static uint8_t countdownChanged = 0;  // Flag to detect button presses
static uint32_t countdownTimer = 0;   // Timer for countdown
 
// Helper function to draw a digit in the cube
// Using full 7x7 grid with extra space filled with 0s
void drawDigit(uint8_t digit, rgb_t color) {

// Define numbers using full 7x7 grid (all 49 positions defined)
static const uint8_t digitPatterns[10][7][7] = {
    { // 0
        {0, 0, 0, 0, 0, 0, 0},
        {0, 1, 1, 1, 1, 1, 0},
        {1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1},
        {0, 1, 1, 1, 1, 1, 0},
        {0, 0, 0, 0, 0, 0, 0}
    },
    { // 1
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 1, 0},
        {1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0}
    },
    { // 2
        {0, 0, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 1, 0},
        {1, 1, 0, 0, 0, 0, 1},
        {1, 0, 1, 0, 0, 0, 1},
        {1, 0, 0, 1, 0, 0, 1},
        {1, 0, 0, 0, 1, 1, 0},
        {0, 0, 0, 0, 0, 0, 0}
    },
    { // 3
        {0, 0, 0, 0, 0, 0, 0},
        {0, 1, 0, 0, 0, 1, 0},
        {1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 1, 0, 0, 1},
        {1, 0, 0, 1, 0, 0, 1},
        {0, 1, 1, 0, 1, 1, 0},
        {0, 0, 0, 0, 0, 0, 0}
    },
    { // 4
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 1, 1, 0, 0, 0},
        {0, 0, 1, 0, 1, 0, 0},
        {0, 0, 1, 0, 0, 1, 0},
        {1, 1, 1, 1, 1, 1, 1},
        {0, 0, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0}
    },
    { // 5
        {0, 0, 0, 0, 0, 0, 0},
        {0, 1, 0, 1, 1, 1, 1},
        {1, 0, 0, 1, 0, 0, 1},
        {1, 0, 0, 1, 0, 0, 1},
        {1, 0, 0, 1, 0, 0, 1},
        {0, 1, 1, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 0}
    },
    { // 6
        {0, 0, 0, 0, 0, 0, 0},
        {0, 1, 1, 1, 1, 0, 0},
        {1, 0, 0, 1, 0, 1, 0},
        {1, 0, 0, 1, 0, 0, 1},
        {1, 0, 0, 1, 0, 0, 1},
        {0, 1, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0}
    },
    { // 7
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 0, 0, 0, 1},
        {0, 0, 0, 1, 0, 0, 1},
        {0, 0, 0, 0, 1, 0, 1},
        {0, 0, 0, 0, 0, 1, 1},
        {0, 0, 0, 0, 0, 0, 0}
    },
    { // 8
        {0, 0, 0, 0, 0, 0, 0},
        {0, 1, 1, 0, 1, 1, 0},
        {1, 0, 0, 1, 0, 0, 1},
        {1, 0, 0, 1, 0, 0, 1},
        {1, 0, 0, 1, 0, 0, 1},
        {0, 1, 1, 0, 1, 1, 0},
        {0, 0, 0, 0, 0, 0, 0}
    },
    { // 9
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 1, 0},
        {1, 0, 0, 1, 0, 0, 1},
        {1, 0, 0, 1, 0, 0, 1},
        {0, 1, 0, 1, 0, 0, 1},
        {0, 0, 1, 1, 1, 1, 0},
        {0, 0, 0, 0, 0, 0, 0}
    }
};
    
    // Starting position and thickness
    uint8_t x_start = 0;         // Start at X=0
    uint8_t y_start = 0;         // Start at front of cube (Y=0)
    uint8_t y_thickness = 7;     // Thickness of 4 layers
    uint8_t z_start = 0;         // Start at bottom (Z=0)
    
    // Draw the current digit pattern
    for (uint8_t row = 0; row < 7; row++) {
        for (uint8_t col = 0; col < 7; col++) {
            // Check if this position is lit (1) or not (0)
            if (digitPatterns[digit][row][col] == 1) {
                // Calculate actual position
                uint8_t x = x_start + col;  // X position
                uint8_t z = z_start + row;  // Z position
                
                // Draw voxel across multiple Y planes for thickness
                for (uint8_t y = y_start; y < y_start + y_thickness && y < CUBE_SIZE; y++) {
                    setVoxel(x, y, z, color);
                }
            }
        }
    }
}

// Countdown pattern - displays numbers counting down
void updateCountdownPattern(uint8_t position, float brightness) {
    // Check if button 2 is pressed and wasn't pressed before
    if (GPIO_Button2Pressed() && !countdownChanged) {
        // Button just pressed - decrement the value
        if (countdownValue > 0) {
            countdownValue--;
        } else {
            countdownValue = 9; // Reset to 9 after reaching 0
        }
        countdownChanged = 1;  // Set flag to prevent multiple decrements per press
    }
    else if (!GPIO_Button2Pressed()) {
        // Button released - clear the flag
        countdownChanged = 0;
    }
    
    // Count frames for exactly 1 second timing
    countdownTimer++;
    if (countdownTimer >= 100) {  // 100 * 10ms = 1 second precisely
        countdownTimer = 0;
        if (countdownValue > 0) {
            countdownValue--;
        } else {
            countdownValue = 9; // Reset to 9 after reaching 0
        }
    }
    
    // Clear all LEDs
    clearAllLeds();
    
    // Calculate color that changes with each number
    float hue = countdownValue * 40.0f;
    uint8_t r = (uint8_t)((sinf(hue * 0.0174f) + 1.0f) * 20.0f);
    uint8_t g = (uint8_t)((sinf((hue + 120.0f) * 0.0174f) + 1.0f) * 20.0f);
    uint8_t b = (uint8_t)((sinf((hue + 240.0f) * 0.0174f) + 1.0f) * 20.0f);
    
    // Scale the brightness
    rgb_t digitColor = {r, g, b};
    digitColor = scaleBrightness(digitColor, brightness);
    
    // Draw the current number
    drawDigit(countdownValue, digitColor);
}



















// Rain pattern state
#define MAX_DROPS 15
typedef struct {
    uint8_t x;
    uint8_t y;
    uint8_t z;
    uint8_t active;
} raindrop_t;
static raindrop_t raindrops[MAX_DROPS];

// Rain pattern - drops falling from top to bottom
void updateRainPattern(uint8_t position, float brightness) {
    clearAllLeds();
    
    // Create new drops randomly
    if (position % 5 == 0) {
        for (int i = 0; i < MAX_DROPS; i++) {
            if (!raindrops[i].active) {
                // 30% chance to create a new drop
                if ((rand() % 10) < 3) {
                    raindrops[i].x = rand() % CUBE_SIZE;
                    raindrops[i].y = rand() % CUBE_SIZE;
                    raindrops[i].z = CUBE_SIZE - 1; // Start at the top
                    raindrops[i].active = 1;
                    break; // Only create one new drop per iteration
                }
            }
        }
    }
    
    // Move all active drops down
    for (int i = 0; i < MAX_DROPS; i++) {
        if (raindrops[i].active) {
            // If at bottom, deactivate
            if (raindrops[i].z == 0) {
                raindrops[i].active = 0;
            } else {
                // Move down
                raindrops[i].z--;
            }
            
            // Draw the active drops with blue color
            if (raindrops[i].active) {
                rgb_t dropColor;
                
                // Bright blue at the front of the drop
                dropColor.r = 0;
                dropColor.g = 0;
                dropColor.b = 40;
                dropColor = scaleBrightness(dropColor, brightness * 1.2f);
                setVoxel(raindrops[i].x, raindrops[i].y, raindrops[i].z, dropColor);
                
                // Dimmer blue behind (trail)
                if (raindrops[i].z < CUBE_SIZE - 1) {
                    dropColor.r = 0;
                    dropColor.g = 0;
                    dropColor.b = 20;
                    dropColor = scaleBrightness(dropColor, brightness * 0.6f);
                    setVoxel(raindrops[i].x, raindrops[i].y, raindrops[i].z + 1, dropColor);
                }
            }
        }
    }
}

// Sphere pattern - expanding and contracting sphere
static uint8_t sphereRadius = 0;
static uint8_t sphereExpanding = 1;

// Helper function to determine if a point is on a sphere
int isPointOnSphere(int x, int y, int z, int centerX, int centerY, int centerZ, int radius, int thickness) {
    // Calculate distance from center (squared to avoid sqrt)
    int dx = x - centerX;
    int dy = y - centerY;
    int dz = z - centerZ;
    int distSquared = dx*dx + dy*dy + dz*dz;
    
    // Check if point is within the sphere's shell
    int minRadiusSquared = (radius - thickness) * (radius - thickness);
    int maxRadiusSquared = radius * radius;
    
    return (distSquared <= maxRadiusSquared && distSquared >= minRadiusSquared);
}

void updateSpherePattern(uint8_t position, float brightness) {
    clearAllLeds();
    
    // Update sphere radius every few frames
    if (position % 3 == 0) {
        if (sphereExpanding) {
            sphereRadius++;
            if (sphereRadius >= CUBE_SIZE) {
                sphereRadius = CUBE_SIZE - 1;
                sphereExpanding = 0;
            }
        } else {
            if (sphereRadius > 0) {
                sphereRadius--;
            } else {
                sphereExpanding = 1;
            }
        }
    }
    
    // Center of the cube
    int centerX = CUBE_SIZE / 2;
    int centerY = CUBE_SIZE / 2;
    int centerZ = CUBE_SIZE / 2;
    
    // Calculate color based on radius
    rgb_t sphereColor;
    
    // Change color based on expansion/contraction
    if (sphereExpanding) {
        float ratio = (float)sphereRadius / (CUBE_SIZE - 1);
        sphereColor.r = (uint8_t)(40.0f * (1.0f - ratio));
        sphereColor.g = 0;
        sphereColor.b = (uint8_t)(40.0f * ratio);
    } else {
        float ratio = (float)sphereRadius / (CUBE_SIZE - 1);
        sphereColor.r = (uint8_t)(40.0f * ratio);
        sphereColor.g = (uint8_t)(40.0f * (1.0f - ratio));
        sphereColor.b = 0;
    }
    
    // Apply brightness scaling
    sphereColor = scaleBrightness(sphereColor, brightness);
    
    // Draw the sphere
    for (uint8_t x = 0; x < CUBE_SIZE; x++) {
        for (uint8_t y = 0; y < CUBE_SIZE; y++) {
            for (uint8_t z = 0; z < CUBE_SIZE; z++) {
                if (isPointOnSphere(x, y, z, centerX, centerY, centerZ, sphereRadius, 1)) {
                    setVoxel(x, y, z, sphereColor);
                }
            }
        }
    }
}

// Spiral pattern state
static uint8_t spiralAngle = 0;
static uint8_t spiralHeight = 0;

// Spiral pattern - rotating spiral that moves up and down
void updateSpiralPattern(uint8_t position, float brightness) {
    clearAllLeds();
    
    // Update spiral parameters
    spiralAngle = (spiralAngle + 1) % 36; // 10 degree increments (360/36 = 10)
    
    // Move the spiral up and down
    if (position % 10 == 0) {
        spiralHeight = (spiralHeight + 1) % (CUBE_SIZE * 2);
    }
    
    // Map height to a sine wave to get smooth up/down motion
    float height = CUBE_SIZE/2.0f + (CUBE_SIZE/2.0f - 0.5f) * sinf(spiralHeight * 0.1f * 3.14159f);
    
    // Center of spiral (X,Y)
    float centerX = (CUBE_SIZE - 1) / 2.0f;
    float centerY = (CUBE_SIZE - 1) / 2.0f;
    
    // Draw the spiral
    for (int i = 0; i < 36; i++) {
        float angle = (i * 10 + spiralAngle) * 0.0174533f; // Convert to radians
        
        // Spiral radius increases with angle
        float radius = (float)i / 36.0f * (CUBE_SIZE / 2.0f - 0.5f);
        
        // Calculate X,Y position
        int x = (int)(centerX + radius * cosf(angle));
        int y = (int)(centerY + radius * sinf(angle));
        int z = (int)height;
        
        // Check bounds
        if (x >= 0 && x < CUBE_SIZE && y >= 0 && y < CUBE_SIZE && z >= 0 && z < CUBE_SIZE) {
            // Calculate color based on position in spiral
            rgb_t spiralColor;
            float colorScale = (float)i / 36.0f;
            spiralColor.r = (uint8_t)(40.0f * (1.0f - colorScale));
            spiralColor.g = (uint8_t)(40.0f * colorScale);
            spiralColor.b = (uint8_t)(20.0f);
            
            // Apply brightness
            spiralColor = scaleBrightness(spiralColor, brightness);
            
            // Set the voxel
            setVoxel(x, y, z, spiralColor);
        }
    }
}
int main(void) {
    // Initialize system
    Board_Init();
    
    // Initialize ADC for potentiometer
    ADC_Init();
    
    // Initialize LED cube
    Cube_Init();
    
    // Initialize patterns that need setup
    initRainPattern();
    initRainRGBPattern();
    initFireworksPattern();
    
    // Main loop variables
    int updateStatus = STATUS_OK;
    uint32_t positionChangeTime = 0;
    uint32_t patternChangeTime = 0;
    uint8_t buttonState = 0;
    float brightness = 0.5f;
    
    // Updated number of patterns
    #define PATTERN_COUNT 17
    
    // Create a brief flash to indicate system start
    clearAllLeds();
    for (int i = 0; i < CUBE_SIZE; i++) {
        for (int j = 0; j < CUBE_SIZE; j++) {
            setVoxel(i, j, 3, (rgb_t){20, 20, 20}); // White flash at middle layer
        }
    }
    WS2812_Show(testBuffer, NUM_LEDS);
    SysTick_Delay(500);
    
    while (1) {
        // Update brightness from potentiometer
        brightness = updateBrightness();
        
        // Handle button 1 (SW1) to change pattern
        if (GPIO_Button1Pressed()) {
            if (buttonState == 0 || buttonState == 2) {
                // Button just pressed
                buttonState = 1;
                currentPattern = (currentPattern + 1) % PATTERN_COUNT;
                currentPosition = 0;  // Reset position when changing patterns
                
                // Handle pattern-specific initialization when changing patterns
								switch (currentPattern) {
										case PATTERN_GAME_OF_LIFE_3D:
												resetGameOfLife3D(); // Function call instead of direct variable access
												break;
										case PATTERN_SNAKE_3D:
												resetSnake3D(); // Function call instead of direct variable access
												break;
								}
							}
        } else if (GPIO_Button2Pressed()) {
            if (buttonState == 0 || buttonState == 1) {
                // Button just pressed
                buttonState = 2;
                // Manually advance position
                currentPosition = (currentPosition + 1) % (CUBE_SIZE * 2);
            }
        } else {
            // Both buttons released
            buttonState = 0;
        }
        
        // Auto change position every 1 second
        if (positionChangeTime >= 100) {  // 100 * 10ms = 1 second
            positionChangeTime = 0;
            currentPosition = (currentPosition + 1) % (CUBE_SIZE * 10); // Extended range for more animation frames
        } else {
            positionChangeTime++;
        }
        
        // Auto change pattern every 15 seconds
        if (patternChangeTime >= 1500) {  // 1500 * 10ms = 15 seconds
            patternChangeTime = 0;
            currentPattern = (currentPattern + 1) % PATTERN_COUNT;
            currentPosition = 0;  // Reset position when changing patterns
            
            // Handle pattern-specific initialization when auto-changing patterns
            switch (currentPattern) {
										case PATTERN_GAME_OF_LIFE_3D:
												resetGameOfLife3D(); // Function call instead of direct variable access
												break;
										case PATTERN_SNAKE_3D:
												resetSnake3D(); // Function call instead of direct variable access
												break;
								}
        } else {
            patternChangeTime++;
        }
        
        // Update the current pattern
        updatePattern(currentPattern, currentPosition, brightness);
        
        // Show selected pattern on board LED
        showSelectedPattern(currentPattern);
        
        // Send data to LEDs
        updateStatus = WS2812_Show(testBuffer, NUM_LEDS);
        
        // If update failed, wait a bit before trying again
        if (updateStatus == STATUS_ERROR) {
            SysTick_Delay(500);
            clearAllLeds();
        }
        
        // Delay to control animation speed
        SysTick_Delay(10);
    }
}
