#include "../inc/tsl2591_light.h"

void TSL2591_Init(I2C_HandleType *hi2c, I2C_ManualTypeDef *regs, I2C_SpeedType speed)
{
    I2C_Init(hi2c, regs, speed);
}

// Initialize the TSL2591 sensor
uint8_t TSL2591_Begin(I2C_HandleType *hi2c)
{
    // Check if we can communicate with the sensor by reading the device ID
    uint8_t device_id = tsl_read_byte(hi2c, TSL2591_ADDR, TSL2591_REGISTER_DEVICE_ID);
    if (device_id != 0x50) { // TSL2591 device ID should be 0x50
        return 0; // Failed to detect sensor
    }
    
    // Enable the sensor with default settings
    TSL2591_Enable(hi2c);
    TSL2591_SetGainAndTime(hi2c, TSL2591_CONTROL_AGAIN_MED, TSL2591_CONTROL_ATIME_200MS);
    
    return 1; // Success
}

// Enable the TSL2591 sensor
void TSL2591_Enable(I2C_HandleType *hi2c)
{
    tsl_write_byte(hi2c, TSL2591_ADDR, TSL2591_REGISTER_ENABLE, 
                   TSL2591_ENABLE_POWERON | TSL2591_ENABLE_AEN);
}

// Disable the TSL2591 sensor
void TSL2591_Disable(I2C_HandleType *hi2c)
{
    tsl_write_byte(hi2c, TSL2591_ADDR, TSL2591_REGISTER_ENABLE, TSL2591_ENABLE_POWEROFF);
}

// Set gain and integration time
void TSL2591_SetGainAndTime(I2C_HandleType *hi2c, uint8_t gain, uint8_t time)
{
    tsl_write_byte(hi2c, TSL2591_ADDR, TSL2591_REGISTER_CONTROL, gain | time);
}

// Read a single byte from a TSL2591 register
uint8_t tsl_read_byte(I2C_HandleType *hi2c, uint8_t address, uint8_t reg)
{
    if (!hi2c) return 0xFF;

    uint8_t command_reg = TSL2591_COMMAND_BIT | reg;

    I2C_Start(hi2c);
    if (!I2C_SendAddress(hi2c, address, 0)) {
        I2C_Stop(hi2c);
        return 0xFF;
    }
    I2C_Write(hi2c, command_reg);

    I2C_Restart(hi2c);
    if (!I2C_SendAddress(hi2c, address, 1)) {
        I2C_Stop(hi2c);
        return 0xFF;
    }

    I2C_DisableAck(hi2c);
    uint8_t data = (uint8_t)I2C_Read(hi2c);
    I2C_Stop(hi2c);

    return data;
}

// Write a single byte to a TSL2591 register
void tsl_write_byte(I2C_HandleType *hi2c, uint8_t address, uint8_t reg, uint8_t value)
{
    if (!hi2c) return;

    uint8_t command_reg = TSL2591_COMMAND_BIT | reg;

    I2C_Start(hi2c);
    if (!I2C_SendAddress(hi2c, address, 0)) {
        I2C_Stop(hi2c);
        return;
    }
    I2C_Write(hi2c, command_reg);
    I2C_Write(hi2c, value);
    I2C_Stop(hi2c);
}

// Read a 16-bit word from a TSL2591 register
uint16_t tsl_read_word(I2C_HandleType *hi2c, uint8_t address, uint8_t reg)
{
    if (!hi2c) return 0xFFFF;

    uint8_t command_reg = TSL2591_COMMAND_BIT | TSL2591_WORD_BIT | reg;

    I2C_Start(hi2c);
    if (!I2C_SendAddress(hi2c, address, 0)) {
        I2C_Stop(hi2c);
        return 0xFFFF;
    }
    I2C_Write(hi2c, command_reg);

    I2C_Restart(hi2c);
    if (!I2C_SendAddress(hi2c, address, 1)) {
        I2C_Stop(hi2c);
        return 0xFFFF;
    }

    I2C_EnableAck(hi2c);
    uint8_t lsb = (uint8_t)I2C_Read(hi2c);
    I2C_DisableAck(hi2c);
    uint8_t msb = (uint8_t)I2C_Read(hi2c);
    I2C_Stop(hi2c);

    return ((uint16_t)msb << 8) | lsb;
}

// Read light data (full spectrum + IR) from TSL2591
uint32_t TSL2591_ReadLightData(I2C_HandleType *hi2c, uint8_t address)
{
    uint16_t ch0 = tsl_read_word(hi2c, address, TSL2591_REGISTER_CHAN0_LOW);
    uint16_t ch1 = tsl_read_word(hi2c, address, TSL2591_REGISTER_CHAN1_LOW);

    if (ch0 == 0xFFFF || ch1 == 0xFFFF) {
        return 0xFFFFFFFF; // Indicate error
    }

    return ((uint32_t)ch1 << 16) | ch0; // Combine into 32-bit value
}