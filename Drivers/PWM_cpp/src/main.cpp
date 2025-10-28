#include "pwm.h"

// Simple delay function
void delay_fn(volatile int count) {
    for (volatile int i = 0; i < count; i++);
}

int main(void) {
    PWM pwm_timer(TIM_9, PWM_PRESCALER_1599U, 1000U); // 10kHz, ARR=1000 for 10Hz PWM

    while(1) {
        pwm_timer.SetDutyCycle(TIM_9, PWM_CHANNEL_1, HALF_DC); // 50% of ARR=1000
        delay_fn(1000000);
        pwm_timer.SetDutyCycle(TIM_9, PWM_CHANNEL_1, QUARTER_DC); // 25% of ARR=1000
        delay_fn(1000000);
    }
}

