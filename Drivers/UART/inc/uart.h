#ifndef __UART_H
#define __UART_H

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include <stdio.h>

#define __IO volatile

#define PERIPH_BASE 0x40000000U
#define APB1PERIPH_BASE (PERIPH_BASE + 0x00000000U)
#define USART1_BASE (APB1PERIPH_BASE + 0x00001000U)
#define USART2_BASE (APB1PERIPH_BASE + 0x00004400U)

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
} USART_TypeDef;

#define USART2 ((USART_TypeDef *)USART2_BASE)


void USART2_Init(void);
int USART2_Write(int ch);
int USART2_Read(void);
void test_setup(void);

#endif // __UART_H
