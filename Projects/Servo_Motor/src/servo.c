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

void Servo_GPIO_Init(GPIO_TypeDef *GPIOx, RCC_TypeDef *rcc, uint8_t pinNumber, uint8_t afNumber, uint32_t gpioEnableMask)
{
    // Enable GPIO port clock (AHB1)
    rcc->RCC_AHB1ENR |= gpioEnableMask;

    // Configure pin as Alternate Function
    uint32_t pin = (uint32_t)pinNumber & 0xFU;
    uint32_t af  = (uint32_t)afNumber & 0xFU;

    GPIOx->MODER &= ~(0x3U << (pin * 2U));   
    GPIOx->MODER |=  (0x2U << (pin * 2U));   // MODER: set to 10b for AF mode
    
    GPIOx->PUPDR &= ~(0x3U << (pin * 2U));   // PUPDR: no pull
    
    GPIOx->OSPEEDR &= ~(0x3U << (pin * 2U)); // OSPEEDR: medium speed
    GPIOx->OSPEEDR |=  (0x1U << (pin * 2U));

    // AFR: select AFRL for pins 0..7, AFRH for 8..15
    if (pin < 8U) {
        GPIOx->AFR[0] &= ~(0xFU << (pin * 4U));
        GPIOx->AFR[0] |=  (af   << (pin * 4U));
    } else {
        uint32_t pinH = pin - 8U;
        GPIOx->AFR[1] &= ~(0xFU << (pinH * 4U));
        GPIOx->AFR[1] |=  (af   << (pinH * 4U));
    }
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
