#ifndef TSL2591_LIGHT_H
#define TSL2591_LIGHT_H

#include <stdio.h>
#include <stdint.h>
#include "../../../Drivers/I2C/inc/i2c.h"
#include "../../../Drivers/UART/inc/uart.h"

// TSL2591 default 7-bit address is 0x29 (per datasheet)
#ifndef TSL2591_ADDR
#define TSL2591_ADDR 0x29
#endif

// I2C function prototypes
void TSL2591_Init(I2C_HandleType *hi2c, I2C_ManualTypeDef *regs, I2C_SpeedType speed);
uint16_t tsl_read_word(I2C_HandleType *hi2c, uint8_t address, uint8_t reg);
uint32_t TSL2591_ReadLightData(I2C_HandleType *hi2c, uint8_t address);

#endif // TSL2591_LIGHT_H
