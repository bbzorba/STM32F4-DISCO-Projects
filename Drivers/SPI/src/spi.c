#include "../inc/spi.h"

void SPI_Init(SPI_HandleType *spi, SPI_ManualType *regs, SPI_PinConfigType _pinConfig, SPI_ModeType _mode, SPI_BaudRateType _baudrate, SPI_DirectionType _direction)
{
    spi->regs = regs;
    spi->pinConfig = _pinConfig;
    spi->mode = _mode;
    spi->baudrate = _baudrate;
    spi->direction = _direction;

    // Enable SPI clock & configure pins

    // configuration for SPI1 SCK on PA5, MISO on PA6, MOSI on PA7
    if (regs == SPI_1 && _pinConfig == SPI1_PORTA) {
        RCC->APB2ENR |= RCC_APB2ENR_SPI_1EN;
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
        GPIO_A->MODER &= ~(MODER_PIN5_MASK | MODER_PIN6_MASK | MODER_PIN7_MASK);
        GPIO_A->MODER |=  (MODER_PIN5_SET | MODER_PIN6_SET | MODER_PIN7_SET);
        GPIO_A->AFR[0] &= ~(AFRL_PIN5_MASK | AFRL_PIN6_MASK | AFRL_PIN7_MASK);
        GPIO_A->AFR[0] |=  (AFRL_PIN5_SET_AF5 | AFRL_PIN6_SET_AF5 | AFRL_PIN7_SET_AF5);
    }

    // configuration for SPI1 SCK on PB3, MISO on PB4, MOSI on PB5 
    else if (regs == SPI_1 && _pinConfig == SPI1_PORTB) {
        RCC->APB2ENR |= RCC_APB2ENR_SPI_1EN;
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
        GPIO_B->MODER &= ~(MODER_PIN3_MASK | MODER_PIN4_MASK | MODER_PIN5_MASK);
        GPIO_B->MODER |=  (MODER_PIN3_SET | MODER_PIN4_SET | MODER_PIN5_SET);
        GPIO_B->AFR[0] &= ~(AFRL_PIN3_MASK | AFRL_PIN4_MASK | AFRL_PIN5_MASK);
        GPIO_B->AFR[0] |=  (AFRL_PIN3_SET_AF5 | AFRL_PIN4_SET_AF5 | AFRL_PIN5_SET_AF5);
    }

    // configuration for SPI2 SCK on PB13, MISO on PB14, MOSI on PB15
    else if (regs == SPI_2 && _pinConfig == SPI2_PORTB) {
        RCC->APB1ENR |= RCC_APB1ENR_SPI_2EN;
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
        GPIO_B->MODER &= ~(MODER_PIN13_MASK | MODER_PIN14_MASK | MODER_PIN15_MASK);
        GPIO_B->MODER |=  (MODER_PIN13_SET | MODER_PIN14_SET | MODER_PIN15_SET);
        GPIO_B->AFR[1] &= ~(AFRH_PIN13_MASK | AFRH_PIN14_MASK | AFRH_PIN15_MASK);
        GPIO_B->AFR[1] |=  (AFRH_PIN13_SET_AF5 | AFRH_PIN14_SET_AF5 | AFRH_PIN15_SET_AF5);
    } 
    
    // configuration for SPI2 SCK on PB13, MISO on PC2, MOSI on PC3
    else if (regs == SPI_2 && _pinConfig == SPI2_PORTC) {
        RCC->APB1ENR |= RCC_APB1ENR_SPI_2EN;
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN; // Enable GPIOB clock for SCK
        GPIO_B->MODER &= ~MODER_PIN13_MASK; // SCK on PB13
        GPIO_C->MODER &= ~(MODER_PIN2_MASK | MODER_PIN3_MASK); // MISO on PC2, MOSI on PC3
        GPIO_C->MODER |=  (MODER_PIN2_SET | MODER_PIN3_SET);
        GPIO_B->MODER |=  MODER_PIN13_SET; // Set AF for PB13
        GPIO_C->AFR[0] &= ~(AFRL_PIN2_MASK | AFRL_PIN3_MASK);
        GPIO_C->AFR[0] |=  (AFRL_PIN2_SET_AF5 | AFRL_PIN3_SET_AF5);
        GPIO_B->AFR[1] &= ~AFRH_PIN13_MASK;
        GPIO_B->AFR[1] |= AFRH_PIN13_SET_AF5;
    }

    // configuration for SPI3 SCK on PB3, MISO on PB4, MOSI on PB5
    else if (regs == SPI_3 && _pinConfig == SPI3_PORTB) {
        RCC->APB1ENR |= RCC_APB1ENR_SPI_3EN;
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
        GPIO_B->MODER &= ~(MODER_PIN3_MASK | MODER_PIN4_MASK | MODER_PIN5_MASK);
        GPIO_B->MODER |=  (MODER_PIN3_SET | MODER_PIN4_SET | MODER_PIN5_SET);
        GPIO_B->AFR[0] &= ~(AFRL_PIN3_MASK | AFRL_PIN4_MASK | AFRL_PIN5_MASK);
        GPIO_B->AFR[0] |=  (AFRL_PIN3_SET_AF6 | AFRL_PIN4_SET_AF6 | AFRL_PIN5_SET_AF6);
    }

    // configuration for SPI3 SCK on PC10, MISO on PC11, MOSI on PC12
    else if (regs == SPI_3 && _pinConfig == SPI3_PORTC) {
        RCC->APB1ENR |= RCC_APB1ENR_SPI_3EN;
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
        GPIO_C->MODER &= ~(MODER_PIN10_MASK | MODER_PIN11_MASK | MODER_PIN12_MASK);
        GPIO_C->MODER |=  (MODER_PIN10_SET | MODER_PIN11_SET | MODER_PIN12_SET);
        GPIO_C->AFR[1] &= ~(AFRH_PIN10_MASK | AFRH_PIN11_MASK | AFRH_PIN12_MASK);
        GPIO_C->AFR[1] |=  (AFRH_PIN10_SET_AF6 | AFRH_PIN11_SET_AF6 | AFRH_PIN12_SET_AF6);
    }

    // Configure SPI_CR1
    uint32_t cr1 = 0;
    cr1 |= (_mode == SPI_MODE_MASTER) ? SPI_CR1_MSTR : 0;
    cr1 |= ((uint32_t)_baudrate << 3) & SPI_CR1_BR;
    cr1 |= (_direction == SPI_DIRECTION_2LINES) ? 0 : SPI_CR1_BIDIMODE;
    cr1 |= SPI_CR1_SSM | SPI_CR1_SSI; // Software slave management
    regs->CR1 = cr1;

    // Enable SPI
    regs->CR1 |= SPI_CR1_SPE;
}


int SPI_WriteRead(SPI_HandleType *spi, const uint8_t *txData, uint8_t *rxData, size_t length)
{
    if (!spi || !spi->regs || length == 0) return -1;

    for (size_t i = 0; i < length; i++) {
        // Wait until TXE (bit1) set
        while (!(spi->regs->SR & 0x02));
        // In 1-line bidirectional transmit ensure BIDIOE set
        if ((spi->direction == SPI_DIRECTION_1LINE) && !(spi->regs->CR1 & SPI_CR1_BIDIOE)) {
            spi->regs->CR1 |= SPI_CR1_BIDIOE;
        }
        spi->regs->DR = txData ? txData[i] : 0xFF;

        // Wait until RXNE (bit0) set (for 2-line or RX-only)
        while (!(spi->regs->SR & 0x01));
        uint8_t r = (uint8_t)spi->regs->DR;
        if (rxData) rxData[i] = r;

        // Clear OVR if set (read SR then DR per RM)
        if (spi->regs->SR & (1 << 6)) { // OVR bit position 6
            (void)spi->regs->SR;
            (void)spi->regs->DR;
        }
    }

    // Wait for BSY (bit7) to clear to ensure last clock finished
    while (spi->regs->SR & (1 << 7));

    // If 1-line and we were transmitting, optionally switch back to input
    if (spi->direction == SPI_DIRECTION_1LINE && (spi->regs->CR1 & SPI_CR1_BIDIOE)) {
        spi->regs->CR1 &= ~SPI_CR1_BIDIOE; // Return to receive if desired
    }

    return 0;
}

void SPI_DeInit(SPI_HandleType *spi)
{
    if (!spi || !spi->regs) return;
    spi->regs->CR1 &= ~SPI_CR1_SPE;

    if (spi->regs == SPI_1) {
        RCC->APB2ENR &= ~RCC_APB2ENR_SPI_1EN;
    } else if (spi->regs == SPI_2) {
        RCC->APB1ENR &= ~RCC_APB1ENR_SPI_2EN;
    } else if (spi->regs == SPI_3) {
        RCC->APB1ENR &= ~RCC_APB1ENR_SPI_3EN;
    }
}

void SPI_SetMode(SPI_HandleType *spi, uint8_t mode) {
	// mode: 0..3
	spi->regs->CR1 &= ~(SPI_CR1_CPOL | SPI_CR1_CPHA);
	if (mode & 0x02) spi->regs->CR1 |= SPI_CR1_CPOL;
	if (mode & 0x01) spi->regs->CR1 |= SPI_CR1_CPHA;
}

void SPI_CS_Low(GPIO_HandleTypeDef *GPIOx, uint16_t CS_pin)  { GPIO_ResetPin(GPIOx, CS_pin); }
void SPI_CS_High(GPIO_HandleTypeDef *GPIOx, uint16_t CS_pin) { GPIO_SetPin(GPIOx, CS_pin); }