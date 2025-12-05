#ifndef TSL2591_LIGHT_H
#define TSL2591_LIGHT_H

#include <stdio.h>
#include <stdint.h>
#include "../../../Drivers/I2C/inc/i2c.h"
#include "../../../Drivers/UART/inc/uart.h"

// TSL2591 default 7-bit address is 0x29 (per datasheet)
#ifndef TSL2591_ADDR
#define TSL2591_ADDR 0x29
#endif

// TSL2591 Register Definitions
#define TSL2591_COMMAND_BIT     (0x80)    // Must be 1
#define TSL2591_WORD_BIT        (0x20)    // 1 = read/write word (rather than byte)
#define TSL2591_REGISTER_ENABLE             (0x00)
#define TSL2591_REGISTER_CONTROL            (0x01)
#define TSL2591_REGISTER_CHAN0_LOW          (0x14)
#define TSL2591_REGISTER_CHAN1_LOW          (0x16)
#define TSL2591_REGISTER_DEVICE_ID          (0x12)

// Enable Register (0x00)
#define TSL2591_ENABLE_POWERON              (0x01)
#define TSL2591_ENABLE_POWEROFF             (0x00)
#define TSL2591_ENABLE_AEN                  (0x02)
#define TSL2591_ENABLE_AIEN                 (0x10)
#define TSL2591_ENABLE_NPIEN                (0x80)

// Control Register (0x01)
#define TSL2591_CONTROL_RESET               (0x80)
#define TSL2591_CONTROL_AGAIN_LOW           (0x00)  // Low gain (1x)
#define TSL2591_CONTROL_AGAIN_MED           (0x10)  // Medium gain (25x)
#define TSL2591_CONTROL_AGAIN_HIGH          (0x20)  // High gain (428x)
#define TSL2591_CONTROL_AGAIN_MAX           (0x30)  // Max gain (9876x)
#define TSL2591_CONTROL_ATIME_100MS         (0x00)  // 100 millis
#define TSL2591_CONTROL_ATIME_200MS         (0x01)  // 200 millis
#define TSL2591_CONTROL_ATIME_300MS         (0x02)  // 300 millis
#define TSL2591_CONTROL_ATIME_400MS         (0x03)  // 400 millis
#define TSL2591_CONTROL_ATIME_500MS         (0x04)  // 500 millis
#define TSL2591_CONTROL_ATIME_600MS         (0x05)  // 600 millis

// I2C function prototypes
void TSL2591_Init(I2C_HandleType *hi2c, I2C_ManualTypeDef *regs, I2C_SpeedType speed);
uint8_t TSL2591_Begin(I2C_HandleType *hi2c);
void TSL2591_Enable(I2C_HandleType *hi2c);
void TSL2591_Disable(I2C_HandleType *hi2c);
void TSL2591_SetGainAndTime(I2C_HandleType *hi2c, uint8_t gain, uint8_t time);
uint8_t tsl_read_byte(I2C_HandleType *hi2c, uint8_t address, uint8_t reg);
void tsl_write_byte(I2C_HandleType *hi2c, uint8_t address, uint8_t reg, uint8_t value);
uint16_t tsl_read_word(I2C_HandleType *hi2c, uint8_t address, uint8_t reg);
uint32_t TSL2591_ReadLightData(I2C_HandleType *hi2c, uint8_t address);

#endif // TSL2591_LIGHT_H
