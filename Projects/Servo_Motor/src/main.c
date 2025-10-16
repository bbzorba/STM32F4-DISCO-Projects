#include "servo.h"

// Function prototypes
void delay(volatile uint32_t count);


//main function
int main(void) {
    Servo_Init();
    Servo_SetAngle(90);
    Servo_Start();

    while (1) {
        // Main loop
    }
}

void delay(volatile uint32_t count) {
    while(count--) {
        __asm__("nop");
    }
}
