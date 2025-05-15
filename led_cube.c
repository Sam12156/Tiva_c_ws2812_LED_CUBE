/**
 * @file led_cube.c
 * @brief Implements cube frame buffer and rendering with physical LED mapping.
 */

#include "led_cube.h"
#include "WS2812.h"
#include "ADC.h"
#include <stdbool.h>  // For bool type

// External declaration of ADC0_ReadChannel
uint16_t ADC0_ReadChannel(uint8_t channel);

const uint16_t led_map[7][7][7] = {
  {
    { 7, 6, 5, 4, 3, 2, 1 },
    { 8, 9, 10, 11, 12, 13, 14 },
    { 21, 20, 19, 18, 17, 16, 15 },
    { 22, 23, 24, 25, 26, 27, 28 },
    { 35, 34, 33, 32, 31, 30, 29 },
    { 36, 37, 38, 39, 40, 41, 42 },
    { 49, 48, 47, 46, 45, 44, 43 },
  },
  {
    { 56, 55, 54, 53, 52, 51, 50 },
    { 57, 58, 59, 60, 61, 62, 63 },
    { 70, 69, 68, 67, 66, 65, 64 },
    { 71, 72, 73, 74, 75, 76, 77 },
    { 84, 83, 82, 81, 80, 79, 78 },
    { 85, 86, 87, 88, 89, 90, 91 },
    { 98, 97, 96, 95, 94, 93, 92 },
  },
  {
    { 105, 104, 103, 102, 101, 100, 99 },
    { 106, 107, 108, 109, 110, 111, 112 },
    { 119, 118, 117, 116, 115, 114, 113 },
    { 120, 121, 122, 123, 124, 125, 126 },
    { 133, 132, 131, 130, 129, 128, 127 },
    { 134, 135, 136, 137, 138, 139, 140 },
    { 147, 146, 145, 144, 143, 142, 141 },
  },
  {
    { 154, 153, 152, 151, 150, 149, 148 },
    { 155, 156, 157, 158, 159, 160, 161 },
    { 168, 167, 166, 165, 164, 163, 162 },
    { 169, 170, 171, 172, 173, 174, 175 },
    { 182, 181, 180, 179, 178, 177, 176 },
    { 183, 184, 185, 186, 187, 188, 189 },
    { 196, 195, 194, 193, 192, 191, 190 },
  },
  {
    { 203, 202, 201, 200, 199, 198, 197 },
    { 204, 205, 206, 207, 208, 209, 210 },
    { 217, 216, 215, 214, 213, 212, 211 },
    { 218, 219, 220, 221, 222, 223, 224 },
    { 231, 230, 229, 228, 227, 226, 225 },
    { 232, 233, 234, 235, 236, 237, 238 },
    { 245, 244, 243, 242, 241, 240, 239 },
  },
  {
    { 252, 251, 250, 249, 248, 247, 246 },
    { 253, 254, 255, 256, 257, 258, 259 },
    { 266, 265, 264, 263, 262, 261, 260 },
    { 267, 268, 269, 270, 271, 272, 273 },
    { 280, 279, 278, 277, 276, 275, 274 },
    { 281, 282, 283, 284, 285, 286, 287 },
    { 294, 293, 292, 291, 290, 289, 288 },
  },
  {
    { 301, 300, 299, 298, 297, 296, 295 },
    { 302, 303, 304, 305, 306, 307, 308 },
    { 315, 314, 313, 312, 311, 310, 309 },
    { 316, 317, 318, 319, 320, 321, 322 },
    { 329, 328, 327, 326, 325, 324, 323 },
    { 330, 331, 332, 333, 334, 335, 336 },
    { 343, 342, 341, 340, 339, 338, 337 },
  },
};

// 3D framebuffer
static rgb_t frame[CUBE_SIZE][CUBE_SIZE][CUBE_SIZE];

// Render buffer
static uint8_t ledBuffer[NUM_LEDS * 3];

void Cube_Init(void) {
    Cube_Clear();
}

void Cube_Clear(void) {
    for (uint8_t x = 0; x < CUBE_SIZE; x++)
        for (uint8_t y = 0; y < CUBE_SIZE; y++)
            for (uint8_t z = 0; z < CUBE_SIZE; z++)
                frame[x][y][z] = (rgb_t){0, 0, 0};
                
    // Also clear the buffer
    for (int i = 0; i < NUM_LEDS * 3; i++) {
        ledBuffer[i] = 0;
    }
}

void Cube_SetPixel(uint8_t x, uint8_t y, uint8_t z, rgb_t color) {
    if (x < CUBE_SIZE && y < CUBE_SIZE && z < CUBE_SIZE)
        frame[x][y][z] = color;
}

static const uint16_t deadLEDs[] = {0, 108, 156, 157, 206, 213, 221};
static const uint8_t numDeadLEDs = 7;

static int isDeadLED(uint16_t ledNum) {
    for (uint8_t i = 0; i < numDeadLEDs; i++) {
        if (deadLEDs[i] == ledNum) {
            return 1;
        }
    }
    return 0;
}

// Modified render function that skips dead LEDs
uint8_t* Cube_RenderFrame(void) {
    float brightness = Potentiometer_GetScale(); // get brightness scaling
    
    // Apply brightness limit to prevent power issues
    if (brightness > 0.5f) brightness = 0.5f;

    for (uint8_t z = 0; z < CUBE_SIZE; z++) {
        for (uint8_t y = 0; y < CUBE_SIZE; y++) {
            for (uint8_t x = 0; x < CUBE_SIZE; x++) {
                uint16_t led_idx = led_map[x][y][z];
                
                // Skip dead LEDs and ensure valid range
                if (led_idx > 0 && led_idx <= NUM_LEDS && !isDeadLED(led_idx-1)) {
                    rgb_t c = frame[x][y][z];

                    // Adjust for 1-based LED numbering in the mapping
                    ledBuffer[(led_idx-1) * 3 + 0] = (uint8_t)(c.g * brightness);
                    ledBuffer[(led_idx-1) * 3 + 1] = (uint8_t)(c.r * brightness);
                    ledBuffer[(led_idx-1) * 3 + 2] = (uint8_t)(c.b * brightness);
                }
            }
        }
    }

    return ledBuffer;
}

float Potentiometer_GetScale(void)
{
    uint16_t raw = ADC0_ReadChannel(0);
    return (float)raw / 4095.0f; // 12-bit scale
}