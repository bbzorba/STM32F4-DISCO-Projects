#include "pwm.h"

// Simple delay function
void delay_fn(volatile int count) {
    for (volatile int i = 0; i < count; i++);
}

int main(void) {
    Timer_Init(TIM_9, RCC);
    Configure_PWM(TIM_9, PWM_PRESCALER_15U, 1000U);

    while(1) {
        PWM_SetDutyCycle(TIM_9, PWM_CHANNEL_1, HALF_DC); // 50% of ARR=1000
        delay_fn(1000000);
        PWM_SetDutyCycle(TIM_9, PWM_CHANNEL_1, QUARTER_DC); // 25% of ARR=1000
        delay_fn(1000000);
    }
}

