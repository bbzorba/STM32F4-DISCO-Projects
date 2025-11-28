#include "spi.h"

// Simple delay
static void short_delay(volatile int n) { while (n--) { __asm__("nop"); } }

GPIO_InitTypeDef GPIO_InitStruct;

int main(void)
{   
    // Configure GPIO_InitStruct for output pins
    GPIO_InitStruct.Pin = GPIO_PIN_7; // CS pin
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;  // Push-pull output mode
    GPIO_InitStruct.Pull = GPIO_NOPULL;          // No pull-up/pull-down
    GPIO_InitStruct.Speed = GPIO_SPEED_MEDIUM;   // Medium speed
    
    GPIO_HandleTypeDef CS_Handle;
    GPIO_constructor(&CS_Handle, GPIO_D, &GPIO_InitStruct);

    SPI_HandleType spi1;

    // Init CS pin (PD7)
    SPI_CS_Init(&CS_Handle, GPIO_InitStruct.Pin);

    // Initialize SPI1 on PB3/PB4/PB5 as master, baud rate div16, full duplex
    SPI_Init(&spi1, SPI_1, SPI1_PORTB, SPI_MODE_MASTER, SPI_BAUDRATE_DIV16, SPI_DIRECTION_2LINES);

    // Test transfer: Read JEDEC ID (0x9F) from a typical SPI flash (if attached)
    uint8_t tx[4] = { 0x9F, 0xFF, 0xFF, 0xFF }; // command + dummy bytes
    uint8_t rx[4] = {0};

    SPI_CS_Low(&CS_Handle, GPIO_InitStruct.Pin); // Assert CS
    SPI_WriteRead(&spi1, tx, rx, 4);
    SPI_CS_High(&CS_Handle, GPIO_InitStruct.Pin);

    // Simple loopback test if no device: send pattern and read back
    uint8_t loop_tx[8] = {0x55,0xAA,0x12,0x34,0xBE,0xEF,0x00,0xFF};
    uint8_t loop_rx[8] = {0};
    SPI_CS_Low(&CS_Handle, GPIO_InitStruct.Pin); // Assert CS
    SPI_WriteRead(&spi1, loop_tx, loop_rx, 8);
    SPI_CS_High(&CS_Handle, GPIO_InitStruct.Pin);

    // Periodically send random bytes over MOSI for analyzer visibility
    uint32_t lfsr = 0xACE1u; // simple 16-bit LFSR
    while (1) {
        uint8_t buf[16];
        for (int i = 0; i < 16; ++i) {
            // X^16 + X^14 + X^13 + X^11 taps
            uint32_t bit = ((lfsr >> 0) ^ (lfsr >> 2) ^ (lfsr >> 3) ^ (lfsr >> 5)) & 1u;
            lfsr = (lfsr >> 1) | (bit << 15);
            buf[i] = (uint8_t)(lfsr & 0xFF);
        }
        SPI_CS_Low(&CS_Handle, GPIO_InitStruct.Pin);
        SPI_WriteRead(&spi1, buf, NULL, sizeof(buf)); // write-only, ignore RX
        SPI_CS_High(&CS_Handle, GPIO_InitStruct.Pin);
        short_delay(200000);
    }
    return 0;
}