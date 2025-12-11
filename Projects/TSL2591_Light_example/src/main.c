#include "../inc/tsl2591_light.h"

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_I2C1_Init();

    USART_HandleType huart;
    // Use USART2 (PA2 TX, PA3 RX) which is commonly wired to CP210x
    USART_constructor(&huart, USART_2, RX_AND_TX, __115200);
    HAL_Delay(20);
    USART_WriteString(&huart, "USART2 initialized for debug output.\n");

    USART_WriteString(&huart, "Scanning I2C bus...\n");
    I2C_Scan();

    if (TSL2591_Init() == HAL_OK)
        USART_WriteString(&huart, "TSL2591 initialized successfully!\n");
    else
        USART_WriteString(&huart, "TSL2591 initialization failed!\n");

    while (1)
    {
        uint16_t ch0 = TSL2591_ReadChannel(TSL2591_REG_CHAN0_L);
        USART_WriteString(&huart, "Reading light data...\n");
        uint16_t ch1 = TSL2591_ReadChannel(TSL2591_REG_CHAN1_L);
        USART_WriteString(&huart, "Done.\n");

        char buffer[64];
        snprintf(buffer, sizeof(buffer), "CH0: %u  CH1: %u\n", ch0, ch1);
        USART_WriteString(&huart, buffer);
        HAL_Delay(500);
    }
}

