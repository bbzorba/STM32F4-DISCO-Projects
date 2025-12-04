#include "../inc/tsl2591_light.h"

/*
 * TSL2591 SMBus read word sequence
    1. START
    2. Send slave address with write bit (0)
    3. Wait for ACK
    4. Send register address
    5. Wait for ACK
    6. RESTART
    7. Send slave address with read bit (1)
    8. Wait for ACK
    9. Enable ACK
    10. Read LSB
    11. Read MSB
    12. Disable ACK
    13. Read PEC
    14. STOP
 */

void TSL2591_Init(I2C_HandleType *hi2c, I2C_ManualTypeDef *regs, I2C_SpeedType speed)
{
    I2C_Init(hi2c, regs, speed);
}

// Read a 16-bit word from a TSL2591 register using SMBus-like sequence
uint16_t tsl_read_word(I2C_HandleType *hi2c, uint8_t address, uint8_t reg)
{
    if (!hi2c) return 0xFFFF;

    I2C_Start(hi2c);
    if (!I2C_SendAddress(hi2c, address, 0)) { // Write to select register
        I2C_Stop(hi2c);
        return 0xFFFF; // NACK on address
    }
    I2C_Write(hi2c, reg);

    I2C_Restart(hi2c);
    if (!I2C_SendAddress(hi2c, address, 1)) { // Read two data bytes (+ PEC)
        I2C_Stop(hi2c);
        return 0xFFFF; // NACK on address
    }

    I2C_EnableAck(hi2c);
    uint8_t lsb = (uint8_t)I2C_Read(hi2c);
    uint8_t msb = (uint8_t)I2C_Read(hi2c);
    I2C_DisableAck(hi2c);
    (void)I2C_Read(hi2c); // Read PEC byte (not used)

    I2C_Stop(hi2c);

    return ((uint16_t)msb << 8) | lsb;
}

// Read light data (full spectrum + IR) from TSL2591
uint32_t TSL2591_ReadLightData(I2C_HandleType *hi2c, uint8_t address)
{
    uint16_t ch0 = tsl_read_word(hi2c, address, 0x14); // ALS_DATA_0LOW
    uint16_t ch1 = tsl_read_word(hi2c, address, 0x16); // ALS_DATA_1LOW

    if (ch0 == 0xFFFF || ch1 == 0xFFFF) {
        return 0xFFFFFFFF; // Indicate error
    }

    return ((uint32_t)ch1 << 16) | ch0; // Combine into 32-bit value
}