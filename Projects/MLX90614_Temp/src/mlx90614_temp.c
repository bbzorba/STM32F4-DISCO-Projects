#include "mlx90614_temp.h"

void MLX90614_Init()
{
    I2C_Init(I2C_STANDARD_MODE);
}

// Read a 16-bit word from an MLX90614 register using SMBus-like sequence
static uint16_t mlx_read_word(uint8_t address, uint8_t reg)
{
    // Write register pointer
    I2C_Start();
    I2C_SendAddress(address, 0);      // write
    I2C_Write(reg);

    // Repeated start for read
    I2C_Restart();
    I2C_SendAddress(address, 1);      // read

    // Receive first byte (ACK it)
    I2C_EnableAck();
    while (!(I2C_1->SR1 & I2C_SR1_RXNE));
    uint8_t low = (uint8_t)I2C_1->DR;

    // Prepare to NACK second (last) byte
    I2C_DisableAck();
    while (!(I2C_1->SR1 & I2C_SR1_RXNE));
    uint8_t high = (uint8_t)I2C_1->DR;
    I2C_Stop();

    return (uint16_t)((uint16_t)low | ((uint16_t)high << 8));
}

int read_obj_temp(uint8_t address)
{
    // Object temperature 1 register = 0x07
    uint16_t raw = mlx_read_word(address, 0x07);
    // Convert to Celsius using integer math: T(°C) = raw * 0.02 - 273.15
    // Scale by 100 to avoid float: T_x100 = raw*2 - 27315
    int32_t t_x100 = ((int32_t)raw * 2) - 27315;
    // Round to nearest integer Celsius
    int temp_c = (t_x100 >= 0) ? (int)((t_x100 + 50) / 100) : (int)((t_x100 - 50) / 100);
    return temp_c;
}

int read_amb_temp(uint8_t address)
{
    // Ambient temperature register = 0x06
    uint16_t raw = mlx_read_word(address, 0x06);
    // Convert to Celsius using integer math: T(°C) = raw * 0.02 - 273.15
    // Scale by 100 to avoid float: T_x100 = raw*2 - 27315
    int32_t t_x100 = ((int32_t)raw * 2) - 27315;
    // Round to nearest integer Celsius
    int temp_c = (t_x100 >= 0) ? (int)((t_x100 + 50) / 100) : (int)((t_x100 - 50) / 100);
    return temp_c;
}