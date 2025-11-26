#include "../inc/systick.h"

SYSTICK::SYSTICK(SysTick_ManualType *regs, SysTick_StatusTypeDef status){
    this->regs = regs;
    this->status = status;
    this->SystemCoreClock = 16000000; // Assuming default HSI clock

    if(this->status != SYSTICK_OK){
        // Handle error if needed
        return;
    }
}

void SYSTICK::delay(volatile uint32_t sec){

    this->regs->LOAD = 16000000 * sec - 1; // Load the SysTick timer for 1ms
    this->regs->VAL = 0; // Clear the current value
    this->regs->CTRL = 5; // Enable SysTick with processor clock, no interrupt

    for(volatile uint32_t i=0; i<sec; i++){
        while (!(this->regs->CTRL & SysTick_CTRL_COUNTFLAG_Msk)); // Wait for the COUNTFLAG
    }
    
    this->regs->CTRL = 0; // Disable SysTick
}

void SYSTICK::delay_ms(volatile uint32_t ms){

    this->regs->LOAD = 16000 - 1; // Load the SysTick timer for 1ms
    this->regs->VAL = 0; // Clear the current value
    this->regs->CTRL = 5; // Enable SysTick with processor clock, no interrupt

    for(volatile uint32_t i=0; i<ms; i++){
        while (!(this->regs->CTRL & SysTick_CTRL_COUNTFLAG_Msk)); // Wait for the COUNTFLAG
    }
    
    this->regs->CTRL = 0; // Disable SysTick
}