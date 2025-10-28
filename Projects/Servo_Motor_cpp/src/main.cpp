#include "servo.h"

// Function prototypes
void delay(volatile uint32_t count);

//main function
int main(void) {
    // Create and initialize a Servo object on TIM9 CH1 (PE5 AF3)
    Servo servo_PE5(SERVO_180_TYPE, SERVO_DEFAULT_ANGLE, TIM_9, GPIO_E, RCC, 5, 3, GPIOE_EN);
    servo_PE5.SetAngle(SERVO_DEFAULT_ANGLE);
    servo_PE5.Start();

    while (1) {
        for (uint8_t angle = SERVO_MIN_ANGLE; angle <= SERVO_MAX_ANGLE; angle += 10) {
            servo_PE5.SetAngle(static_cast<servoAngle_Type>(angle));
            delay(1000000); // ~100ms at 16 MHz
        }
        for (int16_t angle = SERVO_MAX_ANGLE; angle >= SERVO_MIN_ANGLE; angle -= 10) {
            servo_PE5.SetAngle(static_cast<servoAngle_Type>(angle));
            delay(1000000); // ~100ms at 16 MHz
        }
    }
}

void delay(volatile uint32_t count) {
    while(count--) {
        __asm__("nop");
    }
}
