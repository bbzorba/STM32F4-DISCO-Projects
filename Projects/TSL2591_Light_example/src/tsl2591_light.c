#include "../inc/tsl2591_light.h"

I2C_HandleType hi2c1; // define the I2C handle here

// Minimal stubs to preserve structure without HAL
void HAL_Delay(uint32_t ms)
{
    volatile uint32_t c = ms * 40000u; // rough delay for 16MHz
    while (c--) { __asm volatile ("nop"); }
}
void HAL_Init(void) {}

uint8_t TSL2591_Init(void)
{
    uint8_t enable = TSL2591_ENABLE_POWERON | TSL2591_ENABLE_AEN;
    uint8_t ctrl   = TSL2591_GAIN_MED | TSL2591_IT_100MS;

    // Write ENABLE register
    // Write ENABLE register
    I2C_Start(&hi2c1);
    if (!I2C_SendAddress(&hi2c1, TSL2591_ADDR, 0)) { I2C_Stop(&hi2c1); return 1; }
    I2C_Write(&hi2c1, TSL2591_COMMAND_BIT | TSL2591_REG_ENABLE);
    I2C_Write(&hi2c1, enable);
    I2C_Stop(&hi2c1);

    HAL_Delay(5);

    // Write CONTROL register
    I2C_Start(&hi2c1);
    if (!I2C_SendAddress(&hi2c1, TSL2591_ADDR, 0)) { I2C_Stop(&hi2c1); return 1; }
    I2C_Write(&hi2c1, TSL2591_COMMAND_BIT | TSL2591_REG_CONTROL);
    I2C_Write(&hi2c1, ctrl);
    I2C_Stop(&hi2c1);

    HAL_Delay(120);  // wait for integration

    return 0;
}

uint16_t TSL2591_ReadChannel(uint8_t reg)
{
    uint8_t data[2];

    // Write register address, then read 2 bytes
    I2C_Start(&hi2c1);
    if (!I2C_SendAddress(&hi2c1, TSL2591_ADDR, 0)) { I2C_Stop(&hi2c1); return 0; }
    I2C_Write(&hi2c1, TSL2591_COMMAND_BIT | reg);
    I2C_Restart(&hi2c1);
    if (!I2C_SendAddress(&hi2c1, TSL2591_ADDR, 1)) { I2C_Stop(&hi2c1); return 0; }
    I2C_EnableAck(&hi2c1);
    data[0] = (uint8_t)I2C_Read(&hi2c1);
    I2C_DisableAck(&hi2c1);
    data[1] = (uint8_t)I2C_Read(&hi2c1);
    I2C_Stop(&hi2c1);

    return (data[1] << 8) | data[0];
}


void I2C_Scan(void)
{
    for (uint8_t addr = 1; addr < 128; addr++) {
        I2C_Start(&hi2c1);
        int ack = I2C_SendAddress(&hi2c1, addr << 1, 0);
        I2C_Stop(&hi2c1);
        (void)ack; // avoid blocking printf; UART prints handled in main
    }
}

void MX_I2C1_Init(void)
{
    // Configure PB6=SCL, PB7=SDA with pull-ups (GPIO driver)
    GPIO_InitTypeDef gpio;
    GPIO_HandleTypeDef gpioB;
    gpio.Mode = GPIO_MODE_AF_OD; // Open-drain
    gpio.Pull = GPIO_PULLUP;
    gpio.Speed = GPIO_SPEED_HIGH;
    gpio.Alternate = GPIO_AF4_I2C1;
    gpio.Pin = GPIO_PIN_6 | GPIO_PIN_7;
    GPIO_constructor(&gpioB, GPIO_B, &gpio);
    __RCC_GPIO_CLK_ENABLE(&gpioB);
    GPIO_Init(&gpioB);

    // Initialize our I2C driver at 400kHz
    I2C_Init(&hi2c1, I2C_1, I2C_FAST_MODE);
}

void SystemClock_Config(void)
{
    // No HAL clock configuration; keep signature intact.
}