#ifndef __I2C_H
#define __I2C_H

#include <stdio.h>

#define GPIOB_MODER_ALTERNATE ((uint32_t)0x0000F000)         // mask to clear PB7/PB8 mode bits
#define GPIOB_MODER_PIN6     ((uint32_t)0x00002000)          // set AF (0b10) for PB6
#define GPIOB_MODER_PIN7     ((uint32_t)0x00008000)          // set AF (0b10) for PB7

#define GPIOB_AFR_TYPE       ((uint32_t)0xFF000000)          // AFRL for PB6 and PB7
#define GPIOB_AFR_VALUE      ((uint32_t)0x77000000)          // AF7 for USART2

void I2C1_Init(void);
int I2C1_Read(I2C_SR1_ADDR);

#endif // __I2C_H
