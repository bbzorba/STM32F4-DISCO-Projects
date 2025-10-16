#ifndef SERVO_H
#define SERVO_H

#include "../../../Drivers/GPIO/inc/gpio.h"
#include "../../../Drivers/PWM/inc/pwm.h"

// Peripheral addresses
#define GPIOE_BASE (AHB1PERIPH_BASE + 0x00001000U)

// Servo configuration
#define SERVO_MIN_ANGLE 0
#define SERVO_MAX_ANGLE 180
#define SERVO_DEFAULT_ANGLE 90

// GPIO configuration for servo control
#define GPIO_SERVO_PIN GPIO_PIN_5 // Example pin, change as needed
#define GPIO_SERVO_PORT GPIO_E     // Example port, change as needed

#endif // SERVO_H