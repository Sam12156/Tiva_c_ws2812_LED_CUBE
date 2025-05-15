/**
 * @file GPIO.h
 * @brief GPIO driver for buttons and ADC pin.
 */
#ifndef GPIO_H
#define GPIO_H
#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Initialize user buttons (PF4, PF0) and POT pin (PE3).
 */
void GPIO_Init_ButtonsAndPot(void);

/**
 * @brief Check if BUTTON1 (SW1) is pressed.
 * @return true if pressed, false otherwise.
 */
bool GPIO_Button1Pressed(void);

/**
 * @brief Check if BUTTON2 (SW2) is pressed.
 * @return true if pressed, false otherwise.
 */
bool GPIO_Button2Pressed(void);

#endif // GPIO_H
