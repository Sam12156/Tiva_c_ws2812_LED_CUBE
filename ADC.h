/**
 * @file adc.h
 * @brief ADC driver for reading potentiometer.
 */
#ifndef ADC_H
#define ADC_H
#include <stdint.h>

/**
 * @brief Initialize ADC0 sample sequencer 0 for AIN0.
 */
void ADC_Init(void);

/**
 * @brief Read a single sample from ADC0 at the given channel.
 * @param channel ADC channel number (0..7).
 * @return 12-bit ADC result.
 */
uint16_t ADC0_ReadChannel(uint8_t channel);

#endif // ADC_H
