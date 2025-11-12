#include "bme68x_env_sensor.h"

void BME68x_Init()
{
    I2C_Init(I2C_STANDARD_MODE);
}

// Read a 16-bit word from a BME68x register using SMBus-like sequence
static uint16_t bme68x_read_word(uint8_t address, uint8_t reg)
{
    uint8_t low, high;
    // Write phase: send command register
    I2C_Start();
    I2C_SendAddress(address, 0); // write
    I2C_Write(reg);

    // Read phase: repeated start then 2-byte read (low then high)
    I2C_Restart();
    I2C_SendAddress(address, 1); // read

    // For 2-byte reception on F4, disable ACK before last byte and use BTF
    I2C_DisableAck();
    // Wait until two bytes are received (BTF = 1)
    while (!(I2C_1->SR1 & I2C_SR1_BTF));
    // Generate STOP then read both bytes
    I2C_Stop();
    low  = (uint8_t)I2C_1->DR;
    high = (uint8_t)I2C_1->DR;
    // Re-enable ACK for future receptions
    I2C_EnableAck();

    return (uint16_t)((uint16_t)low | ((uint16_t)high << 8));
}

int read_temp(uint8_t address)
{
    // Temperature register = 0x22 (example)
    uint16_t raw = bme68x_read_word(address, 0x22);
    // Convert raw value to temperature in Celsius (example conversion)
    int temp_c = (int)(raw / 100); // Placeholder conversion
    return temp_c;
}

int read_gas_density(uint8_t address)
{
    // Gas density register = 0x2C (example)
    uint16_t raw = bme68x_read_word(address, 0x2C);
    // Convert raw value to gas density in hPa (example conversion)
    int density_hPa = (int)(raw / 100); // Placeholder conversion
    return density_hPa;
}

int read_humidity(uint8_t address)
{
    // Humidity register = 0x25 (example)
    uint16_t raw = bme68x_read_word(address, 0x25);
    // Convert raw value to relative humidity in % (example conversion)
    int humidity_percent = (int)(raw / 100); // Placeholder conversion
    return humidity_percent;
}