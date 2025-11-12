#ifndef BME68X_ENV_SENSOR_H
#define BME68X_ENV_SENSOR_H

#include <stdio.h>
#include <stdint.h>
#include "../../../Drivers/I2C/inc/i2c.h"

// BME68x default 7-bit address is 0x76 (per datasheet)
#ifndef BME68X_ADDR
#define BME68X_ADDR 0x76
#endif

// I2C function prototypes
void BME68x_Init(void);
int read_temp(uint8_t address);
int read_gas_density(uint8_t address);
int read_humidity(uint8_t address);

#endif // BME68X_ENV_SENSOR_H
