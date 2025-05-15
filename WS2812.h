/**
 * @file ws2812.h
 * @brief WS2812 (NeoPixel) LED driver.
 */
#ifndef WS2812_H
#define WS2812_H
#include <stdint.h>

/**
 * @brief Initialize SSI0 for WS2812 timing.
 */
void WS2812_Init(void);

/**
 * @brief Send GRB buffer to WS2812 chain.
 * @param grb   Pointer to GRB byte array (length = count*3).
 * @param count Number of LEDs.
 */
int WS2812_Show(const uint8_t *grb, int count);

#endif // WS2812_H
