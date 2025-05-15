/**
 * @file led_cube.h
 * @brief Framebuffer and render API for 7󬱟 cube.
 */
#ifndef LED_CUBE_H
#define LED_CUBE_H
#include <stdint.h>
#include "board.h"

extern const uint16_t led_map[CUBE_SIZE][CUBE_SIZE][CUBE_SIZE];

typedef struct { uint8_t g, r, b; } rgb_t;
float Potentiometer_GetScale(void);
void Cube_Init(void);
void Cube_Clear(void);
uint8_t* Cube_RenderFrame(void);


void Cube_SetPixel(uint8_t x, uint8_t y, uint8_t z, rgb_t color);

#endif // LED_CUBE_H

