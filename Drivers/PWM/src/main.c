#include "../inc/pwm.h"

// Simple delay function
void delay_fn(volatile int count) {
    for (volatile int i = 0; i < count; i++);
}

int main(void) {
    PWM_HandleType pwm1;
    PWM_constructor(&pwm1, PWM_CHANNEL_1, QUARTER_DC, PWM_PRESCALER_15U, 1000U, TIM_9);

    USART_HandleType usart; // Initialize USART for debugging output
    USART_constructor(&usart, USART_2, TX_ONLY, __115200);

    USART_WriteString(&usart, "PWM Debugging:\r\n");
    USART_WriteString(&usart, "Connected on: ");
    USART_WriteString(&usart, GetPortName(&usart));
    USART_WriteString(&usart, "\r\n");

    while(1) {
        USART_WriteString(&usart, "Starting PWM at 1 kHz\r\n");
        PWM_Start(&pwm1);
        delay_fn(1000000);

        USART_WriteString(&usart, "Setting duty cycle to 50%\r\n");
        PWM_SetDutyCycle(&pwm1, HALF_DC); // 50% of ARR=1000
        USART_WriteString(&usart, "Duty cycle set to 50%\r\n");
        delay_fn(1000000);

        USART_WriteString(&usart, "Setting duty cycle to 25%\r\n");
        PWM_SetDutyCycle(&pwm1, QUARTER_DC); // 25% of ARR=1000
        USART_WriteString(&usart, "Duty cycle set to 25%\r\n");
        delay_fn(1000000);

        USART_WriteString(&usart, "Setting duty cycle to 75%\r\n");
        PWM_SetDutyCycle(&pwm1, THREE_QUARTERS_DC);
        USART_WriteString(&usart, "Duty cycle set to 75%\r\n");
        delay_fn(1000000);

        USART_WriteString(&usart, "\r\n");
        PWM_SetPulseTicks(&pwm1, 600U); // Directly set CCR to 600 ticks
        USART_WriteString(&usart, "Pulse width set to 600 ticks\r\n");
        delay_fn(1000000);

        USART_WriteString(&usart, "Stopping PWM\r\n");
        PWM_Stop(&pwm1);
        delay_fn(1000000);
    }
}

