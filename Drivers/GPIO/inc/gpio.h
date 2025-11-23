// GPIO header file for STM32F4xx microcontrollers
#include <stdint.h>

#ifndef __GPIO_H
#define __GPIO_H

///////////////////////////////////////GPIO ADDRESS DEFINITIONS//////////////////////////////////////////////////
/* GPIO initialization structure */
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
///////////////////////////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////REGISTER BIT DEFINITIONS/////////////////////////////////////////////////
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

// RCC register bit definitions
#define RCC_AHB1ENR_GPIOAEN  ((uint32_t)0x00000001)         // Bit 0
#define RCC_AHB1ENR_GPIOBEN  ((uint32_t)0x00000002)         // Bit 1
#define RCC_AHB1ENR_GPIOCEN  ((uint32_t)0x00000004)         // Bit 2
#define RCC_AHB1ENR_GPIODEN  ((uint32_t)0x00000008)         // Bit 3
#define RCC_AHB1ENR_GPIOEEN  ((uint32_t)0x00000010)         // Bit 4

// GPIO MODER register Mask bit definitions
#define MODER_PIN0_MASK          ((uint32_t)0x00000003)        // mask to clear MODER_PIN0 bits (bits 1:0)
#define MODER_PIN1_MASK          ((uint32_t)0x0000000C)        // mask to clear MODER_PIN1 bits (bits 3:2)
#define MODER_PIN2_MASK          ((uint32_t)0x00000030)        // mask to clear MODER_PIN2 bits (bits 5:4)
#define MODER_PIN3_MASK          ((uint32_t)0x000000C0)        // mask to clear MODER_PIN3 bits (bits 7:6)
#define MODER_PIN4_MASK          ((uint32_t)0x00000300)        // mask to clear MODER_PIN4 bits (bits 9:8)
#define MODER_PIN5_MASK          ((uint32_t)0x00000C00)        // mask to clear MODER_PIN5 bits (bits 11:10)
#define MODER_PIN6_MASK          ((uint32_t)0x00003000)        // mask to clear MODER_PIN6 bits (bits 13:12)
#define MODER_PIN7_MASK          ((uint32_t)0x0000C000)        // mask to clear MODER_PIN7 bits (bits 15:14)
#define MODER_PIN8_MASK          ((uint32_t)0x00030000)        // mask to clear MODER_PIN8 bits (bits 17:16)
#define MODER_PIN9_MASK          ((uint32_t)0x000C0000)        // mask to clear MODER_PIN9 bits (bits 19:18)
#define MODER_PIN10_MASK         ((uint32_t)0x00300000)        // mask to clear MODER_PIN10 bits (bits 21:20)
#define MODER_PIN11_MASK         ((uint32_t)0x00C00000)        // mask to clear MODER_PIN11 bits (bits 23:22)
#define MODER_PIN12_MASK         ((uint32_t)0x03000000)        // mask to clear MODER_PIN12 bits (bits 25:24)
#define MODER_PIN13_MASK         ((uint32_t)0x0C000000)        // mask to clear MODER_PIN13 bits (bits 27:26)
#define MODER_PIN14_MASK         ((uint32_t)0x30000000)        // mask to clear MODER_PIN14 bits (bits 29:28)
#define MODER_PIN15_MASK         ((uint32_t)0xC0000000)        // mask to clear MODER_PIN15 bits (bits 31:30)

// GPIO MODER register Set bit definitions for Alternate Function mode (10b)
#define MODER_PIN0_SET           ((uint32_t)0x00000002)        // set AF for PIN0 (bits 1:0)
#define MODER_PIN1_SET           ((uint32_t)0x00000008)        // set AF for PIN1 (bits 3:2)
#define MODER_PIN2_SET           ((uint32_t)0x00000020)        // set AF for PIN2 (bits 5:4)
#define MODER_PIN3_SET           ((uint32_t)0x00000080)        // set AF for PIN3 (bits 7:6)
#define MODER_PIN4_SET           ((uint32_t)0x00000200)        // set AF for PIN4 (bits 9:8)
#define MODER_PIN5_SET           ((uint32_t)0x00000800)        // set AF for PIN5 (bits 11:10)
#define MODER_PIN6_SET           ((uint32_t)0x00002000)        // set AF for PIN6 (bits 13:12)
#define MODER_PIN7_SET           ((uint32_t)0x00008000)        // set AF for PIN7 (bits 15:14)
#define MODER_PIN8_SET           ((uint32_t)0x00020000)        // set AF for PIN8 (bits 17:16)
#define MODER_PIN9_SET           ((uint32_t)0x00080000)        // set AF for PIN9 (bits 19:18)
#define MODER_PIN10_SET          ((uint32_t)0x00200000)        // set AF for PIN10 (bits 21:20)
#define MODER_PIN11_SET          ((uint32_t)0x00800000)        // set AF for PIN11 (bits 23:22)
#define MODER_PIN12_SET          ((uint32_t)0x02000000)        // set AF for PIN12 (bits 25:24)
#define MODER_PIN13_SET          ((uint32_t)0x08000000)        // set AF for PIN13 (bits 27:26)
#define MODER_PIN14_SET          ((uint32_t)0x20000000)        // set AF for PIN14 (bits 29:28)
#define MODER_PIN15_SET          ((uint32_t)0x80000000)        // set AF for PIN15 (bits 31:30)

// GPIO AFRL register Mask bit definitions
#define AFRL_PIN0_MASK           ((uint32_t)0x0000000F)        // mask to clear AFRL_PIN0 bits (bits 3:0)
#define AFRL_PIN1_MASK           ((uint32_t)0x000000F0)        // mask to clear AFRL_PIN1 bits (bits 7:4)
#define AFRL_PIN2_MASK           ((uint32_t)0x00000F00)        // mask to clear AFRL_PIN2 bits (bits 11:8)
#define AFRL_PIN3_MASK           ((uint32_t)0x0000F000)        // mask to clear AFRL_PIN3 bits (bits 15:12)
#define AFRL_PIN4_MASK           ((uint32_t)0x000F0000)        // mask to clear AFRL_PIN4 bits (bits 19:16)
#define AFRL_PIN5_MASK           ((uint32_t)0x00F00000)        // mask to clear AFRL_PIN5 bits (bits 23:20)
#define AFRL_PIN6_MASK           ((uint32_t)0x0F000000)        // mask to clear AFRL_PIN6 bits (bits 27:24)
#define AFRL_PIN7_MASK           ((uint32_t)0xF0000000)        // mask to clear AFRL_PIN7 bits (bits 31:28)
#define AFRH_PIN8_MASK           ((uint32_t)0x0000000F)        // mask to clear AFRH_PIN8 bits (bits 3:0)
#define AFRH_PIN9_MASK           ((uint32_t)0x000000F0)        // mask to clear AFRH_PIN9 bits (bits 7:4)
#define AFRH_PIN10_MASK          ((uint32_t)0x00000F00)        // mask to clear AFRH_PIN10 bits (bits 11:8)
#define AFRH_PIN11_MASK          ((uint32_t)0x0000F000)        // mask to clear AFRH_PIN11 bits (bits 15:12)
#define AFRH_PIN12_MASK          ((uint32_t)0x000F0000)        // mask to clear AFRH_PIN12 bits (bits 19:16)
#define AFRH_PIN13_MASK          ((uint32_t)0x00F00000)        // mask to clear AFRH_PIN13 bits (bits 23:20)
#define AFRH_PIN14_MASK          ((uint32_t)0x0F000000)        // mask to clear AFRH_PIN14 bits (bits 27:24)
#define AFRH_PIN15_MASK          ((uint32_t)0xF0000000)        // mask to clear AFRH_PIN15 bits (bits 31:28)

//GPIO AFR register Set bit definitions for AF5 (0101b) [SPI1, SPI2]
#define AFRL_PIN0_SET_AF5        ((uint32_t)0x00000005)        // set AF5 for PIN0 (bits 3:0)
#define AFRL_PIN1_SET_AF5        ((uint32_t)0x00000050)        // set AF5 for PIN1 (bits 7:4)
#define AFRL_PIN2_SET_AF5        ((uint32_t)0x00000500)        // set AF5 for PIN2 (bits 11:8)
#define AFRL_PIN3_SET_AF5        ((uint32_t)0x00005000)        // set AF5 for PIN3 (bits 15:12)
#define AFRL_PIN4_SET_AF5        ((uint32_t)0x00050000)        // set AF5 for PIN4 (bits 19:16)
#define AFRL_PIN5_SET_AF5        ((uint32_t)0x00500000)        // set AF5 for PIN5 (bits 23:20)
#define AFRL_PIN6_SET_AF5        ((uint32_t)0x05000000)        // set AF5 for PIN6 (bits 27:24)
#define AFRL_PIN7_SET_AF5        ((uint32_t)0x50000000)        // set AF5 for PIN7 (bits 31:28)
#define AFRH_PIN8_SET_AF5        ((uint32_t)0x00000005)        // set AF5 for PIN8 (bits 3:0)
#define AFRH_PIN9_SET_AF5        ((uint32_t)0x00000050)        // set AF5 for PIN9 (bits 7:4)
#define AFRH_PIN10_SET_AF5       ((uint32_t)0x00000500)        // set AF5 for PIN10 (bits 11:8)
#define AFRH_PIN11_SET_AF5       ((uint32_t)0x00005000)        // set AF5 for PIN11 (bits 15:12)
#define AFRH_PIN12_SET_AF5       ((uint32_t)0x00050000)        // set AF5 for PIN12 (bits 19:16)
#define AFRH_PIN13_SET_AF5       ((uint32_t)0x00500000)        // set AF5 for PIN13 (bits 23:20)
#define AFRH_PIN14_SET_AF5       ((uint32_t)0x05000000)        // set AF5 for PIN14 (bits 27:24)
#define AFRH_PIN15_SET_AF5       ((uint32_t)0x50000000)        // set AF5 for PIN15 (bits 31:28)

// GPIO AFR register Set bit definitions for AF6 (0110b) [SPI3, I2S3]
#define AFRL_PIN0_SET_AF6        ((uint32_t)0x00000006)        // set AF6 for PIN0 (bits 3:0)
#define AFRL_PIN1_SET_AF6        ((uint32_t)0x00000060)        // set AF6 for PIN1 (bits 7:4)
#define AFRL_PIN2_SET_AF6        ((uint32_t)0x00000600)        // set AF6 for PIN2 (bits 11:8)
#define AFRL_PIN3_SET_AF6        ((uint32_t)0x00006000)        // set AF6 for PIN3 (bits 15:12)
#define AFRL_PIN4_SET_AF6        ((uint32_t)0x00060000)        // set AF6 for PIN4 (bits 19:16)
#define AFRL_PIN5_SET_AF6        ((uint32_t)0x00600000)        // set AF6 for PIN5 (bits 23:20)
#define AFRL_PIN6_SET_AF6        ((uint32_t)0x06000000)        // set AF6 for PIN6 (bits 27:24)
#define AFRL_PIN7_SET_AF6        ((uint32_t)0x60000000)        // set AF6 for PIN7 (bits 31:28)
#define AFRH_PIN8_SET_AF6        ((uint32_t)0x00000006)        // set AF6 for PIN8 (bits 3:0)
#define AFRH_PIN9_SET_AF6        ((uint32_t)0x00000060)        // set AF6 for PIN9 (bits 7:4)
#define AFRH_PIN10_SET_AF6       ((uint32_t)0x00000600)        // set AF6 for PIN10 (bits 11:8)
#define AFRH_PIN11_SET_AF6       ((uint32_t)0x00006000)        // set AF6 for PIN11 (bits 15:12)
#define AFRH_PIN12_SET_AF6       ((uint32_t)0x00060000)        // set AF6 for PIN12 (bits 19:16)
#define AFRH_PIN13_SET_AF6       ((uint32_t)0x00600000)        // set AF6 for PIN13 (bits 23:20)
#define AFRH_PIN14_SET_AF6       ((uint32_t)0x06000000)        // set AF6 for PIN14 (bits 27:24)
#define AFRH_PIN15_SET_AF6       ((uint32_t)0x60000000)        // set AF6 for PIN15 (bits 31:28)

// GPIO AFR register Set bit definitions for AF7 (0111b) [USART1, USART2, USART3]
#define AFRL_PIN0_SET_AF7            ((uint32_t)0x00000007)        // set AF7 for PIN0 (bits 3:0)
#define AFRL_PIN1_SET_AF7            ((uint32_t)0x00000070)        // set AF7 for PIN1 (bits 7:4)
#define AFRL_PIN2_SET_AF7            ((uint32_t)0x00000700)        // set AF7 for PIN2 (bits 11:8)
#define AFRL_PIN3_SET_AF7            ((uint32_t)0x00007000)        // set AF7 for PIN3 (bits 15:12)
#define AFRL_PIN4_SET_AF7            ((uint32_t)0x00070000)        // set AF7 for PIN4 (bits 19:16)
#define AFRL_PIN5_SET_AF7            ((uint32_t)0x00700000)        // set AF7 for PIN5 (bits 23:20)
#define AFRL_PIN6_SET_AF7            ((uint32_t)0x07000000)        // set AF7 for PIN6 (bits 27:24)
#define AFRL_PIN7_SET_AF7            ((uint32_t)0x70000000)        // set AF7 for PIN7 (bits 31:28)
#define AFRH_PIN8_SET_AF7            ((uint32_t)0x00000007)        // set AF7 for PIN8 (bits 3:0)
#define AFRH_PIN9_SET_AF7            ((uint32_t)0x00000070)        // set AF7 for PIN9 (bits 7:4)
#define AFRH_PIN10_SET_AF7           ((uint32_t)0x00000700)        // set AF7 for PIN10 (bits 11:8)
#define AFRH_PIN11_SET_AF7           ((uint32_t)0x00007000)        // set AF7 for PIN11 (bits 15:12)
#define AFRH_PIN12_SET_AF7           ((uint32_t)0x00070000)        // set AF7 for PIN12 (bits 19:16)
#define AFRH_PIN13_SET_AF7           ((uint32_t)0x00700000)        // set AF7 for PIN13 (bits 23:20)
#define AFRH_PIN14_SET_AF7           ((uint32_t)0x07000000)        // set AF7 for PIN14 (bits 27:24)
#define AFRH_PIN15_SET_AF7           ((uint32_t)0x70000000)        // set AF7 for PIN15 (bits 31:28)

// GPIO AFR register Set bit definitions for AF8 (1000b) [UART4, UART5, USART6]
#define AFRL_PIN0_SET_AF8        ((uint32_t)0x00000008)        // set AF8 for PIN0 (bits 3:0)
#define AFRL_PIN1_SET_AF8        ((uint32_t)0x00000080)        // set AF8 for PIN1 (bits 7:4)
#define AFRL_PIN2_SET_AF8        ((uint32_t)0x00000800)        // set AF8 for PIN2 (bits 11:8)
#define AFRL_PIN3_SET_AF8        ((uint32_t)0x00008000)        // set AF8 for PIN3 (bits 15:12)
#define AFRL_PIN4_SET_AF8        ((uint32_t)0x00080000)        // set AF8 for PIN4 (bits 19:16)
#define AFRL_PIN5_SET_AF8        ((uint32_t)0x00800000)        // set AF8 for PIN5 (bits 23:20)
#define AFRL_PIN6_SET_AF8        ((uint32_t)0x08000000)        // set AF8 for PIN6 (bits 27:24)
#define AFRL_PIN7_SET_AF8        ((uint32_t)0x80000000)        // set AF8 for PIN7 (bits 31:28)
#define AFRH_PIN8_SET_AF8        ((uint32_t)0x00000008)        // set AF8 for PIN8 (bits 3:0)
#define AFRH_PIN9_SET_AF8        ((uint32_t)0x00000080)        // set AF8 for PIN9 (bits 7:4)
#define AFRH_PIN10_SET_AF8       ((uint32_t)0x00000800)        // set AF8 for PIN10 (bits 11:8)
#define AFRH_PIN11_SET_AF8       ((uint32_t)0x00008000)        // set AF8 for PIN11 (bits 15:12)
#define AFRH_PIN12_SET_AF8       ((uint32_t)0x00080000)        // set AF8 for PIN12 (bits 19:16)
#define AFRH_PIN13_SET_AF8       ((uint32_t)0x00800000)        // set AF8 for PIN13 (bits 23:20)
#define AFRH_PIN14_SET_AF8       ((uint32_t)0x08000000)        // set AF8 for PIN14 (bits 27:24)
#define AFRH_PIN15_SET_AF8       ((uint32_t)0x80000000)        // set AF8 for PIN15 (bits 31:28)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////GPIO CONFIGURATION DEFINITIONS/////////////////////////////////////////////
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

/* GPIO speed definitions for OSPEEDR register */
#define GPIO_SPEED_LOW       0x00U   // Low speed
#define GPIO_SPEED_MEDIUM    0x01U   // Medium speed  
#define GPIO_SPEED_HIGH      0x02U   // High speed
#define GPIO_SPEED_VERY_HIGH 0x03U   // Very high speed

/* Composite modes (preferred) */
#define GPIO_MODE_OUTPUT_PP    0x00000001U  /* MODER=01, OTYPER=0 */
#define GPIO_MODE_OUTPUT_OD    0x00000011U  /* MODER=01, OTYPER=1 */
#define GPIO_MODE_AF_PP        0x00000002U  /* MODER=10, OTYPER=0 */
#define GPIO_MODE_AF_OD        0x00000012U  /* MODER=10, OTYPER=1 */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////GPIO STRUCTURES AND ENUMERATIONS/////////////////////////////////////////////
/* RCC register definition structure */
typedef struct 
{
    __IO uint32_t CR;              // RCC clock control register
    __IO uint32_t PLLCFGR;         // RCC PLL configuration register
    __IO uint32_t CFGR;            // RCC clock configuration register
    __IO uint32_t CIR;             // RCC clock interrupt register
    __IO uint32_t AHB1RSTR;        // RCC AHB1 peripheral reset register
    __IO uint32_t AHB2RSTR;        // RCC AHB2 peripheral reset register
    __IO uint32_t AHB3RSTR;        // RCC AHB3 peripheral reset register
         uint32_t RESERVED0;       // Reserved
    __IO uint32_t APB1RSTR;        // RCC APB1 peripheral reset register
    __IO uint32_t APB2RSTR;        // RCC APB2 peripheral reset register
         uint32_t RESERVED1[2];    // Reserved
    __IO uint32_t AHB1ENR;         // RCC AHB1 peripheral clock enable register
    __IO uint32_t AHB2ENR;         // RCC AHB2 peripheral clock enable register
    __IO uint32_t AHB3ENR;         // RCC AHB3 peripheral clock enable register
         uint32_t RESERVED2;       // Reserved
    __IO uint32_t APB1ENR;         // RCC APB1 peripheral clock enable register
    __IO uint32_t APB2ENR;         // RCC APB2 peripheral clock enable register
         uint32_t RESERVED3[2];    // Reserved
    __IO uint32_t AHB1LPENR;       // RCC AHB1 peripheral clock enable in low power mode register
    __IO uint32_t AHB2LPENR;       // RCC AHB2 peripheral clock enable in low power mode register
    __IO uint32_t AHB3LPENR;       // RCC AHB3 peripheral clock enable in low power mode register
         uint32_t RESERVED4;       // Reserved
    __IO uint32_t APB1LPENR;       // RCC APB1 peripheral clock enable in low power mode register
    __IO uint32_t APB2LPENR;       // RCC APB2 peripheral clock enable in low power mode register
         uint32_t RESERVED5[2];    // Reserved
    __IO uint32_t BDCR;            // RCC Backup domain control register
    __IO uint32_t CSR;             // RCC clock control & status register
         uint32_t RESERVED6[2];    // Reserved
    __IO uint32_t SSCGR;           // RCC spread spectrum clock generation register
    __IO uint32_t PLLI2SCFGR;      // RCC PLLI2S configuration register
    __IO uint32_t PLLSAICFGR;      // RCC PLLSAI configuration register
    __IO uint32_t DCKCFGR;         // RCC Dedicated Clocks Configuration Register
} RCC_TypeDef;

/* GPIO initialization structure */
typedef struct {
    uint32_t Pin;            // Specifies the GPIO pins to be configured.
    uint32_t Mode;           // Specifies the operating mode for the selected pins.
    uint32_t Pull;           // Specifies the Pull-up or Pull-down activation for the selected pins.
    uint32_t Speed;          // Specifies the speed for the selected pins.
    uint32_t Alternate;     // Specifies the alternate function for the selected pins.
} GPIO_InitTypeDef;

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
} GPIO_ManualTypeDef;

/* GPIO handle structure */
typedef struct {
    GPIO_ManualTypeDef *regs;  // Pointer to GPIO port base address
     GPIO_InitTypeDef *init;     // GPIO initialization structure
} GPIO_HandleTypeDef;

/* GPIO pin state enumeration */
typedef enum {
    GPIO_PIN_RESET = 0,
    GPIO_PIN_SET
} GPIO_PinState;

// Peripheral declarations
#define RCC ((RCC_TypeDef *)RCC_BASE)
#define GPIO_A ((GPIO_ManualTypeDef *)GPIOA_BASE)
#define GPIO_B ((GPIO_ManualTypeDef *)GPIOB_BASE)
#define GPIO_C ((GPIO_ManualTypeDef *)GPIOC_BASE)
#define GPIO_D ((GPIO_ManualTypeDef *)GPIOD_BASE)
#define GPIO_E ((GPIO_ManualTypeDef *)GPIOE_BASE)
#define GPIO_F ((GPIO_ManualTypeDef *)GPIOF_BASE)
#define GPIO_G ((GPIO_ManualTypeDef *)GPIOG_BASE)
#define GPIO_H ((GPIO_ManualTypeDef *)GPIOH_BASE)
#define GPIO_I ((GPIO_ManualTypeDef *)GPIOI_BASE)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////GPIO FUNCTION PROTOTYPES/////////////////////////////////////////////////
/* Function prototypes */
void GPIO_Init(GPIO_HandleTypeDef *GPIOx, GPIO_ManualTypeDef *regs, GPIO_InitTypeDef *GPIO_Init);
void GPIO_TogglePin(GPIO_HandleTypeDef *GPIOx, GPIO_ManualTypeDef *regs, uint16_t GPIO_Pin);
void GPIO_WritePin(GPIO_HandleTypeDef *GPIOx, GPIO_ManualTypeDef *regs, uint16_t GPIO_Pin, GPIO_PinState PinState);
GPIO_PinState GPIO_ReadPin(GPIO_HandleTypeDef *GPIOx, GPIO_ManualTypeDef *regs, uint16_t GPIO_Pin);
void __RCC_GPIO_CLK_ENABLE(GPIO_ManualTypeDef *regs); // RCC peripheral clock enable function
///////////////////////////////////////////////////////////////////////////////////////////////////////////////


#endif // __GPIO_H