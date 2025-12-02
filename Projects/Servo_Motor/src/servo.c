#include "../inc/servo.h"

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
                       TIM_Type *TIMx)
{
    if (!servoMotor) return;
    servoMotor->is_running = 0;
    servoMotor->type = type;
    servoMotor->angle = initial_angle;
    servoMotor->GPIO_regs = GPIO_regs;
    servoMotor->GPIOx_Init = GPIOx_Init;
    servoMotor->rcc = rcc;
    servoMotor->pinNumber = pinNumber;
    servoMotor->afNumber = afNumber;

    // Initialize PWM handle
    PWM_constructor(pwmHandle, channel, dutyCycle, prescaler, arr, TIMx);

    // Initialize GPIO for PWM output using proper handle
    if (GPIO_regs && GPIOx_Init) {
        GPIO_HandleTypeDef gpio_handle;
        gpio_handle.regs = GPIO_regs;
        gpio_handle.init = GPIOx_Init;
        GPIO_constructor(&gpio_handle, GPIO_regs, GPIOx_Init);
    }

    Servo_PWM_Init(servoMotor, pwmHandle);
    
    // Program initial angle into CCR
    pwmHandle->TIMx->TIM_CCR1 = servo_angle_to_ticks(servoMotor, initial_angle);
}

void Servo_PWM_Init(Servo_HandleType *servoMotor, PWM_HandleType* pwmHandle) {
    // Enable timer clock and configure PWM for 50Hz (PSC=1599, ARR=200)
    // Set default pulse corresponding to current_angle
    pwmHandle->TIMx->TIM_CCR1 = servo_angle_to_ticks(servoMotor, servoMotor->angle);
}

Servo_StatusType Servo_SetAngle(Servo_HandleType *servoMotor, PWM_HandleType* pwmHandle, servoAngle_Type angle) {
    if (!servoMotor) return SERVO_ERROR_NOT_INITIALIZED;
    if (angle < SERVO_MIN_ANGLE || angle > SERVO_MAX_ANGLE) return SERVO_ERROR_INVALID_ANGLE;
    servoMotor->angle = angle;
    if (servoMotor->is_running) {
        pwmHandle->TIMx->TIM_CCR1 = servo_angle_to_ticks(servoMotor, angle);
    }
    return SERVO_OK;
}

servoAngle_Type Servo_GetAngle(Servo_HandleType *servoMotor) {
    if (!servoMotor) return SERVO_DEFAULT_ANGLE;
    return servoMotor->angle;
}

void Servo_Start(Servo_HandleType *servoMotor, PWM_HandleType* pwmHandle) {
    if (!servoMotor) return;
    servoMotor->is_running = 1;
    pwmHandle->TIMx->TIM_CCR1 = servo_angle_to_ticks(servoMotor, servoMotor->angle);
    pwmHandle->TIMx->TIM_CR1 |= TIM_CR1_CEN;
}

void Servo_Stop(Servo_HandleType *servoMotor, PWM_HandleType* pwmHandle) {
    if (!servoMotor) return;
    servoMotor->is_running = 0;
    pwmHandle->TIMx->TIM_CR1 &= ~TIM_CR1_CEN;
}

uint32_t servo_angle_to_ticks(Servo_HandleType *servoMotor, servoAngle_Type angle)
{
    // 50 Hz period: ARR=200 at 10 kHz -> 1 tick = 0.1 ms
    // 180°: 0..180 -> 1.0..2.0 ms (10..20 ticks)
    // 360°: interpret "angle" as speed/direction around center (90 = stop)
    if (servoMotor->type == SERVO_360_TYPE) {
        int32_t delta = (int32_t)angle - 90; // -90..+90
        // optional deadband to reduce jitter around stop
        const int32_t deadband = 2; // ~0.11 ms window
        if (delta > -deadband && delta < deadband) {
            return 15U; // 1.5 ms stop
        }
        // scale -90..+90 to -5..+5 ticks around center 15
        // +/-5 ticks -> 1.0..2.0 ms
        int32_t offset = (delta * 5) / 90; // integer arithmetic
        int32_t ticks = 15 + offset;
        if (ticks < 10) ticks = 10;
        if (ticks > 20) ticks = 20;
        return (uint32_t)ticks;
    } else {
        // 180° positional servo
        return 10U + ((uint32_t)angle * 10U) / 180U;
    }
}