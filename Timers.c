// timers.c
#include "Timers.h"
#include "TM4C123GH6PM.h"
/*
void Timer0A_Init(void) {
    SYSCTL->RCGCTIMER |= (1U << 0);      // Enable Timer0
    while ((SYSCTL->PRTIMER & (1U << 0)) == 0); // Wait until ready

    TIMER0->CTL = 0;                     // Disable during setup
    TIMER0->CFG = 0x0;                   // 32-bit mode
    TIMER0->TAMR = 0x2;                  // Periodic mode
    TIMER0->TAILR = 80000 - 1;           // 1ms @ 80 MHz
    TIMER0->ICR = 0x1;                   // Clear timeout flag
    TIMER0->CTL |= 1;                    // Enable Timer0A
}

void Timer1A_Init(void) {
    SYSCTL->RCGCTIMER |= (1U << 1);      // Enable Timer1
    while ((SYSCTL->PRTIMER & (1U << 1)) == 0);

    TIMER1->CTL = 0;
    TIMER1->CFG = 0x0;                   // 32-bit mode
    TIMER1->TAMR = 0x2;                  // Periodic mode
    TIMER1->TAILR = 80 - 1;              // 1us @ 80 MHz
    TIMER1->ICR = 0x1;
    TIMER1->CTL |= 1;
}

void Timer0A_DelayMs(uint32_t ms) {
    for (uint32_t i = 0; i < ms; i++) {
        while ((TIMER0->RIS & 0x1) == 0);
        TIMER0->ICR = 0x1;
    }
}

void Timer1A_DelayUs(uint32_t us) {
    for (uint32_t i = 0; i < us; i++) {
        while ((TIMER1->RIS & 0x1) == 0);
        TIMER1->ICR = 0x1;
    }
}
*/