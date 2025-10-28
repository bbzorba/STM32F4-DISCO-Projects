#ifndef SERVO_H
#define SERVO_H

#include "../../../Drivers/GPIO_cpp/inc/gpio.h"
#include "../../../Drivers/PWM_cpp/inc/pwm.h"

typedef enum {
    SERVO_180_TYPE = 0,
    SERVO_360_TYPE
} servo_Type;

typedef enum {
    SERVO_OK = 0,
    SERVO_ERROR_INVALID_ANGLE,
    SERVO_ERROR_NOT_INITIALIZED
} Servo_StatusType;

typedef enum {
    SERVO_MIN_ANGLE = 0,
    SERVO_DEFAULT_ANGLE = 90,
    SERVO_MAX_ANGLE = 180
} servoAngle_Type;

// Object-oriented C: Servo instance state
class Servo {
private:
    servo_Type type;                  // 180 or 360 type
    servoAngle_Type angle;            // current angle (0..180)
    uint8_t is_running;               // 0/1 running flag
    TIM_TypeDef *TIMx;                // bound timer
    GPIO_TypeDef *GPIOx;              // bound GPIO port
    RCC_TypeDef *rcc;                 // RCC pointer
    uint8_t pinNumber;                // GPIO pin index 0..15
    uint8_t afNumber;                 // AF 0..15
    uint32_t gpioEnableMask;          // AHB1 enable mask e.g., GPIOE_EN
    PWM pwm;                          // PWM helper instance
public:
    Servo(servo_Type type,
          servoAngle_Type initial_angle,
          TIM_TypeDef *TIMx,
          GPIO_TypeDef *GPIOx,
          RCC_TypeDef *rcc,
          uint8_t pinNumber,
          uint8_t afNumber,
          uint32_t gpioEnableMask);
    // Object-oriented methods
    Servo_StatusType SetAngle(servoAngle_Type angle);
    servoAngle_Type GetAngle();
    void Start();
    void Stop();
    uint32_t angle_to_ticks(servoAngle_Type angle);

    // Generic GPIO/PWM helpers (procedural, kept for compatibility)
    void GPIO_Init();
    void PWM_Init();
};

#endif // SERVO_H