#include "../inc/systick.h"

void SysTick_constructor(SysTick_HandleTypeDef *handle, SysTick_ManualType *regs, SysTick_StatusTypeDef status){
    handle->regs = regs;
    handle->status = status;
    handle->SystemCoreClock = 16000000; // Assuming default HSI clock

    if(handle->status != SYSTICK_OK){
        // Handle error if needed
        return;
    }
}

void SysTick_delay(SysTick_HandleTypeDef *handle, volatile uint32_t sec){
    int i;

    handle->regs->LOAD = 16000000 * sec - 1; // Load the SysTick timer for 1ms
    handle->regs->VAL = 0; // Clear the current value
    handle->regs->CTRL = 5; // Enable SysTick with processor clock, no interrupt

    for(i=0; i<sec; i++){
        while (!(handle->regs->CTRL & SysTick_CTRL_COUNTFLAG_Msk)); // Wait for the COUNTFLAG
    }
    
    handle->regs->CTRL = 0; // Disable SysTick
}

void SysTick_delay_ms(SysTick_HandleTypeDef *handle, volatile uint32_t ms){
    int i;

    handle->regs->LOAD = 16000 - 1; // Load the SysTick timer for 1ms
    handle->regs->VAL = 0; // Clear the current value
    handle->regs->CTRL = 5; // Enable SysTick with processor clock, no interrupt

    for(i=0; i<ms; i++){
        while (!(handle->regs->CTRL & SysTick_CTRL_COUNTFLAG_Msk)); // Wait for the COUNTFLAG
    }
    
    handle->regs->CTRL = 0; // Disable SysTick
}