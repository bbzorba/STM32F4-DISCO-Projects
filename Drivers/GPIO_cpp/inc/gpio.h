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
#define GPIO_A ((GPIO_TypeDef *)GPIOA_BASE)
#define GPIO_B ((GPIO_TypeDef *)GPIOB_BASE)
#define GPIO_C ((GPIO_TypeDef *)GPIOC_BASE)
#define GPIO_D ((GPIO_TypeDef *)GPIOD_BASE)

// GPIO mode settings for specific pins
#define MODER_2_OUT (1 << (12 * 2)) // Set PD12 to output mode (01)
#define MODER_3_OUT (1 << (13 * 2)) // Set PD13 to output mode (01)
#define MODER_4_OUT (1 << (14 * 2)) // Set PD14 to output mode (01)
#define MODER_5_OUT (1 << (15 * 2)) // Set PD15 to output mode (01)

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

// GPIO pin definitions for ODR and IDR registers
#define GPIO_PIN_0  (1 << 0)  // Pin 0
#define GPIO_PIN_1  (1 << 1)  // Pin 1
#define GPIO_PIN_2  (1 << 2)  // Pin 2
#define GPIO_PIN_3  (1 << 3)  // Pin 3
#define GPIO_PIN_4  (1 << 4)  // Pin 4
#define GPIO_PIN_5  (1 << 5)  // Pin 5
#define GPIO_PIN_6  (1 << 6)  // Pin 6
#define GPIO_PIN_7  (1 << 7)  // Pin 7
#define GPIO_PIN_8  (1 << 8)  // Pin 8
#define GPIO_PIN_9  (1 << 9)  // Pin 9
#define GPIO_PIN_10 (1 << 10) // Pin 10
#define GPIO_PIN_11 (1 << 11) // Pin 11
#define GPIO_PIN_12 (1 << 12) // Pin 12
#define GPIO_PIN_13 (1 << 13) // Pin 13
#define GPIO_PIN_14 (1 << 14) // Pin 14
#define GPIO_PIN_15 (1 << 15) // Pin 15
#define GPIO_PIN_ALL (0xFFFF) // All pins

/* GPIO mode definitions for MODER register */
#define GPIO_MODE_INPUT 0x00U        // Input mode
#define GPIO_MODE_OUTPUT 0x01U       // Output mode
#define GPIO_MODE_ALTERNATE 0x02U    // Alternate function mode
#define GPIO_MODE_ANALOG 0x03U       // Analog mode

/* GPIO output type definitions for OTYPER register */
#define GPIO_OTYPE_PP 0x00U          // Push-pull
#define GPIO_OTYPE_OD 0x01U          // Open-drain

/* GPIO pull-up/pull-down definitions for PUPDR register */
#define GPIO_NOPULL 0x00U          // No pull-up, pull-down
#define GPIO_PULLUP 0x01U          // Pull-up
#define GPIO_PULLDOWN 0x02U        // Pull-down

/* Composite modes (preferred) */
#define GPIO_MODE_OUTPUT_PP    0x00000001U  /* MODER=01, OTYPER=0 */
#define GPIO_MODE_OUTPUT_OD    0x00000011U  /* MODER=01, OTYPER=1 */
#define GPIO_MODE_AF_PP        0x00000002U  /* MODER=10, OTYPER=0 */
#define GPIO_MODE_AF_OD        0x00000012U  /* MODER=10, OTYPER=1 */

typedef uint32_t pinDataType;

/* GPIO initialization structure */
typedef struct {
    uint32_t Pin;            // Specifies the GPIO pins to be configured.
    uint32_t Mode;           // Specifies the operating mode for the selected pins.
    uint32_t Pull;           // Specifies the Pull-up or Pull-down activation for the selected pins.
    uint32_t Speed;          // Specifies the speed for the selected pins.
    uint32_t Alternate;     // Specifies the alternate function for the selected pins.
} GPIO_InitTypeDef;

typedef enum {
    GPIO_PIN_RESET = 0,
    GPIO_PIN_SET
} GPIO_PinState;

typedef enum {
    PORT_A = 1,
    PORT_B,
    PORT_C,
    PORT_D,
    PORT_E,
    PORT_F,
    PORT_G,
    PORT_H
} port_NameType;

/* GPIO port mode enumeration */
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

/* RCC register definition structure */
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

class GPIO {
private:
    GPIO_TypeDef *GPIOx;
     GPIO_InitTypeDef *GPIO_Init;
    port_NameType portName;

public:
    GPIO(port_NameType port, GPIO_TypeDef *GPIOx, GPIO_InitTypeDef *GPIO_Init);
    void LIB_GPIO_TogglePin(uint16_t pin);
    void LIB_GPIO_WritePin(uint16_t pin, GPIO_PinState state);
    GPIO_PinState LIB_GPIO_ReadPin(uint16_t pin);

     /* RCC peripheral clock enable functions */
     void __LIB_RCC_GPIOA_CLK_ENABLE() const;
     void __LIB_RCC_GPIOB_CLK_ENABLE() const;
     void __LIB_RCC_GPIOC_CLK_ENABLE() const;
     void __LIB_RCC_GPIOD_CLK_ENABLE() const;
     void __LIB_RCC_GPIOE_CLK_ENABLE() const;
     void __LIB_RCC_GPIOF_CLK_ENABLE() const;
     void __LIB_RCC_GPIOG_CLK_ENABLE() const;
     void __LIB_RCC_GPIOH_CLK_ENABLE() const;
};

GPIO_InitTypeDef __GPIO_PIN_Params(pinDataType __Pin, pinDataType __Mode, pinDataType __Pull, pinDataType __Speed, pinDataType __Alternate);

#endif // __GPIO_H