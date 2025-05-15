#include "TM4C123GH6PM.h"
#include "SysTick_Delay.h"

void SysTick_Init(void) {
    SysTick->CTRL = 0;                 // Disable SysTick during setup
    SysTick->LOAD = 0x00FFFFFF;        // Maximum reload value
    SysTick->VAL = 0;                  // Clear current value
    SysTick->CTRL = 0x00000005;        // Enable SysTick with core clock
}

// Wait function using busy-wait counting
void SysTick_Wait(unsigned long delay) {
    volatile unsigned long elapsedTime;
    unsigned long startTime = SysTick->VAL;
    do {
        elapsedTime = (startTime - SysTick->VAL) & 0x00FFFFFF; // 24-bit counter
    } while (elapsedTime <= delay);
}

// Delay in ms units with a 80 MHz clock
void SysTick_Delay(unsigned long ms) {
    unsigned long i;
    for (i = 0; i < ms; i++) {
        SysTick_Wait(80000);  // Wait 1ms (80MHz clock)
    }
}

// More direct delay function for troubleshooting
void SimpleDelay(void) {
    volatile unsigned long i;
    for (i = 0; i < 800000; i++) {
        // Just burn cycles
    }
}