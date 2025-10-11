#ifndef __UART_H
#define __UART_H

#include <stdint.h>
#include <stdio.h>
#include "../../GPIO/inc/gpio.h"

#define __IO volatile

// Base addresses
#define PERIPH_ADDR_BASE 0x40000000U
#define APB1PERIPH_ADDR_BASE (PERIPH_ADDR_BASE + 0x00000000U)
#define USART_1_BASE (APB1PERIPH_ADDR_BASE + 0x00001000U)
#define USART_2_BASE (APB1PERIPH_ADDR_BASE + 0x00004400U)

// RCC register bit definitions
#define RCC_APB1ENR_USART_2EN ((uint32_t)0x00020000)        // Bit 17
#define RCC_AHB1ENR_GPIOAEN  ((uint32_t)0x00000001)         // Bit 0

// GPIOA register bit definitions
#define GPIOA_MODER_ALTERNATE ((uint32_t)0x000000F0)        // mask to clear PA2/PA3 mode bits
#define GPIOA_MODER_PIN2     ((uint32_t)0x00000020)         // set AF (10) for PA2
#define GPIOA_MODER_PIN3     ((uint32_t)0x00000080)         // set AF (10) for PA3
#define GPIOA_AFR_TYPE       ((uint32_t)0x0000FF00)         // AFRL for PA2 and PA3
#define GPIOA_AFR_TX      ((uint32_t)0x00007000)            // AF7 for PA2 (TX)
#define GPIOA_AFR_RX      ((uint32_t)0x00000700)            // AF7 for PA3 (RX)
#define GPIOA_AFR_VALUE      (GPIOA_AFR_TX | GPIOA_AFR_RX)  // AF7 for USART2

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

/* USART2 registers structure */
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
#define USART_2 ((USART_Manual_TypeDef *)USART_2_BASE)

// Function prototypes
void USART_2_Init(UART_COMType comtype, UART_BaudRateType baudrate);
void USART_2_Write(int ch);
char USART_2_Read(void);

#endif // __UART_H
