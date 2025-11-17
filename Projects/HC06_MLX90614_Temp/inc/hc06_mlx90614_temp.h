#ifndef HC06_MLX90614_TEMP_H
#define HC06_MLX90614_TEMP_H

#include <stdio.h>
#include <stdint.h>
#include "../../MLX90614_Temp/inc/mlx90614_temp.h"
#include "../../HC06_Bluetooth/inc/hc06.h"

// I2C function prototypes
void send_obj_temp(HC06 *hc06);
void send_amb_temp(HC06 *hc06);
void hc06_scan_i2c(HC06 *hc06); // Scan I2C bus and report ACKed addresses over Bluetooth

#endif // HC06_MLX90614_TEMP_H
