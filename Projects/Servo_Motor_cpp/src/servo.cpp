#include "../inc/servo.h"

Servo::Servo(servo_Type type,
             servoAngle_Type initial_angle,
             RCC_TypeDef *rcc,
             uint8_t pinNumber,
             uint8_t afNumber,
                 GPIO_ManualTypeDef *GPIO_regs,
                 GPIO_InitTypeDef *GPIOx_Init,
                 PWM_Channel_TypeDef channel,
                 dutyCycle_TypeDef dutyCycle,
                 PWM_Prescaler_TypeDef prescaler,
                 uint32_t arr,
                 TIM_TypeDef *TIMx)
                     : pwm(channel, dutyCycle, prescaler, arr, TIMx)
{
    this->is_running = 0;
    this->type = type;
    this->angle = initial_angle;
    this->GPIO_regs = GPIO_regs;
    this->GPIOx_Init = GPIOx_Init;
    this->rcc = rcc;
    this->pinNumber = pinNumber;
    this->afNumber = afNumber;

    // Initialize GPIO for PWM output using proper handle
    if (GPIO_regs && GPIOx_Init) {
        GPIO gpio_handle(GPIO_regs, GPIOx_Init);
    }

    this->Servo_PWM_Init();
    
    // Program initial angle into CCR
    this->pwm.SetPulseTicks(servo_angle_to_ticks(initial_angle));
}

void Servo::Servo_PWM_Init() {
    // Enable timer clock and configure PWM for 50Hz (PSC=1599, ARR=200)
    // Set default pulse corresponding to current_angle
    this->pwm.SetPulseTicks(servo_angle_to_ticks(this->angle));
}

Servo_StatusType Servo::Servo_SetAngle(servoAngle_Type angle) {
    if (angle < SERVO_MIN_ANGLE || angle > SERVO_MAX_ANGLE) return SERVO_ERROR_INVALID_ANGLE;
    this->angle = angle;
    if (this->is_running) {
        this->pwm.SetPulseTicks(servo_angle_to_ticks(angle));
    }
    return SERVO_OK;
}

servoAngle_Type Servo::Servo_GetAngle() {
    return this->angle;
}

void Servo::Servo_Start() {
    this->is_running = 1;
    this->pwm.SetPulseTicks(servo_angle_to_ticks(this->angle));
    this->pwm.Start();
}

void Servo::Servo_Stop() {
    this->is_running = 0;
    this->pwm.Stop();
}

uint32_t Servo::servo_angle_to_ticks(servoAngle_Type angle)
{
    // 50 Hz period: ARR=200 at 10 kHz -> 1 tick = 0.1 ms
    // 180°: 0..180 -> 1.0..2.0 ms (10..20 ticks)
    // 360°: interpret "angle" as speed/direction around center (90 = stop)
    if (this->type == SERVO_360_TYPE) {
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