#ifndef LIS3DSH_H
#define LIS3DSH_H

#include <stdint.h>
#include "../../Drivers/SPI/inc/spi.h"

// Registers (subset)
#define LIS3DSH_REG_WHO_AM_I   0x0F
#define LIS3DSH_REG_CTRL4      0x20
#define LIS3DSH_REG_CTRL5      0x24
#define LIS3DSH_REG_OUT_X_L    0x28
#define LIS3DSH_REG_OUT_X_H    0x29
#define LIS3DSH_REG_OUT_Y_L    0x2A
#define LIS3DSH_REG_OUT_Y_H    0x2B
#define LIS3DSH_REG_OUT_Z_L    0x2C
#define LIS3DSH_REG_OUT_Z_H    0x2D

// WHO_AM_I expected value
#define LIS3DSH_WHOAMI_EXPECTED 0x3F

// SPI flags
#define LIS3DSH_SPI_READ       0x80
#define LIS3DSH_SPI_AUTO_INC   0x40

// Simple driver API
void    LIS3DSH_Select(GPIO_HandleTypeDef *cs, uint16_t pin);
void    LIS3DSH_Deselect(GPIO_HandleTypeDef *cs, uint16_t pin);
int     LIS3DSH_ReadReg(SPI_HandleType *spi, GPIO_HandleTypeDef *cs, uint16_t pin, uint8_t reg, uint8_t *val);
int     LIS3DSH_WriteReg(SPI_HandleType *spi, GPIO_HandleTypeDef *cs, uint16_t pin, uint8_t reg, uint8_t val);
int     LIS3DSH_ReadWhoAmI(SPI_HandleType *spi, GPIO_HandleTypeDef *cs, uint16_t pin, uint8_t *who);
int     LIS3DSH_Init(SPI_HandleType *spi, GPIO_HandleTypeDef *cs, uint16_t pin);
int     LIS3DSH_ReadXYZ(SPI_HandleType *spi, GPIO_HandleTypeDef *cs, uint16_t pin, int16_t *x, int16_t *y, int16_t *z);
int     LIS3DSH_ReadWhoAmI_Mode(SPI_HandleType *spi, GPIO_HandleTypeDef *cs, uint16_t pin, uint8_t mode, uint8_t *who);

#endif // LIS3DSH_H
