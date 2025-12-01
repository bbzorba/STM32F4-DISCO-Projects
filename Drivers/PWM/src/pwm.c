#include "../inc/pwm.h"

void PWM_constructor(PWM_HandleType* const pwmHandle,
                     PWM_Channel_TypeDef channel,
                     dutyCycle_TypeDef dutyCycle,
                     PWM_Prescaler_TypeDef prescaler,
                     uint32_t arr,
                     TIM_Type *TIMx) {
    pwmHandle->channel = channel;
    pwmHandle->dutyCycle = dutyCycle;
    pwmHandle->prescaler = prescaler;
    pwmHandle->TIMx = TIMx;
    pwmHandle->arr = arr;

    Timer_Init(pwmHandle);
    Configure_PWM(pwmHandle); // ARR=1000 for 1kHz base frequency
    PWM_SetDutyCycle(pwmHandle);
}

void Timer_Init(PWM_HandleType* pwmHandle) {
    // Enable clock for selected TIMx
    if (pwmHandle->TIMx == (TIM_Type *)TIM1_BASE) {
        RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
    } else if (pwmHandle->TIMx == (TIM_Type *)TIM9_BASE) {
        RCC->APB2ENR |= RCC_APB2ENR_TIM9EN;
    } else if (pwmHandle->TIMx == (TIM_Type *)TIM2_BASE) {
        RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    } else if (pwmHandle->TIMx == (TIM_Type *)TIM3_BASE) {
        RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    } else if (pwmHandle->TIMx == (TIM_Type *)TIM4_BASE) {
        RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
    } else if (pwmHandle->TIMx == (TIM_Type *)TIM5_BASE) {
        RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;
    }
}

void Configure_PWM(PWM_HandleType* pwmHandle) {
    // Time base configuration
    pwmHandle->TIMx->TIM_PSC = (uint32_t)pwmHandle->prescaler; // Set prescaler
    pwmHandle->TIMx->TIM_ARR = pwmHandle->arr;
    pwmHandle->TIMx->TIM_CCMR1 |= TIM_CCMR1_OC1M_PWM1; // PWM mode 1 (OC1M=110)
    pwmHandle->TIMx->TIM_CCMR1 |= TIM_CCMR1_OC1PE; // Enable preload for CCR1
    pwmHandle->TIMx->TIM_CCER &= ~TIM_CCER_CC1P;   // Active high polarity
    pwmHandle->TIMx->TIM_CCER |= TIM_CCER_CC1E;    // Enable output on channel 1
    pwmHandle->TIMx->TIM_CR1  |= TIM_CR1_ARPE;        // ARPE: auto-reload preload enable
    pwmHandle->TIMx->TIM_EGR  |= TIM_EGR_UG;       // Generate update to latch PSC/ARR/CCR
    // MOE exists only on advanced timers (e.g., TIM1). Guard to avoid touching non-existent BDTR.
    if (pwmHandle->TIMx == (TIM_Type *)TIM1_BASE) {
        pwmHandle->TIMx->TIM_BDTR |= TIM_BDTR_MOE;     // Main output enable (advanced timer)
    }
    pwmHandle->TIMx->TIM_CR1 |= TIM_CR1_CEN; // Enable the timer
}

void PWM_SetDutyCycle(PWM_HandleType* pwmHandle) {
    // Set PWM duty cycle for specified channel (stub implementation)
    switch(pwmHandle->channel) {
    case PWM_CHANNEL_1: 
        pwmHandle->TIMx->TIM_CCR1 = pwmHandle->dutyCycle;
        break;
    case PWM_CHANNEL_2:
        pwmHandle->TIMx->TIM_CCR2 = pwmHandle->dutyCycle;
        break;
    case PWM_CHANNEL_3:
        pwmHandle->TIMx->TIM_CCR3 = pwmHandle->dutyCycle;
        break;
    case PWM_CHANNEL_4:
        pwmHandle->TIMx->TIM_CCR4 = pwmHandle->dutyCycle;
        break;
    default:
        break;
    }
}

uint32_t PWM_GetDutyCycle(PWM_HandleType* pwmHandle) {
    // Get current PWM duty cycle for specified channel (stub implementation)
    switch(pwmHandle->channel) {
    case PWM_CHANNEL_1:
        return pwmHandle->TIMx->TIM_CCR1;
    case PWM_CHANNEL_2:
        return pwmHandle->TIMx->TIM_CCR2;
    case PWM_CHANNEL_3:
        return pwmHandle->TIMx->TIM_CCR3;
    case PWM_CHANNEL_4:
        return pwmHandle->TIMx->TIM_CCR4;
    default:
        break;
    }

    return 0;
}