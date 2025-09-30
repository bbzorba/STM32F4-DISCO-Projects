#include "i2c.h"

#define GPIOB_MODER_ALTERNATE ((uint32_t)0x0000F000)         // mask to clear PB7/PB8 mode bits
#define GPIOB_MODER_PIN6     ((uint32_t)0x00002000)          // set AF (0b10) for PB6
#define GPIOB_MODER_PIN7     ((uint32_t)0x00008000)          // set AF (0b10) for PB7

#define GPIOB_AFR_TYPE       ((uint32_t)0xFF000000)          // AFRL for PB6 and PB7
#define GPIOB_AFR_VALUE      ((uint32_t)0x77000000)          // AF7 for USART2

void I2C1_Init(void)
{
    //1. enable clock access to I2C
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;

    //2. enable clock access to GPIOB
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

    //3. enable pins for alternate function, PB6 and PB7
    GPIOB->MODER   &= ~(GPIOB_MODER_ALTERNATE);              // Clear mode bits for PB6 and PB7
    GPIOB->MODER   |= (GPIOB_MODER_PIN6 | GPIOB_MODER_PIN7); // Set mode to Alternate Function

    //4. configure type of alternate function
    GPIOB->AFR[0]  &= ~(GPIOB_AFR_TYPE);                     // Clear AFRL for PB6 and PB7
    GPIOB->AFR[0]  |= (GPIOB_AFR_VALUE);                     // Set AF7 for I2C1

    //5. Configure I2C mode
    I2C1->CR1 |= I2C_CR1_PE | I2C_CR1_START;   // Enable I2C master
    I2C1->CR2 |= I2C_CR2_FREQ;

}

int I2C1_Read(I2C_SR1_ADDR)
{
    //1. wait until RXNE is set in the SR register
    while(!(I2C1->SR & I2C_SR1_RXNE));

    //2. read data from DR register
    return (I2C1->DR & 0xFF);
}
