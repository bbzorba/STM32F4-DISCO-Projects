#include "i2c.h"

void I2C_Init(I2C_SpeedType speed)
{
    RCC->RCC_APB1ENR |= RCC_APB1ENR_I2C2EN;                          // Enable clock access to I2C2
    RCC->RCC_AHB1ENR |= RCC_AHB1ENR_GPIOBEN;                         // Enable clock access to GPIOB
    I2C_1->CR1 |= I2C_CR1_PE;                                        // Disable I2C2
    
    GPIO_B->MODER   &= GPIOB_MODER_ALTERNATE;                        // Set PB6 and PB7 to Alternate Function
    GPIO_B->MODER   |= (GPIOB_MODER_PIN6 | GPIOB_MODER_PIN7);
    GPIO_B->AFR[0]  &= GPIOB_AFR_TYPE;                               // Clear AFRL for PB6 and PB7
    GPIO_B->AFR[0]  |= GPIOB_AFR_VALUE;                              // Set AF7 for PB6 and PB7 (I2C2)
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