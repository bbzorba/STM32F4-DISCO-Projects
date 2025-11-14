#include "i2c.h"

static void short_delay(volatile int n) { while (n--) { __asm__("nop"); } }

void I2C_BusRecover(void)
{
    // Ensure GPIOB clock is enabled
    RCC->RCC_AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

    // Use PB6 (SCL) and PB9 (SDA) for recovery (HAL-style mapping for I2C1)
    // Configure as GPIO open-drain outputs with pull-ups
    GPIO_B->MODER &= ~(0x3U << (6*2));
    GPIO_B->MODER |=  (0x1U << (6*2));
    GPIO_B->MODER &= ~(0x3U << (9*2));
    GPIO_B->MODER |=  (0x1U << (9*2));
    GPIO_B->OTYPER |= (1U << 6) | (1U << 9);
    GPIO_B->PUPDR &= ~((0x3U << (6*2)) | (0x3U << (9*2)));
    GPIO_B->PUPDR |=  ((0x1U << (6*2)) | (0x1U << (9*2)));
    // Release lines high
    GPIO_B->ODR |= (1U << 6) | (1U << 9);
    short_delay(1000);

    // If SDA is stuck low, toggle SCL up to 9 pulses
    for (int i = 0; i < 9; ++i) {
        if (GPIO_B->IDR & (1U << 9)) {
            break; // SDA released
        }
        GPIO_B->ODR &= ~(1U << 6);
        short_delay(400);
        GPIO_B->ODR |= (1U << 6);
        short_delay(400);
    }

    // Manual STOP: SDA low while SCL high then release SDA
    GPIO_B->ODR |= (1U << 6); // ensure SCL high
    short_delay(200);
    GPIO_B->ODR &= ~(1U << 9); // SDA low
    short_delay(200);
    GPIO_B->ODR |= (1U << 9);  // SDA high
    short_delay(400);
}

void I2C_Init(I2C_SpeedType speed)
{
    // Enable clocks
    RCC->RCC_APB1ENR |= RCC_APB1ENR_I2C1EN;
    RCC->RCC_AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

    // Ensure peripheral disabled and perform a software reset (HAL-like)
    I2C_1->CR1 &= ~I2C_CR1_PE;
    I2C_1->CR1 |= I2C_CR1_SWRST;
    short_delay(1000);
    I2C_1->CR1 &= ~I2C_CR1_SWRST;

    // Try to recover the bus in case SCL/SDA are stuck low
    I2C_BusRecover();

    // Configure PB6 (SCL) and PB9 (SDA) as AF4 for I2C1
    GPIO_B->MODER   &= ~(MODER_PIN6_MASK | MODER_PIN9_MASK);         // Clear mode bits
    GPIO_B->MODER   |= (MODER_PIN6_SET | MODER_PIN9_SET);            // Alternate function
    GPIO_B->AFR[0]  &= ~(AFRL_PIN6_MASK);                            // Clear AFRL for PB6
    GPIO_B->AFR[0]  |= (AFRL_PIN6_SET_AF4);                          // Set AF4 for PB6
    GPIO_B->AFR[1]  &= ~(AFRH_PIN9_MASK);                            // Clear AFRH for PB9
    GPIO_B->AFR[1]  |= (AFRH_PIN9_SET_AF4);                          // Set AF4 for PB9
    GPIO_B->OTYPER  |= (1U << 6) | (1U << 9);                        // Open-drain
    GPIO_B->OSPEEDR |= (0x2U << (6*2)) | (0x2U << (9*2));            // Medium speed
    GPIO_B->PUPDR   &= ~((0x3U << (6*2)) | (0x3U << (9*2)));         // Clear pulls
    GPIO_B->PUPDR   |=  ((0x1U << (6*2)) | (0x1U << (9*2)));         // Pull-up on SCL/SDA

    // Timing configuration (assumes APB1 ~16 MHz unless system clock changed)
    I2C_1->CR2 = I2C_CR2_FREQ;                                       // Peripheral clock frequency (MHz)
    if (speed == I2C_STANDARD_MODE)
    {
        I2C_1->CCR = 0x28;                                           // Standard mode, 100kHz
        I2C_1->TRISE = 0x09;                                         // Maximum rise time
    }
    else if (speed == I2C_FAST_MODE)
    {
        I2C_1->CCR = 0x0D;                                           // Fast mode, 400kHz
        I2C_1->TRISE = 0x03;                                         // Maximum rise time
    }

    // Own address register (bit 14 must be kept at 1 in 7-bit mode per RM)
    I2C_1->OAR1 = 0x4000;                                            // 7-bit, address 0

    // Enable peripheral
    I2C_1->CR1 |= I2C_CR1_PE;
}

int read_I2C_address(int address)
{
    //1. wait until BUSY is reset in the SR2 register
    while(I2C_1->SR2 & I2C_SR2_BUSY);

    //2. generate START condition
    I2C_1->CR1 |= I2C_CR1_START;

    //3. wait until SB is set in the SR1 register
    while(!(I2C_1->SR1 & I2C_SR1_SB));

    //4. send address (left aligned) + R/W bit (1 for read)
    I2C_1->DR = (address << 1) | 1;

    //5. wait until ADDR is set in the SR1 register
    while(!(I2C_1->SR1 & I2C_SR1_ADDR));

    //6. clear ADDR by reading SR1 and SR2 registers
    volatile uint32_t temp = I2C_1->SR1;
    temp = I2C_1->SR2;
    (void)temp; // Prevent unused variable warning

    return 0;
}

int I2C_Read(void)
{
    //1. wait until RXNE is set in the SR register
    while(!(I2C_1->SR1 & I2C_SR1_RXNE));

    //2. read data from DR register
    return (I2C_1->DR & 0xFF);
}

void I2C_Write(int data)
{
    //1. wait until TXE is set in the SR register
    while(!(I2C_1->SR1 & I2C_SR1_TXE));

    //2. write to DR register
    I2C_1->DR = (data & 0xFF);
}

void I2C_Stop(void)
{
    // Generate STOP condition and wait for bus to become free
    I2C_1->CR1 |= I2C_CR1_STOP;
    // Optionally poll BUSY flag clear (simple delay loop here)
    for(volatile int i=0; i<1000; ++i) { __asm__("nop"); }
}

void I2C_Start(void)
{
    // Wait until bus is free then generate START and wait for SB
    while (I2C_1->SR2 & I2C_SR2_BUSY);
    I2C_1->CR1 |= I2C_CR1_START;
    while (!(I2C_1->SR1 & I2C_SR1_SB));
}

void I2C_Restart(void)
{
    // Generate repeated START and wait for SB
    I2C_1->CR1 |= I2C_CR1_START;
    while (!(I2C_1->SR1 & I2C_SR1_SB));
}

int I2C_SendAddress(uint8_t address, int read)
{
    // Send 7-bit address and R/W bit
    I2C_1->DR = (uint32_t)((address << 1) | (read ? 1 : 0));
    // Wait for either ADDR (ACK) or AF (NACK)
    while (1) {
        uint32_t sr1 = I2C_1->SR1;
        if (sr1 & I2C_SR1_ADDR) {
            // Clear ADDR by reading SR1 then SR2
            volatile uint32_t tmp = I2C_1->SR1; (void)tmp;
            tmp = I2C_1->SR2; (void)tmp;
            return 1; // ACK
        }
        if (sr1 & I2C_SR1_AF) {
            // Clear AF and generate STOP to release bus
            I2C_1->SR1 &= ~I2C_SR1_AF;
            I2C_1->CR1 |= I2C_CR1_STOP;
            return 0; // NACK
        }
    }
}

void I2C_EnableAck(void)
{
    I2C_1->CR1 |= I2C_CR1_ACK;
}

void I2C_DisableAck(void)
{
    I2C_1->CR1 &= ~I2C_CR1_ACK;
}