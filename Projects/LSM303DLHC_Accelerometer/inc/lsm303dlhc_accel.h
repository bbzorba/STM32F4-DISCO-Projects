#ifndef LSM303DLHC_ACCEL_H
#define LSM303DLHC_ACCEL_H

#include <stdio.h>
#include <stdint.h>
#include "../../../Drivers/I2C/inc/i2c.h"
#include "../../../Drivers/UART/inc/uart.h"

#ifndef LSM303DLHC_ADDR
#define LSM303DLHC_ADDR 0x19
#endif

// I2C function prototypes
int read_accel(I2C_HandleType *hi2c, uint8_t address);
void LSM303DLHC_Init(I2C_HandleType *hi2c);

#endif // LSM303DLHC_ACCEL_H
