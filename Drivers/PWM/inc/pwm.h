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

// RCC register bit definitions
#define RCC_APB2ENR_TIM1EN ((uint32_t)1 << 0) // Bit 0
#define TIM_CR1_CEN ((uint16_t)0x0001) // Counter enable
#define TIM_CCER_CC1E        ((uint16_t)0x0001) // Capture/Compare 1 output enable
#define TIM_CCER_CC1P        ((uint16_t)0x0002) // Capture/Compare 1 output polarity
#define TIM_CCMR1_OC1M_PWM1  ((uint16_t)0x0060) // PWM mode 1 on OC1 (110b)
#define TIM_CCMR1_OC1PE      ((uint16_t)0x0008) // Output Compare 1 preload enable
#define TIM_EGR_UG           ((uint16_t)0x0001) // Update generation
#define TIM_BDTR_MOE         ((uint16_t)0x8000) // Main output enable (advanced timers)

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

// Function prototypes
void PWM_Init(void);
void PWM_SetDutyCycle(int channel, uint32_t duty_cycle);
uint32_t PWM_GetDutyCycle(int channel);

#endif // __PWM_H