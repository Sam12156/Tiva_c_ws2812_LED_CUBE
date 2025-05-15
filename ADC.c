/**
 * @file adc.c
 * @brief ADC initialization and reading routines.
 */
#include "TM4C123GH6PM.h"
#include "ADC.h"
#include "board.h"
#include "WS2812.h"
#include "led_cube.h"

// Function to set up ADC for potentiometer reading
void ADC_Init(void) {
    // Enable clock for ADC0
    SYSCTL->RCGCADC |= 0x01;  // Enable ADC0 clock
    
    // Enable clock for Port E (assuming PE3 for ADC input)
    SYSCTL->RCGCGPIO |= 0x10;  // Enable Port E clock
    
    // Wait for clock stabilization
    volatile uint32_t delay = SYSCTL->RCGCGPIO;
    
    // Configure PE3 for analog input
    GPIOE->AFSEL |= 0x08;     // Enable alternate function on PE3
    GPIOE->DEN &= ~0x08;      // Disable digital function on PE3
    GPIOE->AMSEL |= 0x08;     // Enable analog function on PE3
    
    // Configure ADC0
    ADC0->ACTSS &= ~0x0001;   // Disable SS0 during configuration
    ADC0->EMUX &= ~0x000F;    // Software trigger conversion
    ADC0->SSMUX0 = 0;         // Set channel to AIN0 (PE3)
    ADC0->SSCTL0 = 0x06;      // Take one sample, set flag at end
    ADC0->ACTSS |= 0x0001;    // Enable SS0
}

// Function to read ADC value
uint16_t ADC0_ReadChannel(uint8_t channel) {
    // Set the channel in sequencer 0
    ADC0->SSMUX0 = channel & 0x0F;
    
    // Start conversion
    ADC0->PSSI |= 0x01;
    
    // Wait for conversion to complete
    while((ADC0->RIS & 0x01) == 0) {};
    
    // Read result
    uint16_t result = ADC0->SSFIFO0 & 0xFFF;
    
    // Clear completion flag
    ADC0->ISC = 0x01;
    
    return result;
}
