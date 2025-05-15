/**
 * @file patterns.h
 * @brief Animation library for the 7󬱟 RGB cube.
 *
 * Each pattern provides an Init()  (called once) and a Step(t) (called every frame).
 * The table g_patterns lets the main loop iterate through them by index.
 */
#ifndef PATTERNS_H
#define PATTERNS_H
#include <stdint.h>

/* -------- pattern prototypes -------- */
void Pattern_Rainbow_Init(void);
void Pattern_Rainbow_Step(uint32_t t);

void Pattern_ColumnPulse_Init(void);
void Pattern_ColumnPulse_Step(uint32_t t);

void Pattern_TestAxis_X(void);
void Pattern_TestAxis_Y(void);
void Pattern_TestAxis_Z(void);


/* -------- pattern table -------- */
typedef void (*pattern_step_t)(uint32_t t);

extern const pattern_step_t g_patterns[];
extern const uint8_t        g_numPatterns;

#endif // PATTERNS_H
