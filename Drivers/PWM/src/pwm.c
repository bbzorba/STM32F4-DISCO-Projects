#include "pwm.h"

void PWM_Init(void) {
    //clock enable for TIM1
    RCC->RCC_APB2ENR |= RCC_APB2ENR_TIM1EN;

    // Configure TIM1 for PWM mode (stub implementation)
    TIM_1->TIM_PSC = 15; // Prescaler
    TIM_1->TIM_ARR = 1000; // Auto-reload value for 1 kHz frequency
    TIM_1->TIM_CCMR1 |= TIM_CCMR1_OC1M_PWM1; // PWM mode 1 (OC1M=110)
    TIM_1->TIM_CCMR1 |= TIM_CCMR1_OC1PE; // Enable preload for CCR1
    TIM_1->TIM_CCER &= ~TIM_CCER_CC1P;   // Active high polarity
    TIM_1->TIM_CCER |= TIM_CCER_CC1E;    // Enable output on channel 1
    TIM_1->TIM_CR1  |= TIM_CR1_ARPE;        // ARPE: auto-reload preload enable
    TIM_1->TIM_EGR  |= TIM_EGR_UG;       // Generate update to latch PSC/ARR/CCR
    TIM_1->TIM_BDTR |= TIM_BDTR_MOE;     // Main output enable (advanced timer)
    TIM_1->TIM_CR1 |= TIM_CR1_CEN; // Enable the timer
}

void PWM_SetDutyCycle(int channel, uint32_t duty_cycle) {
    // Set PWM duty cycle for specified channel (stub implementation)
    switch(channel) {
    case PWM_CHANNEL_1: 
        TIM_1->TIM_CCR1 = duty_cycle;
        break;
    case PWM_CHANNEL_2:
        TIM_1->TIM_CCR2 = duty_cycle;
        break;
    case PWM_CHANNEL_3:
        TIM_1->TIM_CCR3 = duty_cycle;
        break;
    case PWM_CHANNEL_4:
        TIM_1->TIM_CCR4 = duty_cycle;
        break;
    default:
        break;
    }
}

uint32_t PWM_GetDutyCycle(int channel) {
    // Get current PWM duty cycle for specified channel (stub implementation)
    switch(channel) {
    case PWM_CHANNEL_1:
        return TIM_1->TIM_CCR1;
    case PWM_CHANNEL_2:
        return TIM_1->TIM_CCR2;
    case PWM_CHANNEL_3:
        return TIM_1->TIM_CCR3;
    case PWM_CHANNEL_4:
        return TIM_1->TIM_CCR4;
    default:
        break;
    }

    return 0;
}