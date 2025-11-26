#include "../inc/pwm.h"

// Simple delay function
void delay_fn(volatile int count) {
    for (volatile int i = 0; i < count; i++);
}

int main(void) {
    PWM pwm1(PWM_CHANNEL_1, QUARTER_DC, PWM_PRESCALER_15U, 1000U, TIM_9);

    USART usart(USART_2, TX_ONLY, __115200);

    usart.USART_WriteString("PWM Debugging:\r\n");
    usart.USART_WriteString("Connected on: ");
    usart.USART_WriteString(usart.GetPortName());
    usart.USART_WriteString("\r\n");

    while(1) {
        usart.USART_WriteString("Setting duty cycle to 50%\r\n");
        pwm1.SetDutyCycle(HALF_DC); // 50% of ARR=1000
        usart.USART_WriteString("Duty cycle set to 50%\r\n");
        delay_fn(1000000);

        usart.USART_WriteString("Setting duty cycle to 25%\r\n");
        pwm1.SetDutyCycle(QUARTER_DC); // 25% of ARR=1000
        usart.USART_WriteString("Duty cycle set to 25%\r\n");
        delay_fn(1000000);
    }
}

