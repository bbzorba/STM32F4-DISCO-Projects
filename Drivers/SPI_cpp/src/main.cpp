#include "../inc/spi.h"

// Simple delay
static void short_delay(volatile int n) { while (n--) { __asm__("nop"); } }

GPIO_InitTypeDef GPIO_InitStruct;

int main(void)
{   
    // Configure GPIO_InitStruct for CS pin (PA4)
    GPIO_InitStruct.Pin = GPIO_PIN_4; // CS pin
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;  // Push-pull output mode
    GPIO_InitStruct.Pull = GPIO_NOPULL;          // No pull-up/pull-down
    GPIO_InitStruct.Speed = GPIO_SPEED_MEDIUM;   // Medium speed
    GPIO GPIO_CS(GPIO_A, &GPIO_InitStruct);

    // Initialize SPI1 on PA5/PA6/PA7 as master, baud rate div16, full duplex
    SPI spi1(SPI_1, SPI1_PORTA, SPI_MODE_MASTER, SPI_BAUDRATE_DIV16, SPI_DIRECTION_2LINES, SPI_CLOCK_POL_LOW_PHASE_1EDGE);

    spi1.SPI_CS_High(GPIO_CS, GPIO_InitStruct.Pin); // Set high (not selected)

    // Simple loopback test if no device: send pattern and read back
    uint8_t loop_tx[8] = {0x55,0xAA,0x12,0x34,0xBE,0xEF,0x00,0xFF};
    uint8_t loop_rx[8] = {0};
    while(1) {
        spi1.SPI_CS_Low(GPIO_CS, GPIO_InitStruct.Pin); // Assert CS
        spi1.SPI_WriteRead(loop_tx, loop_rx, 8);
        spi1.SPI_CS_High(GPIO_CS, GPIO_InitStruct.Pin);

        short_delay(1000000);

        spi1.SPI_WriteReg(GPIO_CS, GPIO_InitStruct.Pin, 0x0C, 0x5C); // Example write
        short_delay(1000);

        uint8_t reg_val = spi1.SPI_ReadReg(GPIO_CS, GPIO_InitStruct.Pin, 0x0F, 0x80); // Example read from WHO_AM_I

        // Simple delay
        short_delay(1000000);
    }

    return 0;
}