#include "stdint.h"

#define __IO volatile
#define PERIPH_BASE 0x40000000U
#define AHB1PERIPH_BASE (PERIPH_BASE + 0x00020000U)
#define RCC_BASE (AHB1PERIPH_BASE + 0x00003800U)
#define GPIOD_BASE (AHB1PERIPH_BASE + 0x00000C00U)

#define RCC ((RCC_TypeDef *)RCC_BASE)
#define GPIOD ((GPIO_TypeDef *)GPIOD_BASE)

#define MODER_5_OUT (1 << (15 * 2)) // Set PD15 to output mode (01)
#define LED_PIN (1 << 15)        // Pin 15

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

void GPIO_Init(void);
void GPIO_TogglePin(void);
void delay(volatile int count);