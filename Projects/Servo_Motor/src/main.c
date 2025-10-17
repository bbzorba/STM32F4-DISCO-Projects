#include "servo.h"

// Function prototypes
void delay(volatile uint32_t count);

//main function
int main(void) {
    // Create and initialize a Servo object on TIM9 CH1 (PE5 AF3)
    Servo servo_PE5;
    servo_constructor(&servo_PE5, SERVO_180_TYPE, SERVO_DEFAULT_ANGLE, TIM_9, GPIO_E, RCC, 5, 3, GPIOE_EN);
    Servo_SetAngle(&servo_PE5, SERVO_DEFAULT_ANGLE);
    Servo_Start(&servo_PE5);

    while (1) {
        for (uint8_t angle = SERVO_MIN_ANGLE; angle <= SERVO_MAX_ANGLE; angle += 10) {
            Servo_SetAngle(&servo_PE5, angle);
            delay(1000000); // ~100ms at 16 MHz
        }
        for (int8_t angle = SERVO_MAX_ANGLE; angle >= SERVO_MIN_ANGLE; angle -= 10) {
            Servo_SetAngle(&servo_PE5, (servoAngle_Type)angle);
            delay(1000000); // ~100ms at 16 MHz
        }
    }
}

void delay(volatile uint32_t count) {
    while(count--) {
        __asm__("nop");
    }
}
