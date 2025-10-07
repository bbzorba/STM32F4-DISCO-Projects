#ifndef __UART_H
#define __UART_H

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include <stdio.h>

#define __IO volatile

#define PERIPH_ADDR_BASE 0x40000000U
#define APB1PERIPH_ADDR_BASE (PERIPH_ADDR_BASE + 0x00000000U)
#define USART_1_BASE (APB1PERIPH_ADDR_BASE + 0x00001000U)
#define USART_2_BASE (APB1PERIPH_ADDR_BASE + 0x00004400U)

#define RCC_APB1ENR_USART_2EN ((uint32_t)0x00020000)          // Bit 17
#define RCC_AHB1ENR_GPIOAEN  ((uint32_t)0x00000001)          // Bit 0

#define GPIOA_MODER_ALTERNATE ((uint32_t)0x000000F0)         // mask to clear PA2/PA3 mode bits
#define GPIOA_MODER_PIN2     ((uint32_t)0x00000020)          // set AF (10) for PA2
#define GPIOA_MODER_PIN3     ((uint32_t)0x00000080)          // set AF (10) for PA3

#define GPIOA_AFR_TYPE       ((uint32_t)0x0000FF00)          // AFRL for PA2 and PA3
#define GPIOA_AFR_VALUE      ((uint32_t)0x00007700)          // AF7 for USART2

#define BAUD_RATE 115200

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

#define USART_2 ((USART_Manual_TypeDef *)USART_2_BASE)


void USART_2_Init(void);
int USART_2_Write(int ch);
int USART_2_Read(void);
void test_setup(void);

#endif // __UART_H
