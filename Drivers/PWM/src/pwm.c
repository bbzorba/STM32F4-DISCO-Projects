#include "pwm.h"

void Timer_Init(TIM_TypeDef *TIMx, RCC_TypeDef *rcc) {
    // Enable clock for selected TIMx
    if (TIMx == (TIM_TypeDef *)TIM1_BASE) {
        rcc->RCC_APB2ENR |= RCC_APB2ENR_TIM1EN;
    } else if (TIMx == (TIM_TypeDef *)TIM9_BASE) {
        rcc->RCC_APB2ENR |= RCC_APB2ENR_TIM9EN;
    } else if (TIMx == (TIM_TypeDef *)TIM2_BASE) {
        rcc->RCC_APB1ENR |= RCC_APB1ENR_TIM2EN;
    } else if (TIMx == (TIM_TypeDef *)TIM3_BASE) {
        rcc->RCC_APB1ENR |= RCC_APB1ENR_TIM3EN;
    } else if (TIMx == (TIM_TypeDef *)TIM4_BASE) {
        rcc->RCC_APB1ENR |= RCC_APB1ENR_TIM4EN;
    } else if (TIMx == (TIM_TypeDef *)TIM5_BASE) {
        rcc->RCC_APB1ENR |= RCC_APB1ENR_TIM5EN;
    }
}

void Configure_PWM(TIM_TypeDef *TIMx) {

    // Configure TIMx for PWM mode (stub implementation)
    TIMx->TIM_PSC = 15; // Prescaler
    TIMx->TIM_ARR = 1000; // Auto-reload value for 1 kHz frequency
    TIMx->TIM_CCMR1 |= TIM_CCMR1_OC1M_PWM1; // PWM mode 1 (OC1M=110)
    TIMx->TIM_CCMR1 |= TIM_CCMR1_OC1PE; // Enable preload for CCR1
    TIMx->TIM_CCER &= ~TIM_CCER_CC1P;   // Active high polarity
    TIMx->TIM_CCER |= TIM_CCER_CC1E;    // Enable output on channel 1
    TIMx->TIM_CR1  |= TIM_CR1_ARPE;        // ARPE: auto-reload preload enable
    TIMx->TIM_EGR  |= TIM_EGR_UG;       // Generate update to latch PSC/ARR/CCR
    // MOE exists only on advanced timers (e.g., TIM1). Guard to avoid touching non-existent BDTR.
    if (TIMx == (TIM_TypeDef *)TIM1_BASE) {
        TIMx->TIM_BDTR |= TIM_BDTR_MOE;     // Main output enable (advanced timer)
    }
    TIMx->TIM_CR1 |= TIM_CR1_CEN; // Enable the timer
}

void PWM_SetDutyCycle(TIM_TypeDef *TIMx, int channel, uint32_t duty_cycle) {
    // Set PWM duty cycle for specified channel (stub implementation)
    switch(channel) {
    case PWM_CHANNEL_1: 
        TIMx->TIM_CCR1 = duty_cycle;
        break;
    case PWM_CHANNEL_2:
        TIMx->TIM_CCR2 = duty_cycle;
        break;
    case PWM_CHANNEL_3:
        TIMx->TIM_CCR3 = duty_cycle;
        break;
    case PWM_CHANNEL_4:
        TIMx->TIM_CCR4 = duty_cycle;
        break;
    default:
        break;
    }
}

uint32_t PWM_GetDutyCycle(TIM_TypeDef *TIMx, int channel) {
    // Get current PWM duty cycle for specified channel (stub implementation)
    switch(channel) {
    case PWM_CHANNEL_1:
        return TIMx->TIM_CCR1;
    case PWM_CHANNEL_2:
        return TIMx->TIM_CCR2;
    case PWM_CHANNEL_3:
        return TIMx->TIM_CCR3;
    case PWM_CHANNEL_4:
        return TIMx->TIM_CCR4;
    default:
        break;
    }

    return 0;
}