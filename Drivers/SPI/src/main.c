#include "spi.h"

// Simple delay
static void short_delay(volatile int n) { while (n--) { __asm__("nop"); } }

int main(void)
{
    // SPI handle
    static SPI_TypeDef spi1_handle;

    // Init CS pin (PA4)
    SPI_CS_Init(GPIO_A, GPIO_PIN_4);

    // Initialize SPI1 on PA5/PA6/PA7 as master, baud rate div16, full duplex
    SPI_Init(&spi1_handle, SPI_1, SPI1_PORTA, SPI_MODE_MASTER, SPI_BAUDRATE_DIV16, SPI_DIRECTION_2LINES);

    // Test transfer: Read JEDEC ID (0x9F) from a typical SPI flash (if attached)
    uint8_t tx[4] = { 0x9F, 0xFF, 0xFF, 0xFF }; // command + dummy bytes
    uint8_t rx[4] = {0};

    SPI_CS_Low(GPIO_A, GPIO_PIN_4); // Assert CS
    SPI_WriteRead(&spi1_handle, tx, rx, 4);
    SPI_CS_High(GPIO_A, GPIO_PIN_4);

    // rx[1..3] would contain Manufacturer / Memory Type / Capacity for flash; store them volatile for inspection
    volatile uint8_t manuf_id = rx[1];
    volatile uint8_t mem_type  = rx[2];
    volatile uint8_t capacity  = rx[3];

    // Simple loopback test if no device: send pattern and read back
    uint8_t loop_tx[8] = {0x55,0xAA,0x12,0x34,0xBE,0xEF,0x00,0xFF};
    uint8_t loop_rx[8] = {0};
    SPI_CS_Low(GPIO_A, GPIO_PIN_4); // Assert CS
    SPI_WriteRead(&spi1_handle, loop_tx, loop_rx, 8);
    SPI_CS_High(GPIO_A, GPIO_PIN_4);

    // Keep running; could toggle CS or perform periodic reads
    while (1) {
        short_delay(1000000);
    }
    return 0;
}