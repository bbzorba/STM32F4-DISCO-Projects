#include "../inc/servo.h"

// Function prototypes
void delay(volatile uint32_t count);

//main function
int main(void) {
    // Create and initialize a Servo object on TIM9 CH1 (PE5 AF3)
    Servo_HandleType servo_PE5;

    GPIO_InitTypeDef servo_gpio_init;
    servo_gpio_init.Pin = (1U << 5); // PE5
    servo_gpio_init.Mode = GPIO_MODE_AF_PP;
    servo_gpio_init.Pull = GPIO_NOPULL;
    servo_gpio_init.Speed = GPIO_SPEED_MEDIUM;
    servo_gpio_init.Alternate = 3; // AF3 TIM9 CH1

    servo_constructor(&servo_PE5,
                      SERVO_180_TYPE,
                      SERVO_DEFAULT_ANGLE,
                      RCC,          // rcc
                      5,            // pinNumber
                      3,            // afNumber
                      GPIO_E,       // GPIO port
                      &servo_gpio_init,
                      PWM_CHANNEL_1,
                      HALF_DC,
                      PWM_PRESCALER_1599U,
                      200,
                      TIM_9);
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
