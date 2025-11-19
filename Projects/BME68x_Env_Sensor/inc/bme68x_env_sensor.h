#ifndef BME68X_ENV_SENSOR_H
#define BME68X_ENV_SENSOR_H

#include <stdio.h>
#include <stdint.h>
#include "../../../Drivers/I2C/inc/i2c.h"

// BME68x default 7-bit address is 0x76 (per datasheet)
#ifndef BME68X_ADDR
#define BME68X_ADDR 0x77
#endif

// I2C function prototypes
void BME68x_Init(void);
int read_temp(uint8_t address);
int bme68x_get_cal_and_raw(uint8_t address, uint16_t *out_t1, int16_t *out_t2, int8_t *out_t3, uint32_t *out_adc_temp);
int read_raw_temp(uint8_t address);

#endif // BME68X_ENV_SENSOR_H
