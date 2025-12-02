#include "../inc/pwm.h"

PWM::PWM(PWM_Channel_TypeDef channel,
                     dutyCycle_TypeDef dutyCycle,
                     PWM_Prescaler_TypeDef prescaler,
                     uint32_t arr,
                     TIM_TypeDef *TIMx) {
    this->channel = channel;
    this->dutyCycle = dutyCycle;
    this->prescaler = prescaler;
    this->TIMx = TIMx;
    this->arr = arr;

    Timer_Init();
    Configure_PWM(); // ARR=1000 for 1kHz base frequency
    PWM_SetDutyCycle();
}

void PWM::Timer_Init(void) {
    // Enable clock for selected TIMx
    if (this->TIMx == (TIM_TypeDef *)TIM1_BASE) {
        RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
    } else if (this->TIMx == (TIM_TypeDef *)TIM9_BASE) {
        RCC->APB2ENR |= RCC_APB2ENR_TIM9EN;
    } else if (this->TIMx == (TIM_TypeDef *)TIM2_BASE) {
        RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    } else if (this->TIMx == (TIM_TypeDef *)TIM3_BASE) {
        RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    } else if (this->TIMx == (TIM_TypeDef *)TIM4_BASE) {
        RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
    } else if (this->TIMx == (TIM_TypeDef *)TIM5_BASE) {
        RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;
    }
}

void PWM::Configure_PWM(void) {
    // Time base configuration
    this->TIMx->TIM_PSC = (uint32_t)this->prescaler; // Set prescaler
    this->TIMx->TIM_ARR = this->arr;
    this->TIMx->TIM_CCMR1 |= TIM_CCMR1_OC1M_PWM1; // PWM mode 1 (OC1M=110)
    this->TIMx->TIM_CCMR1 |= TIM_CCMR1_OC1PE; // Enable preload for CCR1
    this->TIMx->TIM_CCER &= ~TIM_CCER_CC1P;   // Active high polarity
    this->TIMx->TIM_CCER |= TIM_CCER_CC1E;    // Enable output on channel 1
    this->TIMx->TIM_CR1  |= TIM_CR1_ARPE;        // ARPE: auto-reload preload enable
    this->TIMx->TIM_EGR  |= TIM_EGR_UG;       // Generate update to latch PSC/ARR/CCR
    // MOE exists only on advanced timers (e.g., TIM1). Guard to avoid touching non-existent BDTR.
    if (this->TIMx == (TIM_TypeDef *)TIM1_BASE) {
        this->TIMx->TIM_BDTR |= TIM_BDTR_MOE;     // Main output enable (advanced timer)
    }
    this->TIMx->TIM_CR1 |= TIM_CR1_CEN; // Enable the timer
}

void PWM::PWM_SetDutyCycle(void) {
    // Set PWM duty cycle for specified channel (stub implementation)
    switch(this->channel) {
    case PWM_CHANNEL_1: 
        this->TIMx->TIM_CCR1 = this->dutyCycle;
        break;
    case PWM_CHANNEL_2:
        this->TIMx->TIM_CCR2 = this->dutyCycle;
        break;
    case PWM_CHANNEL_3:
        this->TIMx->TIM_CCR3 = this->dutyCycle;
        break;
    case PWM_CHANNEL_4:
        this->TIMx->TIM_CCR4 = this->dutyCycle;
        break;
    default:
        break;
    }
}

uint32_t PWM::PWM_GetDutyCycle(void) {
    // Get current PWM duty cycle for specified channel (stub implementation)
    switch(this->channel) {
    case PWM_CHANNEL_1:
        return this->TIMx->TIM_CCR1;
    case PWM_CHANNEL_2:
        return this->TIMx->TIM_CCR2;
    case PWM_CHANNEL_3:
        return this->TIMx->TIM_CCR3;
    case PWM_CHANNEL_4:
        return this->TIMx->TIM_CCR4;
    default:
        break;
    }

    return 0;
}

void PWM::SetDutyCycle(dutyCycle_TypeDef duty) {
    this->dutyCycle = duty;
    PWM_SetDutyCycle();
}

void PWM::SetPulseTicks(uint32_t ticks) {
    switch (this->channel) {
    case PWM_CHANNEL_1:
        this->TIMx->TIM_CCR1 = ticks;
        break;
    case PWM_CHANNEL_2:
        this->TIMx->TIM_CCR2 = ticks;
        break;
    case PWM_CHANNEL_3:
        this->TIMx->TIM_CCR3 = ticks;
        break;
    case PWM_CHANNEL_4:
        this->TIMx->TIM_CCR4 = ticks;
        break;
    default:
        break;
    }
}

void PWM::Start() {
    this->TIMx->TIM_CR1 |= TIM_CR1_CEN;
}

void PWM::Stop() {
    this->TIMx->TIM_CR1 &= ~TIM_CR1_CEN;
}