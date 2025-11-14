#include "i2c.h"

void I2C_Init(I2C_SpeedType speed)
{
    RCC->RCC_APB1ENR |= RCC_APB1ENR_I2C1EN;                          // Enable clock access to I2C1
    RCC->RCC_AHB1ENR |= RCC_AHB1ENR_GPIOBEN;                         // Enable clock access to GPIOB
    I2C_1->CR1 &= ~I2C_CR1_PE;                                       // Disable I2C1 before configuring

    // Configure PB6/PB7 as AF4 for I2C1 (SCL/SDA), open-drain, medium speed
    GPIO_B->MODER   &= ~(MODER_PIN6_MASK | MODER_PIN7_MASK);         // Clear mode bits
    GPIO_B->MODER   |= (MODER_PIN6_SET | MODER_PIN7_SET);            // Alternate function mode
    GPIO_B->AFR[0]  &= ~(AFRL_PIN6_MASK | AFRL_PIN7_MASK);           // Clear AFRL for PB6/PB7
    GPIO_B->AFR[0]  |= (AFRL_PIN6_SET_AF4 | AFRL_PIN7_SET_AF4);      // AF4 = I2C
    GPIO_B->OTYPER  |= (1U << 6) | (1U << 7);                        // Open-drain for SCL/SDA
    GPIO_B->OSPEEDR |= (0x2U << (6*2)) | (0x2U << (7*2));            // Medium speed
    I2C_1->CR2 = I2C_CR2_FREQ;                                       // Peripheral clock frequency
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

    I2C_1->CR1 |= I2C_CR1_PE;                                        // Enable I2C1
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

void I2C_SendAddress(uint8_t address, int read)
{
    // Send 7-bit address and R/W bit
    I2C_1->DR = (uint32_t)((address << 1) | (read ? 1 : 0));
    while (!(I2C_1->SR1 & I2C_SR1_ADDR));
    // Clear ADDR by reading SR1 and SR2
    volatile uint32_t tmp = I2C_1->SR1; (void)tmp;
    tmp = I2C_1->SR2; (void)tmp;
}

void I2C_EnableAck(void)
{
    I2C_1->CR1 |= I2C_CR1_ACK;
}

void I2C_DisableAck(void)
{
    I2C_1->CR1 &= ~I2C_CR1_ACK;
}