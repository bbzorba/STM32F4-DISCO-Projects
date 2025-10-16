#ifndef SERVO_H
#define SERVO_H

#include "../../../Drivers/GPIO/inc/gpio.h"
#include "../../../Drivers/PWM/inc/pwm.h"

// Servo configuration
#define SERVO_MIN_ANGLE 0
#define SERVO_MAX_ANGLE 180
#define SERVO_DEFAULT_ANGLE 90

// GPIO configuration for servo control
#define GPIO_SERVO_PIN GPIO_PIN_5 // Example pin, change as needed
#define GPIO_SERVO_PORT GPIO_E     // Example port, change as needed
#define GPIO_SERVO_CLK RCC_AHB1ENR_GPIOEEN
#define GPIO_SERVO_MODE_BIT (0x01U << (GPIO_SERVO_PIN * 2)) // Set as output
#define GPIO_SERVO_AF 0 // Not used in this example
#define GPIO_SERVO_SPEED (0x03U << (GPIO_SERVO_PIN * 2)) // High speed
#define GPIO_SERVO_PUPD (0x00U << (GPIO_SERVO_PIN * 2)) // No pull-up/pull-down
#define GPIO_SERVO_OTYPE (0x00U << GPIO_SERVO_PIN) // Push-pull

void Servo_SetAngle(uint8_t angle);
uint8_t Servo_GetAngle(void);
void Servo_Init(void);
void Servo_Start(void);
void Servo_Stop(void);

#endif // SERVO_H