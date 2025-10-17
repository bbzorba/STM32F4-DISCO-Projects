#include "servo.h"

static uint8_t current_angle = SERVO_DEFAULT_ANGLE;
static uint8_t is_running = 0;

// Map 0..180 deg to 1.0ms..2.0ms at 50 Hz
// Using TIM9 with PSC=1599 => 10 kHz tick (0.1 ms); ARR=200 => 20 ms period
static inline uint32_t servo_angle_to_ticks(uint8_t angle)
{
    // ticks = 10 + angle*(10/180) in 0.1ms units
    return 10U + ((uint32_t)angle * 10U) / 180U;
}

void Servo_GPIO_Init(GPIO_TypeDef *GPIOx)
{
    // Enable GPIOE clock
    __LIB_RCC_GPIOE_CLK_ENABLE();

    // Configure PE5 as AF mode (AF3 for TIM9_CH1)
    GPIOx->MODER &= ~(0x3U << (5U * 2U));
    GPIOx->MODER |=  (0x2U << (5U * 2U));
    // Optional: no pull, medium speed
    GPIOx->PUPDR &= ~(0x3U << (5U * 2U));
    GPIOx->OSPEEDR |= (0x1U << (5U * 2U));
    // AFRL for pin 5
    GPIOx->AFR[0] &= ~(0xFU << (5U * 4U));
    GPIOx->AFR[0] |=  (0x3U << (5U * 4U)); // AF3
}

void Servo_PWM_Init(TIM_TypeDef *TIMx, RCC_TypeDef *rcc) {
    // Enable timer clock and configure PWM for 50Hz (PSC=1599, ARR=200)
    Timer_Init(TIMx, rcc);
    Configure_PWM(TIMx, 1599U, 200U);
    // Set default pulse corresponding to current_angle
    TIMx->TIM_CCR1 = servo_angle_to_ticks(current_angle);
}


void Servo_SetAngle(uint8_t angle, TIM_TypeDef *TIMx) {
    if (angle < SERVO_MIN_ANGLE || angle > SERVO_MAX_ANGLE) {
        // Invalid angle, do nothing or handle error
        return;
    }
    current_angle = angle;
    // Update PWM signal to set servo position (stub implementation)
    if (is_running) {
        TIMx->TIM_CCR1 = servo_angle_to_ticks(angle);
    }
}

uint8_t Servo_GetAngle(void) {
    return current_angle;
}

void Servo_Start(TIM_TypeDef *TIMx) {
    is_running = 1;
    // Start PWM signal (stub implementation)
    TIMx->TIM_CCR1 = servo_angle_to_ticks(current_angle);
    TIMx->TIM_CR1 |= TIM_CR1_CEN;
}

void Servo_Stop(TIM_TypeDef *TIMx) {
    is_running = 0;
    // Stop PWM signal (stub implementation)
    TIMx->TIM_CR1 &= ~TIM_CR1_CEN;
}
