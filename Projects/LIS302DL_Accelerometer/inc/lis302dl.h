#include "../../Drivers/SPI/inc/spi.h"
#include "../../Drivers/UART/inc/uart.h"

#ifndef LIS302DL_H
#define LIS302DL_H

#include <stdint.h>

// Registers
#define LIS302DL_REG_WHO_AM_I   0x0F
#define LIS302DL_REG_CTRL1      0x20
#define LIS302DL_REG_CTRL2      0x21
#define LIS302DL_REG_CTRL3      0x22
#define LIS302DL_REG_HP_FILTER  0x23
#define LIS302DL_REG_STATUS     0x27
#define LIS302DL_REG_OUT_X      0x29
#define LIS302DL_REG_OUT_Y      0x2B
#define LIS302DL_REG_OUT_Z      0x2D

// WHO_AM_I expected value
#define LIS302DL_WHOAMI_EXPECTED 0x3B

// Read bit for SPI
#define LIS302DL_SPI_READ       0x80
#define LIS302DL_SPI_AUTO_INC   0x40

// Simple driver helpers
void LIS302DL_Select(GPIO_HandleTypeDef *cs, uint16_t pin);
void LIS302DL_Deselect(GPIO_HandleTypeDef *cs, uint16_t pin);
int  LIS302DL_ReadReg(SPI_HandleType *spi, GPIO_HandleTypeDef *cs, uint16_t pin, uint8_t reg, uint8_t *val);
int  LIS302DL_WriteReg(SPI_HandleType *spi, GPIO_HandleTypeDef *cs, uint16_t pin, uint8_t reg, uint8_t val);
int  LIS302DL_ReadWhoAmI(SPI_HandleType *spi, GPIO_HandleTypeDef *cs, uint16_t pin, uint8_t *who);
int  LIS302DL_Init(SPI_HandleType *spi, GPIO_HandleTypeDef *cs, uint16_t pin);
int  LIS302DL_ReadXYZ(SPI_HandleType *spi, GPIO_HandleTypeDef *cs, uint16_t pin, int8_t *x, int8_t *y, int8_t *z);

// Debug helper: try a specific SPI mode (0..3) when reading WHO_AM_I
int  LIS302DL_ReadWhoAmI_Mode(SPI_HandleType *spi, GPIO_HandleTypeDef *cs, uint16_t pin, uint8_t mode, uint8_t *who);

// Internal helper used by the driver to ensure proper SPI mode
// Tries mode 3 first, then falls back to mode 0 during WHO_AM_I.
// No public API required; exposed here only if future tuning is needed.

// Calibration and mode helpers
void LIS302DL_SetSpiMode(uint8_t mode);
uint8_t LIS302DL_GetSpiMode(void);
int  LIS302DL_SelectBestSpiMode(SPI_HandleType *spi, GPIO_HandleTypeDef *cs, uint16_t pin);
void LIS302DL_SetSensitivityMgPerLsb(uint8_t mg_per_lsb);
int  LIS302DL_Calibrate(SPI_HandleType *spi, GPIO_HandleTypeDef *cs, uint16_t pin, uint16_t samples);
void LIS302DL_GetCalibration(int16_t *ox, int16_t *oy, int16_t *oz, uint8_t *mg_per_lsb);

#endif // LIS302DL_H