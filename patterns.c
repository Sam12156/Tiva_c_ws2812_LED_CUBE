/**
 * @file patterns.c
 * @brief Implements all cube animations and pattern table.
 */
#include "patterns.h"
#include "led_cube.h"   // Cube_SetPixel, Cube_Clear
#include "SysTick_Delay.h"
/* ===========================================================
   Rainbow – hue shifts along x,y,z every frame
   ===========================================================*/
void Pattern_Rainbow_Init(void)
{
    Cube_Clear();
}

void Pattern_Rainbow_Step(uint32_t t)
{
    for (uint8_t x = 0; x < CUBE_SIZE; x++)
        for (uint8_t y = 0; y < CUBE_SIZE; y++)
            for (uint8_t z = 0; z < CUBE_SIZE; z++) {
                rgb_t c = {
                    (uint8_t)((x * 32 + t) & 0xFF),
                    (uint8_t)((y * 32 + t) & 0xFF),
                    (uint8_t)((z * 32 + t) & 0xFF)
                };
                Cube_SetPixel(x, y, z, c);
            }
}

/* ===========================================================
   Column-Pulse – bright green column sweeping through layers
   ===========================================================*/
void Pattern_ColumnPulse_Init(void)
{
    Cube_Clear();
}

void Pattern_ColumnPulse_Step(uint32_t t)
{
    uint8_t z = (t / 10) % CUBE_SIZE;     // active slice
    Cube_Clear();
    for (uint8_t x = 0; x < CUBE_SIZE; x++)
        for (uint8_t y = 0; y < CUBE_SIZE; y++)
            Cube_SetPixel(x, y, z, (rgb_t){0, 40, 0});
}

/* ===========================================================
   Pattern table – add new patterns here
   ===========================================================*/
const pattern_step_t g_patterns[] = {
    Pattern_Rainbow_Step,
    Pattern_ColumnPulse_Step
};
const uint8_t g_numPatterns = sizeof(g_patterns) / sizeof(g_patterns[0]);


void Pattern_TestAxis_X(void) {
    for (uint8_t x = 0; x < CUBE_SIZE; x++) {
        Cube_Clear();
        for (uint8_t y = 0; y < CUBE_SIZE; y++) {
            for (uint8_t z = 0; z < CUBE_SIZE; z++) {
                {
                    Cube_SetPixel(x, y, z, (rgb_t){0, 255, 0});  // Green
                }
            }
        }
        Cube_RenderFrame();
        SysTick_Delay(500);
    }
}

void Pattern_TestAxis_Y(void) {
    for (uint8_t y = 0; y < CUBE_SIZE; y++) {
        Cube_Clear();
        for (uint8_t x = 0; x < CUBE_SIZE; x++) {
            for (uint8_t z = 0; z < CUBE_SIZE; z++) {
                {
                    Cube_SetPixel(x, y, z, (rgb_t){255, 0, 0});  // Red
                }
            }
        }
        Cube_RenderFrame();
        SysTick_Delay(500);
    }
}

void Pattern_TestAxis_Z(void) {
    for (uint8_t z = 0; z < CUBE_SIZE; z++) {
        Cube_Clear();
        for (uint8_t x = 0; x < CUBE_SIZE; x++) {
            for (uint8_t y = 0; y < CUBE_SIZE; y++) {
                {
                    Cube_SetPixel(x, y, z, (rgb_t){0, 0, 255});  // Blue
                }
            }
        }
        Cube_RenderFrame();
        SysTick_Delay(500);
    }
}