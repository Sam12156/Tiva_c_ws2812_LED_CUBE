/**
 * @file board.h
 * @brief Board pin mappings, constants and basic macros.
 */
#ifndef BOARD_H
#define BOARD_H

#include "TM4C123GH6PM.h"

// CPU 
#define SYS_CLOCK      80000000U

// GPIO pin masks (CMSIS doesn’t provide these)
#define GPIO_PIN_0     (1U<<0)
#define GPIO_PIN_1     (1U<<1)
#define GPIO_PIN_2     (1U<<2)
#define GPIO_PIN_3     (1U<<3)
#define GPIO_PIN_4     (1U<<4)
#define GPIO_PIN_5     (1U<<5)
#define GPIO_PIN_6     (1U<<6)
#define GPIO_PIN_7     (1U<<7)

// WS2812 data out (PA5 -> SSI0TX)
#define WS2812_PORT    GPIO_PORTA_BASE
#define WS2812_PIN     GPIO_PIN_5

// User buttons on LaunchPad
#define BUTTON1_PORT   GPIO_PORTF_BASE
#define BUTTON1_PIN    GPIO_PIN_4    // SW1, active-low
#define BUTTON2_PORT   GPIO_PORTF_BASE
#define BUTTON2_PIN    GPIO_PIN_0    // SW2, active-low (unlocked)

// Potentiometer analog input
#define POT_PORT       GPIO_PORTE_BASE
#define POT_PIN        GPIO_PIN_3    // AIN0

// Cube dimensions
#define CUBE_SIZE      7
#define NUM_LEDS       (CUBE_SIZE * CUBE_SIZE * CUBE_SIZE)

// Prototype for Board_Init if you’re using it
void Board_Init(void);

#endif // BOARD_H
