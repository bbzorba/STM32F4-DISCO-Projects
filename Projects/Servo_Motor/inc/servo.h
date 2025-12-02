#ifndef SERVO_H
#define SERVO_H

#include "../../../Drivers/GPIO/inc/gpio.h"
#include "../../../Drivers/PWM/inc/pwm.h"

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
typedef struct {
    servo_Type type;                  // 180 or 360 type
    servoAngle_Type angle;            // current angle (0..180)
    uint8_t is_running;               // 0/1 running flag
    GPIO_ManualTypeDef *GPIO_regs; // GPIO register base
    GPIO_InitTypeDef *GPIOx_Init;   // GPIO initialization structure
    RCC_TypeDef *rcc;                 // RCC pointer
    uint8_t pinNumber;                // GPIO pin index 0..15
    uint8_t afNumber;                 // AF 0..15
} Servo_HandleType;

void servo_constructor(Servo_HandleType *servoMotor,
                       servo_Type type,
                       servoAngle_Type initial_angle,
                       RCC_TypeDef *rcc,
                       uint8_t pinNumber,
                       uint8_t afNumber,
                       GPIO_ManualTypeDef *GPIO_regs,
                       GPIO_InitTypeDef *GPIOx_Init,
                       PWM_HandleType* pwmHandle,
                       PWM_Channel_TypeDef channel,
                       dutyCycle_TypeDef dutyCycle,
                       PWM_Prescaler_TypeDef prescaler,
                       uint32_t arr,
                       TIM_Type *TIMx);

// Object-oriented methods
Servo_StatusType Servo_SetAngle(Servo_HandleType *servoMotor, PWM_HandleType* pwmHandle, servoAngle_Type angle);
servoAngle_Type Servo_GetAngle(Servo_HandleType *servoMotor);
void Servo_Start(Servo_HandleType *servoMotor, PWM_HandleType* pwmHandle);
void Servo_Stop(Servo_HandleType *servoMotor, PWM_HandleType* pwmHandle);
uint32_t servo_angle_to_ticks(Servo_HandleType *servoMotor, servoAngle_Type angle);
// Generic GPIO/PWM helpers (procedural, kept for compatibility)
void Servo_PWM_Init(Servo_HandleType *servoMotor, PWM_HandleType* pwmHandle);

#endif // SERVO_H