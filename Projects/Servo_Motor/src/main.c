#include "servo.h"

// Function prototypes
void delay(volatile uint32_t count);

//main function
int main(void) {
    // Initialize servo on TIM9 channel 1 (PE5 AF3)
    Servo_GPIO_Init(GPIO_E);
    Servo_PWM_Init(TIM_9, RCC);
    Servo_SetAngle(90, TIM_9);
    Servo_Start(TIM_9);

    while (1) {
        for (uint8_t angle = SERVO_MIN_ANGLE; angle <= SERVO_MAX_ANGLE; angle += 10) {
            Servo_SetAngle(angle, TIM_9);
            delay(1000000); // ~100ms at 16 MHz
        }
        for (int8_t angle = SERVO_MAX_ANGLE; angle >= SERVO_MIN_ANGLE; angle -= 10) {
            Servo_SetAngle(angle, TIM_9);
            delay(1000000); // ~100ms at 16 MHz
        }
    }
}

void delay(volatile uint32_t count) {
    while(count--) {
        __asm__("nop");
    }
}
