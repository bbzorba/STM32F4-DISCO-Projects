#ifndef __UART_H
#define __UART_H

#include <stdint.h>
#include <stdio.h>
#include "../../GPIO/inc/gpio.h"

#define __IO volatile

/* 
 USART pins for STM32F4xx series:
 USART1 -> PB6 (TX), PB7 (RX) or PA9 (TX), PA10 (RX)
 USART2 -> PA2 (TX), PA3 (RX) or PD5 (TX), PD6 (RX)
 USART3 -> PB10 (TX), PB11 (RX) or PD8 (TX), PD9 (RX)
 UART4 -> PA0 (TX), PA1 (RX) or PC10 (TX), PC11 (RX)
 UART5 -> PC12 (TX), PD2 (RX)
 USART6 -> PC6 (TX), PC7 (RX)*/

// Base addresses
#define PERIPH_ADDR_BASE 0x40000000U
#define APB1PERIPH_ADDR_BASE (PERIPH_ADDR_BASE + 0x00000000U)
#define APB2PERIPH_ADDR_BASE (PERIPH_ADDR_BASE + 0x00010000U)

// APB1-related USART peripherals
#define USART_2_BASE (APB1PERIPH_ADDR_BASE + 0x00004400U)
#define USART_3_BASE (APB1PERIPH_ADDR_BASE + 0x00004800U)
#define UART_4_BASE (APB1PERIPH_ADDR_BASE + 0x00004C00U)
#define UART_5_BASE (APB1PERIPH_ADDR_BASE + 0x00005000U)

// APB2-related USART peripherals
#define USART_1_BASE (APB2PERIPH_ADDR_BASE + 0x00001000U)
#define USART_6_BASE (APB2PERIPH_ADDR_BASE + 0x00001400U)

// RCC register bit definitions
#define RCC_APB2ENR_USART_1EN ((uint32_t)0x00004000)        // Bit 14
#define RCC_APB1ENR_USART_2EN ((uint32_t)0x00020000)        // Bit 17
#define RCC_APB1ENR_USART_3EN ((uint32_t)0x00040000)        // Bit 18
#define RCC_APB1ENR_UART_4EN  ((uint32_t)0x00080000)        // Bit 19
#define RCC_APB1ENR_UART_5EN  ((uint32_t)0x00100000)        // Bit 20
#define RCC_APB2ENR_USART_6EN ((uint32_t)0x00020000)        // Bit 5
#define RCC_AHB1ENR_GPIOAEN  ((uint32_t)0x00000001)         // Bit 0
#define RCC_AHB1ENR_GPIOBEN  ((uint32_t)0x00000002)         // Bit 1
#define RCC_AHB1ENR_GPIOCEN  ((uint32_t)0x00000004)         // Bit 2
#define RCC_AHB1ENR_GPIODEN  ((uint32_t)0x00000008)         // Bit 3
#define RCC_AHB1ENR_GPIOEEN  ((uint32_t)0x00000010)         // Bit 4

// GPIOA MODER register bit definitions
#define GPIOA_MODER_PIN0_MASK    ((uint32_t)0x00000003)        // mask to clear PA0 mode bits (bits 1:0)
#define GPIOA_MODER_PIN1_MASK    ((uint32_t)0x0000000C)        // mask to clear PA1 mode bits (bits 3:2)
#define GPIOA_MODER_PIN2_MASK    ((uint32_t)0x00000030)        // mask to clear PA2 mode bits (bits 5:4)
#define GPIOA_MODER_PIN3_MASK    ((uint32_t)0x000000C0)        // mask to clear PA3 mode bits (bits 7:6)
#define GPIOA_MODER_PIN9_MASK    ((uint32_t)0x00000300)        // mask to clear PA9 mode bits (bits 19:18)
#define GPIOA_MODER_PIN10_MASK   ((uint32_t)0x00000C00)        // mask to clear PA10 mode bits (bits 21:20)
#define GPIOA_MODER_PIN0_SET      ((uint32_t)0x00000002)        // set AF for PA0 (bits 1:0)
#define GPIOA_MODER_PIN1_SET      ((uint32_t)0x00000008)        // set AF for PA1 (bits 3:2)
#define GPIOA_MODER_PIN2_SET      ((uint32_t)0x00000020)        // set AF for PA2 (bits 5:4)
#define GPIOA_MODER_PIN3_SET      ((uint32_t)0x00000080)        // set AF for PA3 (bits 7:6)
#define GPIOA_MODER_PIN9_SET      ((uint32_t)0x00000200)        // set AF for PA9 (bits 19:18)
#define GPIOA_MODER_PIN10_SET     ((uint32_t)0x00000800)        // set AF for PA10 (bits 21:20)

// GPIOA AFR register bit definitions
#define GPIOA_AFRL_PIN0_MASK      ((uint32_t)0x0000000F)        // AF8 mask for PA0 (bits 3:0)
#define GPIOA_AFRL_PIN1_MASK      ((uint32_t)0x000000F0)        // AF8 mask for PA1 (bits 7:4)
#define GPIOA_AFRL_PIN2_MASK      ((uint32_t)0x00000F00)        // AF7 mask for PA2 (bits 11:8)
#define GPIOA_AFRL_PIN3_MASK      ((uint32_t)0x00000F00)        // AF7 mask for PA3 (bits 15:12)
#define GPIOA_AFRH_PIN9_MASK      ((uint32_t)0x000000F0)        // AF7 mask for PA9 (bits 11:8)
#define GPIOA_AFRH_PIN10_MASK     ((uint32_t)0x000000F0)        // AF7 mask for PA10 (bits 15:12)
#define GPIOA_AFRL_PIN0_SET       ((uint32_t)0x00000008)        // AF8 for PA0 (bits 3:0)
#define GPIOA_AFRL_PIN1_SET       ((uint32_t)0x00000080)        // AF8 for PA1 (bits 7:4)
#define GPIOA_AFRL_PIN2_SET       ((uint32_t)0x00000700)        // AF7 for PA2 (bits 11:8)
#define GPIOA_AFRL_PIN3_SET       ((uint32_t)0x00007000)        // AF7 for PA3 (bits 15:12)
#define GPIOA_AFRH_PIN9_SET       ((uint32_t)0x00000070)        // AF7 for PA9 (bits 11:8)
#define GPIOA_AFRH_PIN10_SET      ((uint32_t)0x00000070)        // AF7 for PA10 (bits 15:12)

// GPIOB MODER register bit definitions
#define GPIOB_MODER_PIN6_MASK    ((uint32_t)0x00003000)        // mask to clear PB6 mode bits (bits 13:12)
#define GPIOB_MODER_PIN7_MASK    ((uint32_t)0x0000C000)        // mask to clear PB7 mode bits (bits 15:14)
#define GPIOB_MODER_PIN10_MASK   ((uint32_t)0x00300000)        // mask to clear PB10 mode bits (bits 21:20)
#define GPIOB_MODER_PIN11_MASK   ((uint32_t)0x00C00000)        // mask to clear PB11 mode bits (bits 23:22)
#define GPIOB_MODER_PIN6_SET      ((uint32_t)0x00002000)        // set AF for PB6 (bits 13:12)
#define GPIOB_MODER_PIN7_SET      ((uint32_t)0x00008000)        // set AF for PB7 (bits 15:14)
#define GPIOB_MODER_PIN10_SET     ((uint32_t)0x00200000)        // set AF (10b) for PB10 (bits 21:20)
#define GPIOB_MODER_PIN11_SET     ((uint32_t)0x00800000)        // set AF (10b) for PB11 (bits 23:22)

// GPIOB AFR register bit definitions
#define GPIOB_AFRL_PIN6_MASK      ((uint32_t)0x0F000000)        // AF7 mask for PB6 (bits 27:24)
#define GPIOB_AFRL_PIN7_MASK      ((uint32_t)0xF0000000)        // AF7 mask for PB7 (bits 31:28)
#define GPIOB_AFRH_PIN10_MASK      ((uint32_t)0x0000000F)        // AF7 mask for PB10 (bits 3:0)
#define GPIOB_AFRH_PIN11_MASK      ((uint32_t)0x000000F0)        // AF7 mask for PB11 (bits 7:4)
#define GPIOB_AFRL_PIN6_SET       ((uint32_t)0x07000000)        // AF7 for PB6 (bits 27:24)
#define GPIOB_AFRL_PIN7_SET       ((uint32_t)0x70000000)        // AF7 for PB7 (bits 31:28)
#define GPIOB_AFRH_PIN10_SET     ((uint32_t)0x00000007)        // AF7 for PB10 (bits 3:0)
#define GPIOB_AFRH_PIN11_SET     ((uint32_t)0x00000070)        // AF7 for PB11 (bits 7:4)

// GPIOC MODER register bit definitions
#define GPIOC_MODER_PIN6_MASK    ((uint32_t)0x00003000)        // mask to clear PC6 mode bits (bits 13:12)
#define GPIOC_MODER_PIN7_MASK    ((uint32_t)0x0000C000)        // mask to clear PC7 mode bits (bits 15:14)
#define GPIOC_MODER_PIN10_MASK   ((uint32_t)0x000C0000)        // mask to clear PC10 mode bits (bits 21:20)
#define GPIOC_MODER_PIN11_MASK   ((uint32_t)0x00300000)        // mask to clear PC11 mode bits (bits 23:22)
#define GPIOC_MODER_PIN12_MASK   ((uint32_t)0x00300000)        // mask to clear PC12 mode bits (bits 25:24)
#define GPIOC_MODER_PIN6_SET      ((uint32_t)0x00002000)        // set AF for PC6 (bits 13:12)
#define GPIOC_MODER_PIN7_SET      ((uint32_t)0x00008000)        // set AF for PC7 (bits 15:14)
#define GPIOC_MODER_PIN10_SET     ((uint32_t)0x00080000)        // set AF for PC10 (bits 21:20)
#define GPIOC_MODER_PIN11_SET     ((uint32_t)0x00200000)        // set AF for PC11 (bits 23:22)
#define GPIOC_MODER_PIN12_SET     ((uint32_t)0x00200000)        // set AF for PC12 (bits 25:24)

// GPIOC AFR register bit definitions
#define GPIOC_AFRL_PIN6_MASK      ((uint32_t)0x0F000000)        // AF8 mask for PC6 (bits 27:24)
#define GPIOC_AFRL_PIN7_MASK      ((uint32_t)0xF0000000)        // AF8 mask for PC7 (bits 31:28)
#define GPIOC_AFRH_PIN10_MASK     ((uint32_t)0x000000F0)        // AF8 mask for PC10 (bits 11:8)
#define GPIOC_AFRH_PIN11_MASK     ((uint32_t)0x00000F00)        // AF8 mask for PC11 (bits 15:12)
#define GPIOC_AFRH_PIN12_MASK     ((uint32_t)0x000000F0)        // AF8 mask for PC12 (bits 15:12)
#define GPIOC_AFRL_PIN6_SET       ((uint32_t)0x08000000)        // AF8 for PC6 (bits 27:24)
#define GPIOC_AFRL_PIN7_SET       ((uint32_t)0x80000000)        // AF8 for PC7 (bits 31:28)
#define GPIOC_AFRH_PIN10_SET      ((uint32_t)0x00000080)        // AF8 for PC10 (bits 11:8)
#define GPIOC_AFRH_PIN11_SET      ((uint32_t)0x00000800)        // AF8 for PC11 (bits 15:12)
#define GPIOC_AFRH_PIN12_SET      ((uint32_t)0x00000080)        // AF8 for PC12 (bits 15:12)

// GPIOD MODER register bit definitions
#define GPIOD_MODER_PIN2_MASK    ((uint32_t)0x0000000C)        // mask to clear PD2 mode bits (bits 3:2)
#define GPIOD_MODER_PIN5_MASK    ((uint32_t)0x00000300)        // mask to clear PD5 mode bits (bits 11:10)
#define GPIOD_MODER_PIN6_MASK    ((uint32_t)0x00000C00)        // mask to clear PD6 mode bits (bits 13:12)
#define GPIOD_MODER_PIN8_MASK    ((uint32_t)0x00030000)        // mask to clear PD8 mode bits (bits 17:16)
#define GPIOD_MODER_PIN9_MASK    ((uint32_t)0x000C0000)        // mask to clear PD9 mode bits (bits 19:18)
#define GPIOD_MODER_PIN2_SET      ((uint32_t)0x00000008)        // set AF for PD2 (bits 3:2)
#define GPIOD_MODER_PIN5_SET      ((uint32_t)0x00000200)        // set AF for PD5 (bits 11:10)
#define GPIOD_MODER_PIN6_SET      ((uint32_t)0x00000800)        // set AF for PD6 (bits 13:12)
#define GPIOD_MODER_PIN8_SET      ((uint32_t)0x00020000)        // set AF for PD8 (bits 17:16)
#define GPIOD_MODER_PIN9_SET      ((uint32_t)0x00080000)        // set AF for PD9 (bits 19:18)

// GPIOD AFR register bit definitions
#define GPIOD_AFRL_PIN2_MASK      ((uint32_t)0x00000F00)        // AF7 mask for PD2 (bits 11:8)
#define GPIOD_AFRL_PIN5_MASK      ((uint32_t)0x0000F00)        // AF7 mask for PD5 (bits 23:20)
#define GPIOD_AFRL_PIN6_MASK      ((uint32_t)0x000F0000)        // AF7 mask for PD6 (bits 27:24)
#define GPIOD_AFRH_PIN8_MASK      ((uint32_t)0x000000F0)        // AF7 mask for PD8 (bits 11:8)
#define GPIOD_AFRH_PIN9_MASK      ((uint32_t)0x00000F00)        // AF7 mask for PD9 (bits 15:12)
#define GPIOD_AFRL_PIN2_SET       ((uint32_t)0x00000700)        // AF7 for PD2 (bits 11:8)
#define GPIOD_AFRL_PIN5_SET       ((uint32_t)0x00007000)        // AF7 for PD5 (bits 23:20)
#define GPIOD_AFRL_PIN6_SET       ((uint32_t)0x00700000)        // AF7 for PD6 (bits 27:24)
#define GPIOD_AFRH_PIN8_SET       ((uint32_t)0x00000070)        // AF7 for PD8 (bits 11:8)
#define GPIOD_AFRH_PIN9_SET       ((uint32_t)0x00000700)        // AF7 for PD9 (bits 15:12)

// USART BRR register bit definitions
#define BRR_CNF1_115200 0x1A1                               // Set baud rate to 115200
#define BRR_CNF2_9600 0x683                                 // Set baud rate to 9600

//USART CR1, CR2 & CR3 register bit definitions
#define CR2_CNF1 0x0000                                     // 1 stop bit, no-op
#define CR3_CNF1 0x0000                                     // No flow control, no-op
#define USART_2_CR1_DIS 0x0000                              // Disable USART_2
#define USART_CR1_TX_EN 0x0008                              // Enable Transmitter
#define USART_CR1_RX_EN 0x0004                              // Enable Receiver
#define USART_CR1_EN 0x2000                                 // Enable USART

//USART SR & DR register bit definitions
#define USART_SR_RX_NOT_EMP 0x0020                          // USART Status Register - Receiver not Empty
#define USART_SR_TX_EMP 0x0080                              // USART Status Register - Transmitter not Empty

/* USART registers structure */
typedef struct
{
    __IO uint32_t SR;    // Status Register
    __IO uint32_t DR;    // Data Register
    __IO uint32_t BRR;   // Baud Rate Register
    __IO uint32_t CR1;   // Control Register 1
    __IO uint32_t CR2;   // Control Register 2
    __IO uint32_t CR3;   // Control Register 3
    __IO uint32_t GTPR;  // Guard time and prescaler register
} USART_Manual_TypeDef;

// UART configuration enums
typedef enum {
    RX_ONLY = 0,
    TX_ONLY,
    RX_AND_TX
} UART_COMType;

typedef enum {
    __115200 = 0,
    __9600,
} UART_BaudRateType;

// USART2 peripheral declaration
#define USART_1 ((USART_Manual_TypeDef *)USART_1_BASE)
#define USART_2 ((USART_Manual_TypeDef *)USART_2_BASE)
#define USART_3 ((USART_Manual_TypeDef *)USART_3_BASE)
#define UART_4  ((USART_Manual_TypeDef *)UART_4_BASE)
#define UART_5  ((USART_Manual_TypeDef *)UART_5_BASE)
#define USART_6 ((USART_Manual_TypeDef *)USART_6_BASE)


// Function prototypes
void USART_x_Init(USART_Manual_TypeDef *USARTx, UART_COMType comtype, UART_BaudRateType baudrate);
void USART_x_Write(USART_Manual_TypeDef *USARTx, int ch);
char USART_x_Read(USART_Manual_TypeDef *USARTx);

#endif // __UART_H
