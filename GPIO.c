/**
 * @file gpio.c
 * @brief Button and potentiometer pin configuration.
 */
#include "TM4C123GH6PM.h"
#include "GPIO.h"
#include "board.h"

void GPIO_Init_ButtonsAndPot(void) {
    //Enable GPIOF (buttons) and GPIOE (pot)
    SYSCTL->RCGCGPIO |= (1U<<5) | (1U<<4);
		//nop serves as a very short delay
    __NOP();

    //Unlock PF0 (SW2)
    GPIOF->LOCK = 0x4C4F434B;
    GPIOF->CR   |= BUTTON2_PIN;
    GPIOF->LOCK = 0;

    //Configure PF4, PF0 as inputs with pull-ups
    GPIOF->DIR   &= ~(BUTTON1_PIN | BUTTON2_PIN);
    GPIOF->AFSEL &= ~(BUTTON1_PIN | BUTTON2_PIN);
    GPIOF->PUR   |=  (BUTTON1_PIN | BUTTON2_PIN);
    GPIOF->DEN   |=  (BUTTON1_PIN | BUTTON2_PIN);

    //Configure PE3 as analog input for pot
    GPIOE->DIR   &= ~POT_PIN;
    GPIOE->AFSEL |=  POT_PIN;
    GPIOE->DEN   &= ~POT_PIN;
    GPIOE->AMSEL |=  POT_PIN;
}

bool GPIO_Button1Pressed(void) 
{
    return !(GPIOF->DATA & BUTTON1_PIN);
}

bool GPIO_Button2Pressed(void) 
{
    return !(GPIOF->DATA & BUTTON2_PIN);
}
