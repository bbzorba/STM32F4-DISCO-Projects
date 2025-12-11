#include "../../Drivers/I2C/inc/i2c.h"
#include "../../Drivers/GPIO/inc/gpio.h"
#include "../../Drivers/UART/inc/uart.h"
// AF constants for I2C pins (define minimal value to avoid HAL includes)
#define GPIO_AF4_I2C1 ((uint8_t)0x04)
#include <string.h>
#include <stdio.h>

extern I2C_HandleType hi2c1;

#define TSL2591_ADDR         (0x29 << 1)    // HAL uses 8-bit address
#define TSL2591_COMMAND_BIT  0xA0

// TSL2591 registers
#define TSL2591_REG_ENABLE   0x00
#define TSL2591_REG_CONTROL  0x01
#define TSL2591_REG_CHAN0_L  0x14
#define TSL2591_REG_CHAN1_L  0x16

// ENABLE register bits
#define TSL2591_ENABLE_POWERON   0x01
#define TSL2591_ENABLE_AEN       0x02     // ALS Enable

// CONTROL register values
#define TSL2591_GAIN_MED         0x10     // Medium gain
#define TSL2591_IT_100MS         0x00     // Integration time

void SystemClock_Config(void);
void MX_I2C1_Init(void);
void I2C_Scan(void);
uint8_t TSL2591_Init(void);
uint16_t TSL2591_ReadChannel(uint8_t reg);

// Minimal HAL-compat stubs to keep structure
#define HAL_OK 0
void HAL_Delay(uint32_t ms);
void HAL_Init(void);