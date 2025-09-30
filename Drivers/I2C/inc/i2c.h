#ifndef __I2C_H
#define __I2C_H

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include <stdio.h>

void I2C1_Init(void);
int I2C1_Read(I2C_SR1_ADDR);

#endif // __I2C_H
