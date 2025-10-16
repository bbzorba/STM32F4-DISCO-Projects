#include "servo.h"

static uint8_t current_angle = SERVO_DEFAULT_ANGLE;
static uint8_t is_running = 0;

// TIM9 definitions for STM32F407 (APB2)
// TIM9 base and RCC enable for STM32F407 (APB2)
#define TIM_9 ((TIM_TypeDef *)TIM9_BASE)
#define RCC_APB2ENR_TIM9EN ((uint32_t)(1U << 16))

// Map 0..180 deg to 1.0ms..2.0ms at 50 Hz
// Using TIM9 with PSC=1599 => 10 kHz tick (0.1 ms); ARR=200 => 20 ms period
static inline uint32_t servo_angle_to_ticks(uint8_t angle)
{
    // ticks = 10 + angle*(10/180) in 0.1ms units
    return 10U + ((uint32_t)angle * 10U) / 180U;
}

void Servo_Init(void)
{
    // Enable GPIOE clock
    __LIB_RCC_GPIOE_CLK_ENABLE();
    
    // Configure PE5 as AF mode (AF3 for TIM9_CH1)
    GPIO_E->MODER &= ~(0x3U << (5U * 2U));
    GPIO_E->MODER |=  (0x2U << (5U * 2U));
    // Optional: no pull, medium speed
    GPIO_E->PUPDR &= ~(0x3U << (5U * 2U));
    GPIO_E->OSPEEDR |= (0x1U << (5U * 2U));
    // AFRL for pin 5
    GPIO_E->AFR[0] &= ~(0xFU << (5U * 4U));
    GPIO_E->AFR[0] |=  (0x3U << (5U * 4U)); // AF3

    // Enable TIM9 clock
    RCC->RCC_APB2ENR |= RCC_APB2ENR_TIM9EN;

    // Time base: 50Hz using 10kHz tick: PSC=1599 (16MHz/(1599+1)=10kHz), ARR=200 (20ms)
    TIM_9->TIM_PSC = 1599U;
    TIM_9->TIM_ARR = 200U;

    // PWM mode 1 on CH1, preload enable
    TIM_9->TIM_CCMR1 &= ~(0x7U << 4);
    TIM_9->TIM_CCMR1 |= TIM_CCMR1_OC1M_PWM1; // 110b at OC1M
    TIM_9->TIM_CCMR1 |= (1U << 3);           // OC1PE

    // Enable channel and set active high
    TIM_9->TIM_CCER &= ~TIM_CCER_CC1P;
    TIM_9->TIM_CCER |= TIM_CCER_CC1E;

    // Enable ARR preload and generate update
    TIM_9->TIM_CR1 |= (1U << 7); // ARPE
    TIM_9->TIM_EGR |= TIM_EGR_UG;

    // Set default pulse corresponding to current_angle
    TIM_9->TIM_CCR1 = servo_angle_to_ticks(current_angle);
}


void Servo_SetAngle(uint8_t angle) {
    if (angle < SERVO_MIN_ANGLE || angle > SERVO_MAX_ANGLE) {
        // Invalid angle, do nothing or handle error
        return;
    }
    current_angle = angle;
    // Update PWM signal to set servo position (stub implementation)
    if (is_running) {
        TIM_9->TIM_CCR1 = servo_angle_to_ticks(angle);
    }
}

uint8_t Servo_GetAngle(void) {
    return current_angle;
}

void Servo_Start(void) {
    is_running = 1;
    // Start PWM signal (stub implementation)
    TIM_9->TIM_CCR1 = servo_angle_to_ticks(current_angle);
    TIM_9->TIM_CR1 |= TIM_CR1_CEN;
}

void Servo_Stop(void) {
    is_running = 0;
    // Stop PWM signal (stub implementation)
    TIM_9->TIM_CR1 &= ~TIM_CR1_CEN;
}
