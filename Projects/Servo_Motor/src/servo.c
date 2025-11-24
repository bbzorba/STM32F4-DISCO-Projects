#include "../inc/servo.h"

void servo_constructor(Servo *servoMotor,
                       servo_Type type,
                       servoAngle_Type initial_angle,
                       TIM_TypeDef *TIMx,
                       GPIO_ManualTypeDef *GPIO_regs,
                       GPIO_HandleTypeDef *GPIOx,
                       RCC_TypeDef *rcc,
                       uint8_t pinNumber,
                       uint8_t afNumber,
                       uint32_t gpioEnableMask)
{
    if (!servoMotor) return;
    servoMotor->type = type;
    servoMotor->angle = initial_angle;
    servoMotor->is_running = 0;
    servoMotor->TIMx = TIMx;
    servoMotor->GPIO_regs = GPIO_regs;
    servoMotor->GPIOx = GPIOx;
    servoMotor->rcc = rcc;
    servoMotor->pinNumber = pinNumber;
    servoMotor->afNumber = afNumber;
    servoMotor->gpioEnableMask = gpioEnableMask;

    // Configure hardware
    Servo_GPIO_Init(servoMotor, GPIOx);
    Servo_PWM_Init(servoMotor);
    // Program initial angle into CCR
    servoMotor->TIMx->TIM_CCR1 = servo_angle_to_ticks(servoMotor, initial_angle);
}

// Generic GPIO init for any PWM-capable pin: pass port, RCC pointer, pin number [0..15], AF number [0..15], and AHB1 enable mask (e.g., GPIOE_EN)
void Servo_GPIO_Init(Servo *servoMotor, GPIO_HandleTypeDef *GPIOx)
{
    if (!servoMotor) return;
    // Prepare init struct and delegate to generic GPIO driver
    GPIO_InitTypeDef init;
    init.Pin       = (1U << servoMotor->pinNumber); // single pin mask
    init.Mode      = GPIO_MODE_AF_PP;               // alternate function push-pull
    init.Pull      = GPIO_NOPULL;                   // no pull resistors
    init.Speed     = GPIO_SPEED_MEDIUM;             // medium speed adequate for PWM
    init.Alternate = servoMotor->afNumber & 0xFU;   // AF index

    // Driver call handles clock enable + configuration
    GPIO_Init(GPIOx, servoMotor->GPIO_regs, &init);
}

void Servo_PWM_Init(Servo *servoMotor) {
    // Enable timer clock and configure PWM for 50Hz (PSC=1599, ARR=200)
    Timer_Init(servoMotor->TIMx, servoMotor->rcc);
    Configure_PWM(servoMotor->TIMx, PWM_PRESCALER_1599U, 200U);
    // Set default pulse corresponding to current_angle
    servoMotor->TIMx->TIM_CCR1 = servo_angle_to_ticks(servoMotor, servoMotor->angle);
}

Servo_StatusType Servo_SetAngle(Servo *servoMotor, servoAngle_Type angle) {
    if (!servoMotor) return SERVO_ERROR_NOT_INITIALIZED;
    if (angle < SERVO_MIN_ANGLE || angle > SERVO_MAX_ANGLE) return SERVO_ERROR_INVALID_ANGLE;
    servoMotor->angle = angle;
    if (servoMotor->is_running) {
        servoMotor->TIMx->TIM_CCR1 = servo_angle_to_ticks(servoMotor, angle);
    }
    return SERVO_OK;
}

servoAngle_Type Servo_GetAngle(Servo *servoMotor) {
    if (!servoMotor) return SERVO_DEFAULT_ANGLE;
    return servoMotor->angle;
}

void Servo_Start(Servo *servoMotor) {
    if (!servoMotor) return;
    servoMotor->is_running = 1;
    servoMotor->TIMx->TIM_CCR1 = servo_angle_to_ticks(servoMotor, servoMotor->angle);
    servoMotor->TIMx->TIM_CR1 |= TIM_CR1_CEN;
}

void Servo_Stop(Servo *servoMotor) {
    if (!servoMotor) return;
    servoMotor->is_running = 0;
    servoMotor->TIMx->TIM_CR1 &= ~TIM_CR1_CEN;
}

uint32_t servo_angle_to_ticks(Servo *servoMotor, servoAngle_Type angle)
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