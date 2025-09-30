#ifndef __UART_H
#define __UART_H

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include <stdio.h>

void USART2_Init(void);
int USART2_Write(int ch);
int USART2_Read(void);
void test_setup(void);

#endif // __UART_H
