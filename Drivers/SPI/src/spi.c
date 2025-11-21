#include "spi.h"

static void short_delay(volatile int n) { while (n--) { __asm__("nop"); } }

void SPI_Init(SPI_Manual_TypeDef* const spi, SPI_ModeType mode, SPI_BaudRateType baudrate, SPI_DirectionType direction)
{
    // Enable SPI clock
    if (spi == SPI_1) {
        RCC->APB2ENR |= RCC_APB2ENR_SPI_1EN;
    } else if (spi == SPI_2) {
        RCC->APB1ENR |= RCC_APB1ENR_SPI_2EN;
    } else if (spi == SPI_3) {
        RCC->APB1ENR |= RCC_APB1ENR_SPI_3EN;
    }

    // Configure SPI_CR1
    uint32_t cr1 = 0;
    cr1 |= (mode == SPI_MODE_MASTER) ? SPI_CR1_MSTR : 0;
    cr1 |= ((uint32_t)baudrate << 3) & SPI_CR1_BR;
    cr1 |= (direction == SPI_DIRECTION_2LINES) ? 0 : SPI_CR1_BIDIMODE;
    cr1 |= SPI_CR1_SSM | SPI_CR1_SSI; // Software slave management
    spi->CR1 = cr1;

    // Enable SPI
    spi->CR1 |= SPI_CR1_SPE;
}

void SPI_WriteRead(SPI_Manual_TypeDef* const spi, uint8_t* txData, uint8_t* rxData, size_t length)
{
    for (size_t i = 0; i < length; i++) {
        // Wait until TXE (Transmit buffer empty)
        while (!(spi->SR & 0x02));
        // Send data
        spi->DR = txData ? txData[i] : 0xFF;

        // Wait until RXNE (Receive buffer not empty)
        while (!(spi->SR & 0x01));
        // Read data
        if (rxData) {
            rxData[i] = (uint8_t)(spi->DR);
        } else {
            volatile uint8_t dummy = (uint8_t)(spi->DR); // Read to clear RXNE
            (void)dummy; // Suppress unused variable warning
        }
    }
}

void SPI_DeInit(SPI_Manual_TypeDef* const spi)
{
    // Disable SPI
    spi->CR1 &= ~SPI_CR1_SPE;

    // Disable SPI clock
    if (spi == SPI_1) {
        RCC->APB2ENR &= ~RCC_APB2ENR_SPI_1EN;
    } else if (spi == SPI_2) {
        RCC->APB1ENR &= ~RCC_APB1ENR_SPI_2EN;
    } else if (spi == SPI_3) {
        RCC->APB1ENR &= ~RCC_APB1ENR_SPI_3EN;
    }
}