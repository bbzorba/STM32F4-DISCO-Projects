#ifndef HC06_BME68x_ENV_SENSOR_H
#define HC06_BME68x_ENV_SENSOR_H

#include <stdio.h>
#include <stdint.h>
#include "../../BME68x_Env_Sensor/inc/bme68x_env_sensor.h"
#include "../../HC06_Bluetooth/inc/hc06.h"
#include "../../../Drivers/I2C/inc/i2c.h"

#ifndef BME68X_ADDR
#define BME68X_ADDR 0x77
#endif

// I2C function prototypes
void send_temp(HC06 *hc06);
void send_gas_resistance(HC06 *hc06);
void send_humidity(HC06 *hc06);
void send_bme68x_debug(HC06 *hc06);

#endif // HC06_BME68x_ENV_SENSOR_H
