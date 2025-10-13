#include "systick.h"

SYSTICK::SYSTICK() {
    // Constructor (if needed)
}

void SYSTICK::SysTick_delay(volatile uint32_t sec){
    int i;

    SysTick->LOAD = 16000000 * sec - 1; // Load the SysTick timer for 1ms
    SysTick->VAL = 0; // Clear the current value
    SysTick->CTRL = 5; // Enable SysTick with processor clock, no interrupt

    for(i=0; i<sec; i++){
        while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk)); // Wait for the COUNTFLAG
    }
    
    SysTick->CTRL = 0; // Disable SysTick
}

void SYSTICK::SysTick_delay_ms(volatile uint32_t ms){
    int i;

    SysTick->LOAD = 16000 - 1; // Load the SysTick timer for 1ms
    SysTick->VAL = 0; // Clear the current value
    SysTick->CTRL = 5; // Enable SysTick with processor clock, no interrupt

    for(i=0; i<ms; i++){
        while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk)); // Wait for the COUNTFLAG
    }
    
    SysTick->CTRL = 0; // Disable SysTick
}