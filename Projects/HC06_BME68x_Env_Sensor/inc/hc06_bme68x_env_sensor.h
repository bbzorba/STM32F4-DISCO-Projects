#ifndef HC06_BME68x_ENV_SENSOR_H
#define HC06_BME68x_ENV_SENSOR_H

#include <stdio.h>
#include <stdint.h>
#include "../../BME68x_Env_Sensor/inc/bme68x_env_sensor.h"
#include "../../HC06_Bluetooth/inc/hc06.h"

// BME68x default 7-bit address is 0x76 (per datasheet)
#ifndef BME68x_ADDR
#define BME68x_ADDR 0x76
#endif

// I2C function prototypes
void send_temp(HC06 *hc06);
void send_gas_density(HC06 *hc06);
void send_humidity(HC06 *hc06);

#endif // HC06_BME68x_ENV_SENSOR_H
