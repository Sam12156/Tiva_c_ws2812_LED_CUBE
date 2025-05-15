#ifndef SYSTICK_DELAY_H
#define SYSTICK_DELAY_H

#include <stdint.h>

void SysTick_Init(void);




// Wait function using busy-wait counting
void SysTick_Wait(unsigned long delay);

// Delay in 12.5ns units with a 80 MHz clock
void SysTick_Delay(unsigned long ms);

// More direct delay function for troubleshooting
void SimpleDelay(void);

#endif
