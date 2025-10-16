#include "pwm.h"

// Simple delay function
void delay_fn(volatile int count) {
    for (volatile int i = 0; i < count; i++);
}


int main(void) {
    PWM_Init();
    while(1) {
        PWM_SetDutyCycle(PWM_CHANNEL_1, 500); // 50% of ARR=1000
        delay_fn(1000000);
        PWM_SetDutyCycle(PWM_CHANNEL_1, 250); // 25% of ARR=1000
        delay_fn(1000000);
    }
}

