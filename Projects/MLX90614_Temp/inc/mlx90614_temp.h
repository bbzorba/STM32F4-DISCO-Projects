#ifndef MLX90614_TEMP_H
#define MLX90614_TEMP_H

#include <stdio.h>
#include <stdint.h>
#include "../../../Drivers/I2C/inc/i2c.h"
#include "../../../Drivers/UART/inc/uart.h"

// MLX90614 default 7-bit address is 0x5A (per datasheet)
#ifndef MLX90614_ADDR
#define MLX90614_ADDR 0x5A
#endif

// I2C function prototypes
void MLX90614_Init(void);
int read_obj_temp(uint8_t address);
int read_amb_temp(uint8_t address);

#endif // MLX90614_TEMP_H
