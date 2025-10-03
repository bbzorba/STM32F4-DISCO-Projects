// GPIO header file for STM32F4xx microcontrollers
#include "stdint.h"

#ifndef __GPIO_H
#define __GPIO_H

// Base addresses
#define __IO volatile
#define PERIPH_BASE 0x40000000U
#define AHB1PERIPH_BASE (PERIPH_BASE + 0x00020000U)
#define RCC_BASE (AHB1PERIPH_BASE + 0x00003800U)
#define GPIOA_BASE (AHB1PERIPH_BASE + 0x00000000U)
#define GPIOB_BASE (AHB1PERIPH_BASE + 0x00000400U)
#define GPIOC_BASE (AHB1PERIPH_BASE + 0x00000800U)
#define GPIOD_BASE (AHB1PERIPH_BASE + 0x00000C00U)
#define GPIOE_BASE (AHB1PERIPH_BASE + 0x00001000U)
#define GPIOF_BASE (AHB1PERIPH_BASE + 0x00001400U)
#define GPIOG_BASE (AHB1PERIPH_BASE + 0x00001800U)
#define GPIOH_BASE (AHB1PERIPH_BASE + 0x00001C00U)
#define GPIOI_BASE (AHB1PERIPH_BASE + 0x00002000U)

// Peripheral declarations
#define RCC ((RCC_TypeDef *)RCC_BASE)
#define GPIOA ((GPIO_TypeDef *)GPIOA_BASE)
#define GPIOB ((GPIO_TypeDef *)GPIOB_BASE)
#define GPIOC ((GPIO_TypeDef *)GPIOC_BASE)
#define GPIOD ((GPIO_TypeDef *)GPIOD_BASE)

// Enable bits for GPIO ports in RCC AHB1ENR register
#define GPIOA_EN (1 << 0) // Bit 0
#define GPIOB_EN (1 << 1) // Bit 1
#define GPIOC_EN (1 << 2) // Bit 2
#define GPIOD_EN (1 << 3) // Bit 3
#define GPIOE_EN (1 << 4) // Bit 4
#define GPIOF_EN (1 << 5) // Bit 5
#define GPIOG_EN (1 << 6) // Bit 6
#define GPIOH_EN (1 << 7) // Bit 7
#define GPIOI_EN (1 << 8) // Bit 8

// GPIO port mode enumeration
typedef struct {
    __IO uint32_t MODER;    // GPIO port mode register
    __IO uint32_t OTYPER;   // GPIO port output type register
    __IO uint32_t OSPEEDR;  // GPIO port output speed register
    __IO uint32_t PUPDR;    // GPIO port pull-up/pull-down register
    __IO uint32_t IDR;      // GPIO port input data register
    __IO uint32_t ODR;      // GPIO port output data register
    __IO uint32_t BSRR;     // GPIO port bit set/reset register
    __IO uint32_t LCKR;     // GPIO port configuration lock register
    __IO uint32_t AFR[2];   // GPIO alternate function registers
} GPIO_TypeDef;

// RCC register definition structure
typedef struct 
{
    __IO uint32_t CR;              // RCC clock control register
    __IO uint32_t PLLCFGR;         // RCC PLL configuration register
    __IO uint32_t RCC_CFGR;        // RCC clock configuration register
    __IO uint32_t RCC_CIR;         // RCC clock interrupt register
    __IO uint32_t RCC_AHB1RSTR;    // RCC AHB1 peripheral reset register
    __IO uint32_t RCC_AHB2RSTR;    // RCC AHB2 peripheral reset register
    __IO uint32_t RCC_AHB3RSTR;    // RCC AHB3 peripheral reset register
         uint32_t RESERVED0;       // Reserved
    __IO uint32_t RCC_APB1RSTR;    // RCC APB1 peripheral reset register
    __IO uint32_t RCC_APB2RSTR;    // RCC APB2 peripheral reset register
         uint32_t RESERVED1[2];    // Reserved
    __IO uint32_t RCC_AHB1ENR;     // RCC AHB1 peripheral clock enable register
    __IO uint32_t RCC_AHB2ENR;     // RCC AHB2 peripheral clock enable register
    __IO uint32_t RCC_AHB3ENR;     // RCC AHB3 peripheral clock enable register
         uint32_t RESERVED2;       // Reserved
    __IO uint32_t RCC_APB1ENR;     // RCC APB1 peripheral clock enable register
    __IO uint32_t RCC_APB2ENR;     // RCC APB2 peripheral clock enable register
         uint32_t RESERVED3[2];    // Reserved
    __IO uint32_t RCC_AHB1LPENR;   // RCC AHB1 peripheral clock enable in low power mode register
    __IO uint32_t RCC_AHB2LPENR;   // RCC AHB2 peripheral clock enable in low power mode register
    __IO uint32_t RCC_AHB3LPENR;   // RCC AHB3 peripheral clock enable in low power mode register
         uint32_t RESERVED4;       // Reserved
    __IO uint32_t RCC_APB1LPENR;   // RCC APB1 peripheral clock enable in low power mode register
    __IO uint32_t RCC_APB2LPENR;   // RCC APB2 peripheral clock enable in low power mode register
            uint32_t RESERVED5[2];    // Reserved
    __IO uint32_t RCC_BDCR;        // RCC Backup domain control register
    __IO uint32_t RCC_CSR;         // RCC clock control & status register
         uint32_t RESERVED6[2];    // Reserved
    __IO uint32_t RCC_SSCGR;       // RCC spread spectrum clock generation register
    __IO uint32_t RCC_PLLI2SCFGR;  // RCC PLLI2S configuration register
    __IO uint32_t RCC_PLLSAICFGR;  // RCC PLLSAI configuration register
    __IO uint32_t RCC_DCKCFGR;     // RCC Dedicated Clocks Configuration Register
} RCC_TypeDef;

// Function prototypes
void GPIOD_Init(void);
void delay(volatile int count);
void GPIOD_TogglePins(volatile int count);

#endif // __GPIO_H