/**
 * @file ws2812.c
 * @brief WS2812 (NeoPixel) SPI-based driver with timeout protection.
 */

#include "TM4C123GH6PM.h"
#include "WS2812.h"
#include "board.h"
#include "SysTick_Delay.h"
#include "Timers.h"
#include <stddef.h>  // For NULL definition

#define SPI_FREQ    2400000U    // desired SPI clock
#define RESET_US      60U       // reset pulse =50µs
#define MAX_WAIT     1000       // maximum loop iterations to prevent lockup

// Masks from SSI0.c (bit 1 = TNF/empty, bit 4 = BSY)
#define SSI_SSE      (1U<<1)    // CR1, SSE bit
#define SSI_SR_TNF   (1U<<1)    // SR, Transmit FIFO not full
#define SSI_SR_BSY   (1U<<4)    // SR, Busy flag

// Calculate buffer size based on number of LEDs and bytes per LED
// Each LED uses 3 bytes (R,G,B), each byte uses 8 bits
// Each bit expands to 3 SPI bytes 
#define SPI_BUFFER_SIZE (NUM_LEDS * 3 * 8 * 3)

// 3× expansion buffer: 1 LED-bit -> 3 SPI bits
static uint8_t spiBuf[SPI_BUFFER_SIZE];

static inline void encodeByte(uint8_t byte, uint8_t *dst) {
    for (int i = 7; i >= 0; --i) {
        // '110' => 1, '100' => 0
        *dst++ = (byte & (1<<i)) ? 0x6 : 0x4;
        *dst++ = 0;
        *dst++ = 0;
    }
}

void WS2812_Init(void) {
    // Enable clocks for SSI0 and GPIOA
    SYSCTL->RCGCSSI  |= (1U<<0);
    SYSCTL->RCGCGPIO |= (1U<<0);
    __NOP();
    __NOP(); // Additional NOPs to ensure clock is stable

    // Configure PA5 as SSI0TX
    GPIOA->AFSEL |= WS2812_PIN;
    GPIOA->PCTL  = (GPIOA->PCTL & ~0x00F00000) | (2U<<20);
    GPIOA->DEN  |= WS2812_PIN;

    // Disable SSI0 before config (clear SSE)
    SSI0->CR1 &= ~SSI_SSE;  

    // Set prescale: even CPSDVSR = SYS_CLOCK/SPI_FREQ
    {
      uint32_t ps = (SYS_CLOCK + SPI_FREQ/2) / SPI_FREQ;
      if (ps < 2) ps = 2;
      if (ps > 254) ps = 254;
      if (ps & 1) ps++;
      SSI0->CPSR = ps;
    }

    // Configure CR0: SCR=0, SPH=0, SPO=0, FRF=0, DSS=7 (8-bit)
    SSI0->CR0 = (0<<8)|(0<<7)|(0<<6)|(0<<4)|(0x7);

    // Re-enable SSI0 (set SSE)
    SSI0->CR1 |= SSI_SSE;
}

// Return 1 if successful, 0 if timed out
int WS2812_Show(const uint8_t *grb, int count) {

    
    if (count <= 0 || count > NUM_LEDS || grb == NULL) {
        return 0;
    }

    // Calculate actual bytes to send based on LED count
    uint32_t bytesToSend = count * 3 * 8 * 3;

    // Build the SPI-encoded buffer
    uint8_t *p = spiBuf;
    for (int i = 0; i < count*3; ++i) {
        encodeByte(grb[i], p);
        p += 24;
    }

    // Push out every byte with timeout protection
    for (uint32_t i = 0; i < bytesToSend; ++i) {
        // Wait for TX FIFO not full with timeout
        volatile uint32_t timeout = MAX_WAIT;
        while (!(SSI0->SR & SSI_SR_TNF) && --timeout > 0);
        if (timeout == 0) {
            // Reset SSI if it's stuck
            SSI0->CR1 &= ~SSI_SSE;  // Disable
            SSI0->CR1 |= SSI_SSE;   // Re-enable
            return 0;  // Return error
        }
        SSI0->DR = spiBuf[i];
    }
    
    // Wait until SSI no longer busy with timeout
    volatile uint32_t timeout = MAX_WAIT;
    while ((SSI0->SR & SSI_SR_BSY) && --timeout > 0);
    if (timeout == 0) {
        // Reset SSI if it's stuck
        SSI0->CR1 &= ~SSI_SSE;  // Disable
        SSI0->CR1 |= SSI_SSE;   // Re-enable
        return 0;  // Return error
    }

    // latch/reset - use a shorter delay
    SysTick_Wait(12000); // 150μs at 80MHz (24000 cycles)
    return 1;  // Success
}