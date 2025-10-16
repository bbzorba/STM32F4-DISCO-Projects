#include "servo.h"

// Function prototypes
void delay(volatile uint32_t count);


//main function
int main(void) {
    Servo_Init();
    Servo_SetAngle(90);
    Servo_Start();

    while (1) {
        for (uint8_t angle = SERVO_MIN_ANGLE; angle <= SERVO_MAX_ANGLE; angle += 10) {
            Servo_SetAngle(angle);
            delay(1000000); // ~100ms at 16 MHz
        }
        for (int8_t angle = SERVO_MAX_ANGLE; angle >= SERVO_MIN_ANGLE; angle -= 10) {
            Servo_SetAngle(angle);
            delay(1000000); // ~100ms at 16 MHz
        }
    }
}

void delay(volatile uint32_t count) {
    while(count--) {
        __asm__("nop");
    }
}
