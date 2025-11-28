#include "spi.h"

// Simple delay
static void short_delay(volatile int n) { while (n--) { __asm__("nop"); } }

GPIO_InitTypeDef GPIO_InitStruct;
GPIO_HandleTypeDef CS_Handle;
SPI_HandleType spi1;

int main(void)
{   
    // Configure GPIO_InitStruct for CS pin (PD7)
    SPI_CS_Init(&CS_Handle, &GPIO_InitStruct, GPIO_D, GPIO_PIN_7);

    // Initialize SPI1 on PB3/PB4/PB5 as master, baud rate div16, full duplex
    SPI_Init(&spi1, SPI_1, SPI1_PORTB, SPI_MODE_MASTER, SPI_BAUDRATE_DIV16, SPI_DIRECTION_2LINES);

    // Simple loopback test if no device: send pattern and read back
    uint8_t loop_tx[8] = {0x55,0xAA,0x12,0x34,0xBE,0xEF,0x00,0xFF};
    uint8_t loop_rx[8] = {0};
    while(1) {
        SPI_CS_Low(&CS_Handle, GPIO_InitStruct.Pin); // Assert CS
        SPI_WriteRead(&spi1, loop_tx, loop_rx, 8);
        SPI_CS_High(&CS_Handle, GPIO_InitStruct.Pin);

        // Simple delay
        short_delay(1000000);
    }

    return 0;
}