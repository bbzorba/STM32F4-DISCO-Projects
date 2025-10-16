#include "servo.h"

static uint8_t current_angle = SERVO_DEFAULT_ANGLE;
static uint8_t is_running = 0;

void Servo_Init(void) {
    RCC->RCC_APB2ENR |= RCC_APB2ENR_TIM1EN; // Enable TIM1 clock
    RCC->RCC_AHB1ENR |= (1U << 5); // Enable GPIOE clock

    // Configure GPIO pin for PWM output (stub implementation)
    GPIO_E->MODER &= ~(3U << (GPIO_SERVO_PIN * 2));
    GPIO_E->MODER |= (2U << (GPIO_SERVO_PIN * 2)); // Alternate function mode
    
    // Set alternate function to TIM2 (AF1) for the pin (stub implementation)
    GPIO_E->AFR[0] &= ~(0xF << (GPIO_SERVO_PIN * 4));
    GPIO_E->AFR[0] |= (1U << (GPIO_SERVO_PIN * 4));
    
    // Initialize TIM1 for PWM (stub implementation)
    TIM_1->TIM_PSC = 1600 - 1; // Prescaler for 10 kHz (assuming 16 MHz clock)
    TIM_1->TIM_ARR = 20000 - 1; // Auto-reload for 50 Hz
    TIM_1->TIM_CCR1 = 1500; // Default pulse width for 90 degrees
    TIM_1->TIM_CCMR1 |= (6U << 4); // PWM mode 1
    TIM_1->TIM_CCER |= 1U; // Enable output on channel 1
    TIM_1->TIM_CR1 |= 1U; // Enable TIM1

    current_angle = SERVO_DEFAULT_ANGLE;
    is_running = 0;
}

void Servo_SetAngle(uint8_t angle) {
    if (angle < SERVO_MIN_ANGLE || angle > SERVO_MAX_ANGLE) {
        // Invalid angle, do nothing or handle error
        return;
    }
    current_angle = angle;
    // Update PWM signal to set servo position (stub implementation)
}

uint8_t Servo_GetAngle(void) {
    return current_angle;
}

void Servo_Start(void) {
    is_running = 1;
    // Start PWM signal (stub implementation)
    PWM_SetDutyCycle(PWM_CHANNEL_1, current_angle);
}

void Servo_Stop(void) {
    is_running = 0;
    // Stop PWM signal (stub implementation)
    PWM_SetDutyCycle(PWM_CHANNEL_1, 0);
}
