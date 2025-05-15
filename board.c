/**
 * @file board.c
 * @brief Top-level board initialization.
 *
 * Centralizes all low-level bring-up:
 *   - System clock (80 MHz PLL)
 *   - SysTick delays
 *   - WS2812 data-out pin (SSI0)
 *   - User buttons (PF4, PF0)
 *   - Potentiometer analog input (PE3 & AIN0)
 *
 * Call Board_Init() once at the start of main().
 */

#include "board.h"
#include "SysTick_Delay.h"
#include "WS2812.h"
#include "GPIO.h"
#include "ADC.h"
#include <stdint.h>
#include "TM4C123GH6PM.h"
#include "Timers.h"

/***************************************************************************
 * PLL_Init – set system clock to 80 MHz using the main 16 MHz crystal
 * Derived from TI/TM4CWare startup code.
 ****************************************************************************/


void PLL_Init(void) {
    // 0) Use RCC2
    SYSCTL->RCC2 |= 0x80000000;  // USERCC2
    // 1) Bypass PLL while initializing
    SYSCTL->RCC2 |= 0x00000800;  // BYPASS2
    // 2) Select crystal value and oscillator source
    SYSCTL->RCC = (SYSCTL->RCC & ~0x000007C0) | 0x00000540;  // 16 MHz crystal
    SYSCTL->RCC2 &= ~0x00000070;  // Main oscillator
    // 3) Activate PLL by clearing PWRDN
    SYSCTL->RCC2 &= ~0x00002000;  // Clear PWRDN2
    // 4) Set system clock divider and use 400 MHz PLL
    SYSCTL->RCC2 |= 0x40000000;  // DIV400
    SYSCTL->RCC2 = (SYSCTL->RCC2 & ~0x1FC00000) | (4 << 22);  // 80 MHz = 400 MHz / (4+1)
    // 5) Wait for PLL to lock
    while ((SYSCTL->RIS & 0x00000040) == 0) {};  // Wait for PLLLRIS
    // 6) Enable PLL by clearing BYPASS
    SYSCTL->RCC2 &= ~0x00000800;  // Clear BYPASS2
}


void Board_Init(void) 
	{
		// 80 MHz core
		PLL_Init();
		
		//Timer0A_Init();   // for ms delays
    //Timer1A_Init();   // for us delays
		

    //SysTick for µs/ms delays
    SysTick_Init();

    //WS2812 driver (enables SSI0 and PA5)
		WS2812_Init();

    //Buttons & potentiometer GPIO
    GPIO_Init_ButtonsAndPot();

    //ADC sequencer for POT on AIN0
    ADC_Init();
		

		SYSCTL->RCGCGPIO |= 0x20;  // Enable clock to GPIO Port F (bit 5)
		GPIOF->DIR |= 0x02;               // Set PF1 as output
		GPIOF->DEN |= 0x02;               // Enable digital
		GPIOF->AFSEL &= ~0x02;
		GPIOF->DATA &= ~0x02;
}
