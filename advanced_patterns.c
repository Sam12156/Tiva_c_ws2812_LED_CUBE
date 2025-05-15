/**
 * @file advanced_patterns.c
 * @brief More complex 3D animations for the 7x7x7 LED cube.
 */

#include "board.h"
#include "led_cube.h"
#include "new_patterns.h"
#include "common_functions.h"
#include <stdlib.h>
#include <math.h>

// Pattern types (to be added to your existing patterns)
#define PATTERN_DNA             11  // DNA double helix animation
#define PATTERN_GAME_OF_LIFE_3D 12  // 3D version of Conway's Game of Life
#define PATTERN_CUBE_IN_CUBE    13  // Cube within cube animation
#define PATTERN_SNAKE_3D        14  // 3D snake that moves around the cube

// DNA pattern state
static uint8_t dnaRotation = 0;
static uint8_t gameOfLifeReset = 1;
static uint8_t snakeReset = 1;
/*
#define MAX_SNAKE_LENGTH 50
uint8_t snakeX[MAX_SNAKE_LENGTH];  // X positions of snake segments
uint8_t snakeY[MAX_SNAKE_LENGTH];  // Y positions of snake segments
uint8_t snakeZ[MAX_SNAKE_LENGTH];  // Z positions of snake segments
uint8_t snakeLength;               // Current length of the snake
uint8_t foodX, foodY, foodZ;       // Position of the food
*/
// Function to reset the Game of Life 3D pattern
void resetGameOfLife3D(void) {
    gameOfLifeReset = 1;
}

// Function to reset the Snake 3D pattern
void resetSnake3D(void) {
    snakeReset = 1;
}

// Update DNA double helix pattern
void updateDNAPattern(uint8_t position, float brightness) {
    clearAllLeds();
    
    // Increment rotation
    dnaRotation = (dnaRotation + 1) % 36;  // 10-degree increments
    
    // Parameters for double helix
    float centerX = (CUBE_SIZE - 1) / 2.0f;
    float centerY = (CUBE_SIZE - 1) / 2.0f;
    float radius = 1.5f;  // Radius of the helix
    float angleIncrement = 20.0f;  // Angular distance between each Z level
    float startAngle = dnaRotation * 10.0f;  // Starting angle, animated
    
    // Height step between each base pair
    float zStep = 0.5f;
    
    // Draw the double helix
    for (float z = 0; z < CUBE_SIZE; z += zStep) {
        // Calculate the angle for this height
        float angle1 = startAngle + z * angleIncrement;
        float angle2 = angle1 + 180.0f;  // Second strand is 180 degrees offset
        
        // Convert to radians
        float rad1 = angle1 * 0.0174533f;
        float rad2 = angle2 * 0.0174533f;
        
        // Calculate positions
        int x1 = (int)(centerX + radius * cosf(rad1));
        int y1 = (int)(centerY + radius * sinf(rad1));
        
        int x2 = (int)(centerX + radius * cosf(rad2));
        int y2 = (int)(centerY + radius * sinf(rad2));
        
        // Check bounds
        if (x1 >= 0 && x1 < CUBE_SIZE && y1 >= 0 && y1 < CUBE_SIZE && 
            x2 >= 0 && x2 < CUBE_SIZE && y2 >= 0 && y2 < CUBE_SIZE &&
            (int)z >= 0 && (int)z < CUBE_SIZE) {
            
            // Colors for the two strands
            rgb_t color1 = {40, 0, 0};  // Red for one strand
            rgb_t color2 = {0, 0, 40};  // Blue for the other strand
            
            // Draw the two helix strands
            color1 = scaleBrightness(color1, brightness);
            color2 = scaleBrightness(color2, brightness);
            
            setVoxel(x1, y1, (int)z, color1);
            setVoxel(x2, y2, (int)z, color2);
            
            // Draw base pair connection (green) every 2 z-steps
            if (fmodf(z, 1.0f) < 0.1f) {
                rgb_t baseColor = {0, 40, 0};  // Green for base pairs
                baseColor = scaleBrightness(baseColor, brightness);
                
                // Calculate points along the base pair
                for (float t = 0.1f; t < 0.9f; t += 0.2f) {
                    int baseX = (int)(x1 * (1-t) + x2 * t);
                    int baseY = (int)(y1 * (1-t) + y2 * t);
                    
                    // Check bounds again
                    if (baseX >= 0 && baseX < CUBE_SIZE && baseY >= 0 && baseY < CUBE_SIZE) {
                        setVoxel(baseX, baseY, (int)z, baseColor);
                    }
                }
            }
        }
    }
}

// 3D Game of Life state
static uint8_t cellGrid[CUBE_SIZE][CUBE_SIZE][CUBE_SIZE];
static uint8_t nextGrid[CUBE_SIZE][CUBE_SIZE][CUBE_SIZE];


// Initialize the 3D Game of Life with random cells
void initGameOfLife3D(void) {
    // Initialize with random cells (about 20% alive)
    for (uint8_t x = 0; x < CUBE_SIZE; x++) {
        for (uint8_t y = 0; y < CUBE_SIZE; y++) {
            for (uint8_t z = 0; z < CUBE_SIZE; z++) {
                cellGrid[x][y][z] = (rand() % 100 < 20) ? 1 : 0;
            }
        }
    }
    gameOfLifeReset = 0;
}

// Count the number of live neighbors in 3D space (26 potential neighbors)
uint8_t countNeighbors3D(uint8_t x, uint8_t y, uint8_t z) {
    uint8_t count = 0;
    
    // Check all 26 neighboring cells
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            for (int dz = -1; dz <= 1; dz++) {
                // Skip the cell itself
                if (dx == 0 && dy == 0 && dz == 0) continue;
                
                // Calculate neighbor coordinates with wrapping
                int nx = (x + dx + CUBE_SIZE) % CUBE_SIZE;
                int ny = (y + dy + CUBE_SIZE) % CUBE_SIZE;
                int nz = (z + dz + CUBE_SIZE) % CUBE_SIZE;
                
                // Count if neighbor is alive
                count += cellGrid[nx][ny][nz];
            }
        }
    }
    
    return count;
}

// Update 3D Game of Life pattern
void updateGameOfLife3D(uint8_t position, float brightness) {
    // Reset if needed
    if (gameOfLifeReset || position % 100 == 0) {
        initGameOfLife3D();
    }
    
    // Only update the game state every 10 frames
    if (position % 10 == 0) {
        // Calculate the next generation
        for (uint8_t x = 0; x < CUBE_SIZE; x++) {
            for (uint8_t y = 0; y < CUBE_SIZE; y++) {
                for (uint8_t z = 0; z < CUBE_SIZE; z++) {
                    uint8_t neighbors = countNeighbors3D(x, y, z);
                    uint8_t isAlive = cellGrid[x][y][z];
                    
                    // Apply 3D Game of Life rules (modified for 3D space)
                    // A cell is born if it has exactly 5 neighbors
                    // A cell survives if it has 4 to 5 neighbors
                    if (isAlive) {
                        nextGrid[x][y][z] = (neighbors >= 4 && neighbors <= 5) ? 1 : 0;
                    } else {
                        nextGrid[x][y][z] = (neighbors == 5) ? 1 : 0;
                    }
                }
            }
        }
        
        // Update the grid for the next iteration
        for (uint8_t x = 0; x < CUBE_SIZE; x++) {
            for (uint8_t y = 0; y < CUBE_SIZE; y++) {
                for (uint8_t z = 0; z < CUBE_SIZE; z++) {
                    cellGrid[x][y][z] = nextGrid[x][y][z];
                }
            }
        }
        
        // Check if we need to reset (if too few cells are alive)
        uint8_t aliveCells = 0;
        for (uint8_t x = 0; x < CUBE_SIZE; x++) {
            for (uint8_t y = 0; y < CUBE_SIZE; y++) {
                for (uint8_t z = 0; z < CUBE_SIZE; z++) {
                    aliveCells += cellGrid[x][y][z];
                }
            }
        }
        
        if (aliveCells < 5 || aliveCells > CUBE_SIZE*CUBE_SIZE*CUBE_SIZE*0.9) {
            gameOfLifeReset = 1;
        }
    }
    
    // Draw the current state
    clearAllLeds();
    
    for (uint8_t x = 0; x < CUBE_SIZE; x++) {
        for (uint8_t y = 0; y < CUBE_SIZE; y++) {
            for (uint8_t z = 0; z < CUBE_SIZE; z++) {
                if (cellGrid[x][y][z]) {
                    // Calculate color based on number of neighbors
                    uint8_t neighbors = countNeighbors3D(x, y, z);
                    rgb_t cellColor;
                    
                    // Color gradient based on neighbors
                    cellColor.r = (neighbors < 13) ? 40 - (neighbors * 3) : 0;
                    cellColor.g = (neighbors >= 4 && neighbors <= 7) ? 40 : 0;
                    cellColor.b = (neighbors > 7) ? (neighbors - 7) * 5 : 0;
                    
                    // Apply brightness
                    cellColor = scaleBrightness(cellColor, brightness);
                    
                    // Set the voxel
                    setVoxel(x, y, z, cellColor);
                }
            }
        }
    }
}

// Cube in cube pattern state
static uint8_t cubeScale = 0;
static uint8_t expandingCube = 1;

// Update cube in cube pattern
void updateCubeInCubePattern(uint8_t position, float brightness) {
    clearAllLeds();
    
    // Update cube scale
    if (position % 5 == 0) {
        if (expandingCube) {
            cubeScale++;
            if (cubeScale >= 10) { // Scale * 0.3 = max size
                expandingCube = 0;
            }
        } else {
            if (cubeScale > 0) {
                cubeScale--;
            } else {
                expandingCube = 1;
            }
        }
    }
    
    // Calculate current cube size (0.3 to 3.0)
    float scale = 0.3f + (cubeScale * 0.3f);
    
    // Center of the cube
    float centerX = (CUBE_SIZE - 1) / 2.0f;
    float centerY = (CUBE_SIZE - 1) / 2.0f;
    float centerZ = (CUBE_SIZE - 1) / 2.0f;
    
    // Calculate the half-width of the current cube
    float halfWidth = scale;
    
    // Calculate color based on size
    rgb_t cubeColor;
    float colorScale = (float)cubeScale / 10.0f;
    cubeColor.r = (uint8_t)(40.0f * (1.0f - colorScale));
    cubeColor.g = (uint8_t)(40.0f * colorScale);
    cubeColor.b = (uint8_t)(40.0f * sinf(colorScale * 3.14159f));
    cubeColor = scaleBrightness(cubeColor, brightness);
    
    // Draw the cube edges
    for (float t = 0; t <= 1.0f; t += 0.05f) {
        // Eight corners of the cube
        float corners[8][3] = {
            {centerX - halfWidth, centerY - halfWidth, centerZ - halfWidth},
            {centerX + halfWidth, centerY - halfWidth, centerZ - halfWidth},
            {centerX - halfWidth, centerY + halfWidth, centerZ - halfWidth},
            {centerX + halfWidth, centerY + halfWidth, centerZ - halfWidth},
            {centerX - halfWidth, centerY - halfWidth, centerZ + halfWidth},
            {centerX + halfWidth, centerY - halfWidth, centerZ + halfWidth},
            {centerX - halfWidth, centerY + halfWidth, centerZ + halfWidth},
            {centerX + halfWidth, centerY + halfWidth, centerZ + halfWidth}
        };
        
        // Draw 12 edges of the cube
        // Bottom face edges
        drawLine3D(corners[0], corners[1], cubeColor);
        drawLine3D(corners[1], corners[3], cubeColor);
        drawLine3D(corners[3], corners[2], cubeColor);
        drawLine3D(corners[2], corners[0], cubeColor);
        
        // Top face edges
        drawLine3D(corners[4], corners[5], cubeColor);
        drawLine3D(corners[5], corners[7], cubeColor);
        drawLine3D(corners[7], corners[6], cubeColor);
        drawLine3D(corners[6], corners[4], cubeColor);
        
        // Vertical edges
        drawLine3D(corners[0], corners[4], cubeColor);
        drawLine3D(corners[1], corners[5], cubeColor);
        drawLine3D(corners[2], corners[6], cubeColor);
        drawLine3D(corners[3], corners[7], cubeColor);
    }
}

// Helper function to draw a 3D line
void drawLine3D(float start[3], float end[3], rgb_t color) {
    // Number of points to draw along the line
    int steps = 10;
    
    for (int i = 0; i <= steps; i++) {
        float t = (float)i / steps;
        
        // Interpolate between start and end points
        float x = start[0] + (end[0] - start[0]) * t;
        float y = start[1] + (end[1] - start[1]) * t;
        float z = start[2] + (end[2] - start[2]) * t;
        
        // Check bounds and set voxel
        if (x >= 0 && x < CUBE_SIZE && y >= 0 && y < CUBE_SIZE && z >= 0 && z < CUBE_SIZE) {
            setVoxel((uint8_t)x, (uint8_t)y, (uint8_t)z, color);
        }
    }
}

// 3D Snake pattern state
#define MAX_SNAKE_LENGTH 50
static uint8_t snakeX[MAX_SNAKE_LENGTH];
static uint8_t snakeY[MAX_SNAKE_LENGTH];
static uint8_t snakeZ[MAX_SNAKE_LENGTH];
static uint8_t snakeLength;
static uint8_t dirX, dirY, dirZ;
static uint8_t foodX, foodY, foodZ;

// Initialize the 3D Snake game
void initSnake3D(void) {
    // Initial snake position (middle of the cube)
    snakeX[0] = CUBE_SIZE / 2;
    snakeY[0] = CUBE_SIZE / 2;
    snakeZ[0] = CUBE_SIZE / 2;
    snakeLength = 5;
    
    // Fill the rest of the snake
    for (int i = 1; i < snakeLength; i++) {
        snakeX[i] = snakeX[0];
        snakeY[i] = snakeY[0];
        snakeZ[i] = snakeZ[0];
    }
    
    // Initial direction (random)
    dirX = 0;
    dirY = 0;
    dirZ = 0;
    
    // Make sure we have at least one non-zero direction
    while (dirX == 0 && dirY == 0 && dirZ == 0) {
        dirX = (rand() % 3) - 1;
        dirY = (rand() % 3) - 1;
        dirZ = (rand() % 3) - 1;
    }
    
    // Create initial food position
    placeFood();
    
    snakeReset = 0;
}

// Place food at a random empty location
void placeFood(void) {
    // Start with a random position
    foodX = rand() % CUBE_SIZE;
    foodY = rand() % CUBE_SIZE;
    foodZ = rand() % CUBE_SIZE;
    
    // Make sure it's not on the snake
    uint8_t collision = 1;
    while (collision) {
        collision = 0;
        for (int i = 0; i < snakeLength; i++) {
            if (foodX == snakeX[i] && foodY == snakeY[i] && foodZ == snakeZ[i]) {
                collision = 1;
                foodX = rand() % CUBE_SIZE;
                foodY = rand() % CUBE_SIZE;
                foodZ = rand() % CUBE_SIZE;
                break;
            }
        }
    }
}

// Update 3D Snake pattern
void updateSnake3DPattern(uint8_t position, float brightness) {
    // Reset if needed
    if (snakeReset) {
        initSnake3D();
    }
    
    // Only update the snake movement every 15 frames
    if (position % 15 == 0) {
        // Randomly change direction occasionally (30% chance)
        if (rand() % 100 < 30) {
            // Pick a random direction
            int newDir = rand() % 6;
            switch (newDir) {
                case 0: dirX = 1;  dirY = 0;  dirZ = 0;  break; // +X
                case 1: dirX = -1; dirY = 0;  dirZ = 0;  break; // -X
                case 2: dirX = 0;  dirY = 1;  dirZ = 0;  break; // +Y
                case 3: dirX = 0;  dirY = -1; dirZ = 0;  break; // -Y
                case 4: dirX = 0;  dirY = 0;  dirZ = 1;  break; // +Z
                case 5: dirX = 0;  dirY = 0;  dirZ = -1; break; // -Z
            }
        }
        
        // Move the rest of the snake (tail follows the head)
        for (int i = snakeLength - 1; i > 0; i--) {
            snakeX[i] = snakeX[i-1];
            snakeY[i] = snakeY[i-1];
            snakeZ[i] = snakeZ[i-1];
        }
        
        // Move the head in the current direction
        snakeX[0] = (snakeX[0] + dirX + CUBE_SIZE) % CUBE_SIZE; // Wrap around edges
        snakeY[0] = (snakeY[0] + dirY + CUBE_SIZE) % CUBE_SIZE;
        snakeZ[0] = (snakeZ[0] + dirZ + CUBE_SIZE) % CUBE_SIZE;
        
        // Check if the snake ate the food
        if (snakeX[0] == foodX && snakeY[0] == foodY && snakeZ[0] == foodZ) {
            // Grow the snake
            if (snakeLength < MAX_SNAKE_LENGTH) {
                snakeLength++;
                // New tail segment inherits the position of the last segment
                snakeX[snakeLength-1] = snakeX[snakeLength-2];
                snakeY[snakeLength-1] = snakeY[snakeLength-2];
                snakeZ[snakeLength-1] = snakeZ[snakeLength-2];
            }
            
            // Create new food
            placeFood();
        }
        
        // Check if snake hits itself
        for (int i = 1; i < snakeLength; i++) {
            if (snakeX[0] == snakeX[i] && snakeY[0] == snakeY[i] && snakeZ[0] == snakeZ[i]) {
                // Snake collision, reset the game
                snakeReset = 1;
                break;
            }
        }
    }
    
    // Draw the current state
    clearAllLeds();
    
    // Draw food (pulsing white)
    float pulse = (sinf(position * 0.1f) + 1.0f) * 0.5f; // 0.0 to 1.0
    rgb_t foodColor = {(uint8_t)(20.0f + 20.0f * pulse), 
                       (uint8_t)(20.0f + 20.0f * pulse), 
                       (uint8_t)(20.0f + 20.0f * pulse)};
    foodColor = scaleBrightness(foodColor, brightness);
    setVoxel(foodX, foodY, foodZ, foodColor);
    
    // Draw snake body (gradient from head to tail)
    for (int i = 0; i < snakeLength; i++) {
        float ratio = (float)i / snakeLength;
        
        // Color transition from head to tail (green->yellow->red)
        rgb_t snakeColor;
        snakeColor.r = (uint8_t)(40.0f * ratio);
        snakeColor.g = (uint8_t)(40.0f * (1.0f - ratio * 0.5f));
        snakeColor.b = 0;
        
        // Apply brightness
        snakeColor = scaleBrightness(snakeColor, brightness);
        
        // Set voxel
        setVoxel(snakeX[i], snakeY[i], snakeZ[i], snakeColor);
    }
}

// Text scroller state
typedef struct {
    const char *text;
    uint8_t scrollOffset;
    uint8_t textLength;
} scroller_t;

static scroller_t textScroller = {"HELLO", 0, 5};

// Characters defined as 5x7 bitmaps (standard font layout)
static const uint8_t charMap[128][7] = {
    // Space (32)
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    // More character definitions would go here
    // For example, letter 'A' (65)
    {0x1E, 0x21, 0x21, 0x3F, 0x21, 0x21, 0x21},
    // Letter 'B'
    {0x3E, 0x21, 0x21, 0x3E, 0x21, 0x21, 0x3E},
    // Letter 'C'
    {0x1E, 0x21, 0x20, 0x20, 0x20, 0x21, 0x1E},
    // Letter 'D'
    {0x3E, 0x11, 0x21, 0x21, 0x21, 0x11, 0x3E},
    // Letter 'E'
    {0x3F, 0x20, 0x20, 0x3E, 0x20, 0x20, 0x3F},
    // Letter 'F'
    {0x3F, 0x20, 0x20, 0x3E, 0x20, 0x20, 0x20},
    // Letter 'G'
    {0x1E, 0x21, 0x20, 0x27, 0x21, 0x21, 0x1E},
    // Letter 'H'
    {0x21, 0x21, 0x21, 0x3F, 0x21, 0x21, 0x21},
    // Letter 'I'
    {0x3E, 0x08, 0x08, 0x08, 0x08, 0x08, 0x3E},
    // Letter 'J'
    {0x01, 0x01, 0x01, 0x01, 0x01, 0x21, 0x1E},
    // Letter 'K'
    {0x21, 0x22, 0x24, 0x38, 0x24, 0x22, 0x21},
    // Letter 'L'
    {0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x3F},
    // Letter 'M'
    {0x41, 0x63, 0x55, 0x49, 0x41, 0x41, 0x41},
    // Letter 'N'
    {0x21, 0x31, 0x29, 0x25, 0x23, 0x21, 0x21},
    // Letter 'O'
    {0x1E, 0x21, 0x21, 0x21, 0x21, 0x21, 0x1E},
};

// Update scrolling text pattern
void updateTextScrollerPattern(uint8_t position, float brightness) {
    clearAllLeds();
    
    // Update scroll position every 20 frames
    if (position % 20 == 0) {
        textScroller.scrollOffset = (textScroller.scrollOffset + 1) % (textScroller.textLength * 6 + CUBE_SIZE);
    }
    
    // Calculate the first character to display
    int firstCharPos = (textScroller.scrollOffset / 6) - 1;
    int xOffset = textScroller.scrollOffset % 6;
    
    // Display up to 3 characters (max that can fit in cube width)
    for (int charPos = firstCharPos; charPos < firstCharPos + 3; charPos++) {
        // Calculate character index in the text string
        int charIdx = charPos % textScroller.textLength;
        if (charIdx < 0) charIdx += textScroller.textLength;
        
        // Get the character
        char c = textScroller.text[charIdx];
        
        // Skip if out of ascii range
        if (c < 32 || c > 126) continue;
        
        // Calculate x position in the cube
        int xStart = 6 * (charPos - firstCharPos) - xOffset;
        
        // Draw the character
        for (int row = 0; row < 7; row++) {
            for (int col = 0; col < 5; col++) {
                // Check if this pixel is set in the bitmap
                if ((charMap[c - 32][row] >> (4 - col)) & 0x01) {
                    // Calculate position
                    int x = xStart + col;
                    
                    // Make sure it's within the cube
                    if (x >= 0 && x < CUBE_SIZE) {
                        // Set this column on all Y positions (depth)
                        for (int y = 0; y < CUBE_SIZE; y++) {
                            rgb_t color = {40, 40, 0}; // Yellow text
                            color = scaleBrightness(color, brightness);
                            setVoxel(x, y, row, color);
                        }
                    }
                }
            }
        }
    }
}

// Plasma pattern state
static uint8_t plasmaOffset = 0;

// Update plasma pattern - 3D plasma effect
void updatePlasmaPattern(uint8_t position, float brightness) {
    clearAllLeds();
    
    // Increment plasma animation
    plasmaOffset = (plasmaOffset + 1) % 100;
    
    // Calculate plasma based on 3D sine waves
    for (uint8_t x = 0; x < CUBE_SIZE; x++) {
        for (uint8_t y = 0; y < CUBE_SIZE; y++) {
            for (uint8_t z = 0; z < CUBE_SIZE; z++) {
                // Calculate plasma value based on position and time
                float value = 
                    sinf(x * 0.5f + plasmaOffset * 0.1f) + 
                    sinf(y * 0.5f + plasmaOffset * 0.08f) + 
                    sinf(z * 0.5f + plasmaOffset * 0.06f) + 
                    sinf(sqrtf((x-3)*(x-3) + (y-3)*(y-3) + (z-3)*(z-3)) * 0.4f + plasmaOffset * 0.07f);
                
                // Normalize to 0-1 range
                value = (value + 4.0f) / 8.0f;
                
                // Calculate color based on plasma value
                rgb_t color;
                value *= 3.0f; // Scale to cover 3 color regions
                
                if (value < 1.0f) {
                    // Red to Yellow
                    color.r = 40;
                    color.g = (uint8_t)(40.0f * value);
                    color.b = 0;
                } else if (value < 2.0f) {
                    // Yellow to Cyan
                    value -= 1.0f;
                    color.r = (uint8_t)(40.0f * (1.0f - value));
                    color.g = 40;
                    color.b = (uint8_t)(40.0f * value);
                } else {
                    // Cyan to Magenta
                    value -= 2.0f;
                    color.r = (uint8_t)(40.0f * value);
                    color.g = (uint8_t)(40.0f * (1.0f - value));
                    color.b = 40;
                }
                
                // Apply brightness
                color = scaleBrightness(color, brightness);
                
                // Set voxel with plasma color
                setVoxel(x, y, z, color);
            }
        }
    }
}