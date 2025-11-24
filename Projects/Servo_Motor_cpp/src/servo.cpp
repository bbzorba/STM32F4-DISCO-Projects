#include "../inc/servo.h"

Servo::Servo(servo_Type type,
                         servoAngle_Type initial_angle,
                         TIM_TypeDef *TIMx,
                         GPIO_ManualTypeDef *GPIO_regs,
                         RCC_TypeDef *rcc,
                         uint8_t pinNumber,
                         uint8_t afNumber,
                         uint32_t gpioEnableMask)
        : type(type),
            angle(initial_angle),
            is_running(0),
            TIMx(TIMx),
            GPIO_regs(GPIO_regs),
            rcc(rcc),
            pinNumber(pinNumber),
            afNumber(afNumber),
            gpioEnableMask(gpioEnableMask),
            pwm(TIMx, PWM_PRESCALER_1599U, 200U) // construct PWM instance
{
    // Configure hardware
    GPIO_Init();
    PWM_Init();
    // Program initial angle into CCR
    this->TIMx->TIM_CCR1 = angle_to_ticks(initial_angle);
}

// Configure GPIO for the servo PWM pin using C++ GPIO driver
void Servo::GPIO_Init() {
    GPIO_InitTypeDef init;
    init.Pin       = (1U << this->pinNumber);      // single pin mask
    init.Mode      = GPIO_MODE_AF_PP;              // alternate function push-pull
    init.Pull      = GPIO_NOPULL;                  // no pull resistors
    init.Speed     = GPIO_SPEED_MEDIUM;            // medium speed adequate for PWM
    init.Alternate = (this->afNumber & 0xFU);      // AF index
    GPIO gpio_local(this->GPIO_regs, &init);       // configure via constructor (scope local)
    (void)gpio_local;                              // suppress unused warning
}

void Servo::PWM_Init() {
    // Enable timer clock and configure PWM for 50Hz (PSC=1599, ARR=200)
    pwm.Timer_Init(this->TIMx, this->rcc);
    pwm.Configure_PWM(this->TIMx, PWM_PRESCALER_1599U, 200U);
    // Set default pulse corresponding to current_angle
    this->TIMx->TIM_CCR1 = angle_to_ticks(this->angle);
}

Servo_StatusType Servo::SetAngle(servoAngle_Type angle) {
    if (angle < SERVO_MIN_ANGLE || angle > SERVO_MAX_ANGLE) return SERVO_ERROR_INVALID_ANGLE;
    this->angle = angle;
    if (this->is_running) {
    this->TIMx->TIM_CCR1 = angle_to_ticks(angle);
    }
    return SERVO_OK;
}

servoAngle_Type Servo::GetAngle() {
    return this->angle;
}

void Servo::Start() {
    this->is_running = 1;
    this->TIMx->TIM_CCR1 = angle_to_ticks(this->angle);
    this->TIMx->TIM_CR1 |= TIM_CR1_CEN;
}

void Servo::Stop() {
    this->is_running = 0;
    this->TIMx->TIM_CR1 &= ~TIM_CR1_CEN;
}

uint32_t Servo::angle_to_ticks(servoAngle_Type angle)
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