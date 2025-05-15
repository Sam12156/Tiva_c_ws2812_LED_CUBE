// timers.h
#ifndef TIMERS_H
#define TIMERS_H

#include <stdint.h>

void Timer0A_Init(void);          // Initialize Timer0A for ms delay
void Timer1A_Init(void);          // Initialize Timer1A for us delay

void Timer0A_DelayMs(uint32_t ms);
void Timer1A_DelayUs(uint32_t us);

#endif
