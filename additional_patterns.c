/**
 * @file additional_patterns.c
 * @brief Additional patterns for the 7x7x7 LED cube.
 * 
 * These patterns can be added to your existing code by:
 * 1. Adding the function prototypes to an appropriate header file
 * 2. Increasing PATTERN_COUNT in main.c
 * 3. Adding the patterns to the updatePattern() function switch case
 */

#include "board.h"
#include "led_cube.h"
#include "new_patterns.h"
#include "common_functions.h"
#include <stdlib.h>
#include <math.h>

// Pattern types (these would be added to your existing patterns in main.c)
#define PATTERN_FIREWORKS      7   // Fireworks exploding
#define PATTERN_WAVE           8   // Sine wave animation
#define PATTERN_SPINNING_PLANE 9   // Plane that rotates around Z axis
#define PATTERN_RAIN_RGB       10  // RGB rain animation


#define MAX_DROPS 15
typedef struct {
    uint8_t x;
    uint8_t y;
    uint8_t z;
    uint8_t active;
} raindrop_t;


// Declare this as global in additional_patterns.c
static raindrop_t raindrops[MAX_DROPS];

// Implementation of initRainPattern
void initRainPattern(void) {
    // Initialize the raindrops array
    for (int i = 0; i < MAX_DROPS; i++) {
        // Initially all drops are inactive
        raindrops[i].active = 0;
    }
}

// RGB rain pattern state
typedef struct {
    uint8_t x;
    uint8_t y;
    uint8_t z;
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t active;
} rgbdrop_t;

#define MAX_RGB_DROPS 20
static rgbdrop_t rgbDrops[MAX_RGB_DROPS];

// Initialize RGB rain pattern
void initRainRGBPattern(void) {
    for (int i = 0; i < MAX_RGB_DROPS; i++) {
        // Initially all drops are inactive
        rgbDrops[i].active = 0;
    }
}

// Fireworks pattern state
typedef struct {
    float x, y, z;         // Current position
    float vx, vy, vz;      // Velocity
    uint8_t r, g, b;       // Color
    uint8_t age;           // Age counter
    uint8_t active;        // Is this particle active?
} particle_t;

#define MAX_PARTICLES 50
static particle_t particles[MAX_PARTICLES];
static uint8_t fireworksTimer = 0;

// Initialize fireworks pattern
void initFireworksPattern(void) {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        particles[i].active = 0;
    }
    fireworksTimer = 0;
}



// Update fireworks pattern
void updateFireworksPattern(uint8_t position, float brightness) {
    clearAllLeds();
    
    // Create new firework
    fireworksTimer++;
    if (fireworksTimer >= 20) {
        fireworksTimer = 0;
        
        // Start position at the bottom of the cube
        float startX = 3.0f + (rand() % 100) / 100.0f - 0.5f;  // Center X with slight randomness
        float startY = 3.0f + (rand() % 100) / 100.0f - 0.5f;  // Center Y with slight randomness
        float startZ = 0.0f;
        
        // Random color for this firework
        uint8_t r = rand() % 40 + 10;
        uint8_t g = rand() % 40 + 10;
        uint8_t b = rand() % 40 + 10;
        
        // Create explosion at random height
        float targetZ = 2.0f + (rand() % 400) / 100.0f;  // Random height between 2 and 6
        
        // Launch particles upward from the starting point
        for (int i = 0; i < MAX_PARTICLES; i++) {
            if (!particles[i].active) {
                particles[i].x = startX;
                particles[i].y = startY;
                particles[i].z = startZ;
                particles[i].vz = 0.2f + (rand() % 10) / 100.0f;
                particles[i].vx = (rand() % 100) / 500.0f - 0.1f; // Small random X velocity
                particles[i].vy = (rand() % 100) / 500.0f - 0.1f; // Small random Y velocity
                particles[i].r = r;
                particles[i].g = g;
                particles[i].b = b;
                particles[i].age = 0;
                particles[i].active = 1;
                break;
            }
        }
    }
    
    // Update all active particles
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (particles[i].active) {
            // Update position
            particles[i].x += particles[i].vx;
            particles[i].y += particles[i].vy;
            particles[i].z += particles[i].vz;
            
            // Apply gravity
            particles[i].vz -= 0.05f;
            
            // Update age
            particles[i].age++;
            
            // Check if particle reached explode height
            if (particles[i].z >= 4.0f && particles[i].vz > 0) {
                // Explode!
                particles[i].active = 0;
                
                // Create explosion particles
                for (int j = 0; j < 15; j++) {
                    for (int k = 0; k < MAX_PARTICLES; k++) {
                        if (!particles[k].active) {
                            particles[k].x = particles[i].x;
                            particles[k].y = particles[i].y;
                            particles[k].z = particles[i].z;
                            
                            // Random velocity in all directions
                            float angle = (rand() % 628) / 100.0f; // 0 to 2π
                            float elevation = (rand() % 314) / 100.0f - 1.57f; // -π/2 to π/2
                            float speed = 0.1f + (rand() % 15) / 100.0f;
                            
                            particles[k].vx = speed * cosf(elevation) * cosf(angle);
                            particles[k].vy = speed * cosf(elevation) * sinf(angle);
                            particles[k].vz = speed * sinf(elevation);
                            
                            particles[k].r = particles[i].r;
                            particles[k].g = particles[i].g;
                            particles[k].b = particles[i].b;
                            particles[k].age = 0;
                            particles[k].active = 1;
                            break;
                        }
                    }
                }
            }
            
            // Check boundaries
            if (particles[i].x < 0 || particles[i].x >= CUBE_SIZE || 
                particles[i].y < 0 || particles[i].y >= CUBE_SIZE || 
                particles[i].z < 0 || particles[i].z >= CUBE_SIZE || 
                particles[i].age > 30) {
                particles[i].active = 0;
            } else {
                // Draw the particle
                rgb_t particleColor = {
                    particles[i].r, 
                    particles[i].g, 
                    particles[i].b
                };
                
                // Fade color with age
                float ageFade = 1.0f - (particles[i].age / 30.0f);
                particleColor.r = (uint8_t)(particleColor.r * ageFade);
                particleColor.g = (uint8_t)(particleColor.g * ageFade);
                particleColor.b = (uint8_t)(particleColor.b * ageFade);
                
                // Apply brightness scaling
                particleColor = scaleBrightness(particleColor, brightness);
                
                // Set voxel at the particle position
                setVoxel((uint8_t)particles[i].x, (uint8_t)particles[i].y, (uint8_t)particles[i].z, particleColor);
            }
        }
    }
}

// Wave pattern state
static uint8_t waveOffset = 0;

// Update wave pattern
void updateWavePattern(uint8_t position, float brightness) {
    clearAllLeds();
    
    // Increment wave offset for animation
    waveOffset = (waveOffset + 1) % 100;
    
    // Generate a sine wave across the cube
    for (uint8_t x = 0; x < CUBE_SIZE; x++) {
        for (uint8_t y = 0; y < CUBE_SIZE; y++) {
            // Calculate sine wave height based on position and time
            float sinX = sinf((x + waveOffset * 0.2f) * 0.5f);
            float sinY = sinf((y + waveOffset * 0.3f) * 0.5f);
            float combined = (sinX + sinY) * 0.5f + 1.0f; // Range 0 to 2
            
            // Map to z coordinate (0 to 6)
            uint8_t z = (uint8_t)(combined * (CUBE_SIZE - 1) / 2.0f);
            
            // Calculate color based on height
            rgb_t waveColor;
            float colorFactor = (float)z / (CUBE_SIZE - 1);
            waveColor.r = (uint8_t)(40.0f * (1.0f - colorFactor));
            waveColor.g = 0;
            waveColor.b = (uint8_t)(40.0f * colorFactor);
            
            // Apply brightness scale
            waveColor = scaleBrightness(waveColor, brightness);
            
            // Set the voxel at the wave height
            setVoxel(x, y, z, waveColor);
        }
    }
}

// Spinning plane pattern state
static uint8_t planeAngle = 0;

// Update spinning plane pattern
void updateSpinningPlanePattern(uint8_t position, float brightness) {
    clearAllLeds();
    
    // Increment angle for rotation
    planeAngle = (planeAngle + 1) % 72; // 5-degree increments (360/72 = 5)
    
    // Convert to radians
    float angle = planeAngle * 0.0872665f; // 5 degrees in radians
    
    // Center of cube
    float centerX = (CUBE_SIZE - 1) / 2.0f;
    float centerY = (CUBE_SIZE - 1) / 2.0f;
    float centerZ = (CUBE_SIZE - 1) / 2.0f;
    
    // Calculate color based on angle
    rgb_t planeColor;
    float colorFactor = (float)planeAngle / 72.0f;
    planeColor.r = (uint8_t)(40.0f * sinf(colorFactor * 6.28f));
    planeColor.g = (uint8_t)(40.0f * sinf(colorFactor * 6.28f + 2.09f)); // 120° offset
    planeColor.b = (uint8_t)(40.0f * sinf(colorFactor * 6.28f + 4.19f)); // 240° offset
    
    // Apply brightness scaling
    planeColor = scaleBrightness(planeColor, brightness);
    
    // Draw a plane rotating around the Z axis
    for (uint8_t x = 0; x < CUBE_SIZE; x++) {
        for (uint8_t y = 0; y < CUBE_SIZE; y++) {
            // Calculate position relative to center
            float relX = x - centerX;
            float relY = y - centerY;
            
            // Apply rotation matrix for z-axis rotation
            float rotX = relX * cosf(angle) - relY * sinf(angle);
            float rotY = relX * sinf(angle) + relY * cosf(angle);
            
            // If rotated Y is close to 0, it's on the plane
            if (fabsf(rotY) < 0.5f) {
                // Loop through all Z levels
                for (uint8_t z = 0; z < CUBE_SIZE; z++) {
                    setVoxel(x, y, z, planeColor);
                }
            }
        }
    }
}
/*
// RGB rain pattern state
typedef struct {
    uint8_t x;
    uint8_t y;
    uint8_t z;
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t active;
} rgbdrop_t;

#define MAX_RGB_DROPS 20
static rgbdrop_t rgbDrops[MAX_RGB_DROPS];

// Initialize RGB rain pattern
void initRainRGBPattern(void) {
    for (int i = 0; i < MAX_RGB_DROPS; i++) {
        // Initially all drops are inactive
        rgbDrops[i].active = 0;
    }
}
*/
// Update RGB rain pattern - colored raindrops falling from top to bottom
void updateRainRGBPattern(uint8_t position, float brightness) {
    clearAllLeds();
    
    // Create new drops randomly
    if (position % 5 == 0) {
        for (int i = 0; i < MAX_RGB_DROPS; i++) {
            if (!rgbDrops[i].active) {
                // 30% chance to create a new drop
                if ((rand() % 10) < 3) {
                    rgbDrops[i].x = rand() % CUBE_SIZE;
                    rgbDrops[i].y = rand() % CUBE_SIZE;
                    rgbDrops[i].z = CUBE_SIZE - 1; // Start at the top
                    
                    // Random RGB color
                    rgbDrops[i].r = rand() % 40;
                    rgbDrops[i].g = rand() % 40;
                    rgbDrops[i].b = rand() % 40;
                    
                    // Ensure drop has some minimum brightness
                    if (rgbDrops[i].r + rgbDrops[i].g + rgbDrops[i].b < 20) {
                        switch (rand() % 3) {
                            case 0: rgbDrops[i].r = 40; break;
                            case 1: rgbDrops[i].g = 40; break;
                            case 2: rgbDrops[i].b = 40; break;
                        }
                    }
                    
                    rgbDrops[i].active = 1;
                    break; // Only create one new drop per iteration
                }
            }
        }
    }
    
    // Move all active drops down
    for (int i = 0; i < MAX_RGB_DROPS; i++) {
        if (rgbDrops[i].active) {
            // If at bottom, deactivate
            if (rgbDrops[i].z == 0) {
                rgbDrops[i].active = 0;
            } else {
                // Move down
                rgbDrops[i].z--;
            }
            
            // Draw the active drops with their color
            if (rgbDrops[i].active) {
                rgb_t dropColor;
                
                // Full color at the front of the drop
                dropColor.r = rgbDrops[i].r;
                dropColor.g = rgbDrops[i].g;
                dropColor.b = rgbDrops[i].b;
                dropColor = scaleBrightness(dropColor, brightness);
                setVoxel(rgbDrops[i].x, rgbDrops[i].y, rgbDrops[i].z, dropColor);
                
                // Dimmer trail behind
                if (rgbDrops[i].z < CUBE_SIZE - 1) {
                    dropColor.r = rgbDrops[i].r / 2;
                    dropColor.g = rgbDrops[i].g / 2;
                    dropColor.b = rgbDrops[i].b / 2;
                    dropColor = scaleBrightness(dropColor, brightness);
                    setVoxel(rgbDrops[i].x, rgbDrops[i].y, rgbDrops[i].z + 1, dropColor);
                }
                
                // Even dimmer second trail element
                if (rgbDrops[i].z < CUBE_SIZE - 2) {
                    dropColor.r = rgbDrops[i].r / 4;
                    dropColor.g = rgbDrops[i].g / 4;
                    dropColor.b = rgbDrops[i].b / 4;
                    dropColor = scaleBrightness(dropColor, brightness);
                    setVoxel(rgbDrops[i].x, rgbDrops[i].y, rgbDrops[i].z + 2, dropColor);
                }
            }
        }
    }
}