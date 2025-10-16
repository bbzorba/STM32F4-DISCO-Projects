#ifndef __PWM_H
#define __PWM_H

#include "stdint.h"

#include "../../GPIO/inc/gpio.h"
#include <stdint.h>

#define __IO volatile

// Base addresses
#define PERIPH_ADDR_BASE 0x40000000U
#define APB1PERIPH_ADDR_BASE (PERIPH_ADDR_BASE + 0x00000000U)
#define APB2PERIPH_ADDR_BASE (PERIPH_ADDR_BASE + 0x00010000U)
#define TIM1_BASE (APB2PERIPH_ADDR_BASE + 0x00000000U)
#define TIM2_BASE (APB1PERIPH_ADDR_BASE + 0x00000000U)
#define TIM3_BASE (APB1PERIPH_ADDR_BASE + 0x00000400U)
#define TIM4_BASE (APB1PERIPH_ADDR_BASE + 0x00000800U)
#define TIM5_BASE (APB1PERIPH_ADDR_BASE + 0x00000C00U)
#define TIM9_BASE (APB2PERIPH_ADDR_BASE + 0x00004000U)

// RCC register bit definitions
#define RCC_APB2ENR_TIM1EN ((uint32_t)(1U << 0)) // TIM1 clock enable (APB2)
#define RCC_APB1ENR_TIM2EN ((uint32_t)(1U << 0)) // TIM2 clock enable (APB1)
#define RCC_APB1ENR_TIM3EN ((uint32_t)(1U << 1)) // TIM3 clock enable (APB1)
#define RCC_APB1ENR_TIM4EN ((uint32_t)(1U << 2)) // TIM4 clock enable (APB1)
#define RCC_APB1ENR_TIM5EN ((uint32_t)(1U << 3)) // TIM5 clock enable (APB1)
#define RCC_APB2ENR_TIM9EN ((uint32_t)(1U << 16)) // TIM9 clock enable (APB2)

// TIM register bit definitions
#define TIM_CR1_CEN ((uint16_t)0x0001) // Counter enable
#define TIM_CCER_CC1E        ((uint16_t)1<<0) // Capture/Compare 1 output enable
#define TIM_CCER_CC1P        ((uint16_t)1<<1) // Capture/Compare 1 output polarity
#define TIM_CCMR1_OC1M_PWM1  ((uint16_t)(6<<4)) // PWM mode 1 on OC1 (110b)
#define TIM_CCMR1_OC1PE      ((uint16_t)(0x0008 << 4)) // Output Compare 1 preload enable
#define TIM_EGR_UG           ((uint16_t)(1<<0)) // Update generation
#define TIM_BDTR_MOE         ((uint16_t)(1<<7)) // Main output enable (advanced timers)
#define TIM_CCMR1_OC2M_PWM1  ((uint16_t)(6<<12)) // PWM mode 1 on OC2 (110b)
#define TIM_CCMR1_OC2PE      ((uint16_t)(0x0008 << 12)) // Output Compare 2 preload enable
#define TIM_CR1_ARPE        ((uint16_t)(1<<7)) // Auto-reload preload enable

typedef struct
{ 
    __IO uint32_t TIM_CR1;   // Control register 1
    __IO uint32_t TIM_CR2;   // Control register 2
    __IO uint32_t TIM_SMCR;  // Slave mode control register
    __IO uint32_t TIM_DIER;  // DMA/Interrupt enable register
    __IO uint32_t TIM_SR;    // Status register
    __IO uint32_t TIM_EGR;   // Event generation register
    __IO uint32_t TIM_CCMR1; // Capture/Compare mode register 1
    __IO uint32_t TIM_CCMR2; // Capture/Compare mode register 2
    __IO uint32_t TIM_CCER;  // Capture/Compare enable register
    __IO uint32_t TIM_CNT;   // Counter
    __IO uint32_t TIM_PSC;   // Prescaler
    __IO uint32_t TIM_ARR;   // Auto-reload register
    __IO uint32_t TIM_RCR;   // Repetition counter register
    __IO uint32_t TIM_CCR1;  // Capture/Compare register 1
    __IO uint32_t TIM_CCR2;  // Capture/Compare register 2
    __IO uint32_t TIM_CCR3;  // Capture/Compare register 3
    __IO uint32_t TIM_CCR4;  // Capture/Compare register 4
    __IO uint32_t TIM_BDTR;  // Break and dead-time register
    __IO uint32_t TIM_DCR;   // DMA control register
    __IO uint32_t TIM_DMAR;  // DMA address for full transfer
}TIM_TypeDef;

typedef enum {
    CLOCK_DIVIDER_1 = 0,
    CLOCK_DIVIDER_2,
    CLOCK_DIVIDER_4
} Clock_Divider_TypeDef;

typedef enum {
    PWM_CHANNEL_1 = 1,
    PWM_CHANNEL_2,
    PWM_CHANNEL_3,
    PWM_CHANNEL_4
} PWM_Channel_TypeDef;

typedef enum {
    PWM_OK = 0,
    PWM_ERROR
} PWM_Status_TypeDef;

typedef enum {
    PWM_MODE_1 = 0,
    PWM_MODE_2
} PWM_Mode_TypeDef;

typedef enum {
    PWM_POLARITY_HIGH = 0,
    PWM_POLARITY_LOW
} PWM_Polarity_TypeDef;

typedef enum {
    PWM_STATE_DISABLE = 0,
    PWM_STATE_ENABLE
} PWM_State_TypeDef;

// Peripheral declarations
#define TIM_1 ((TIM_TypeDef *)TIM1_BASE)
#define TIM_9 ((TIM_TypeDef *)TIM9_BASE)

// Function prototypes (generic per-timer)
void PWM_Init(TIM_TypeDef *TIMx, RCC_TypeDef *rcc);
void PWM_SetDutyCycle(TIM_TypeDef *TIMx, int channel, uint32_t duty_cycle);
uint32_t PWM_GetDutyCycle(TIM_TypeDef *TIMx, int channel);

#endif // __PWM_H