#include "../inc/i2c.h"

static void short_delay(volatile int n) { while (n--) { __asm__("nop"); } }

void I2C_Init(I2C_HandleType *hi2c, I2C_ManualTypeDef *regs, I2C_SpeedType speed)
{
    hi2c->regs = regs;
    hi2c->speed = speed;
    
    if (regs == I2C_1) {
        RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
        // Configure PB6 (SCL) and PB7 (SDA) as AF4 for I2C1
        GPIO_B->MODER   &= ~(MODER_PIN6_MASK | MODER_PIN7_MASK);            // Clear mode bits
        GPIO_B->MODER   |= (MODER_PIN6_SET | MODER_PIN7_SET);               // Alternate function
        GPIO_B->AFR[0]  &= ~(AFRL_PIN6_MASK | AFRL_PIN7_MASK);              // Clear AFRL for PB6 and PB7
        GPIO_B->AFR[0]  |= (AFRL_PIN6_SET_AF4 | AFRL_PIN7_SET_AF4);         // Set AF4 for PB6 and PB7
    }
    else if (regs == I2C_2) {
        RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
        // Configure PB10 (SCL) and PB11 (SDA) as AF4 for I2C2
        GPIO_B->MODER   &= ~(MODER_PIN10_MASK | MODER_PIN11_MASK);          // Clear mode bits
        GPIO_B->MODER   |= (MODER_PIN10_SET | MODER_PIN11_SET);             // Alternate function
        GPIO_B->AFR[1]  &= ~(AFRH_PIN10_MASK | AFRH_PIN11_MASK);            // Clear AFRH for PB10 and PB11
        GPIO_B->AFR[1]  |= (AFRH_PIN10_SET_AF4 | AFRH_PIN11_SET_AF4);       // Set AF4 for PB10 and PB11
    } 
    else if (regs == I2C_3) {
        RCC->APB1ENR |= RCC_APB1ENR_I2C3EN;
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
        // Configure PB8 (SCL) and PB9 (SDA) as AF4 for I2C3
        GPIO_B->MODER   &= ~(MODER_PIN8_MASK | MODER_PIN9_MASK);              // Clear mode bits
        GPIO_B->MODER   |= (MODER_PIN8_SET | MODER_PIN9_SET);                 // Alternate function
        GPIO_B->AFR[1]  &= ~(AFRH_PIN8_MASK | AFRH_PIN9_MASK);              // Clear AFRH for PB8 and PB9
        GPIO_B->AFR[1]  |= (AFRH_PIN8_SET_AF4 | AFRH_PIN9_SET_AF4);         // Set AF4 for PB8 and PB9
    }
    else if (regs == I2C_3) {
        RCC->APB1ENR |= RCC_APB1ENR_I2C3EN;
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
        // Configure PB8 (SCL) and PB9 (SDA) as AF4 for I2C3
        GPIO_B->MODER   &= ~(MODER_PIN8_MASK | MODER_PIN9_MASK);              // Clear mode bits
        GPIO_B->MODER   |= (MODER_PIN8_SET | MODER_PIN9_SET);                 // Alternate function
        GPIO_B->AFR[1]  &= ~(AFRH_PIN8_MASK | AFRH_PIN9_MASK);              // Clear AFRH for PB8 and PB9
        GPIO_B->AFR[1]  |= (AFRH_PIN8_SET_AF4 | AFRH_PIN9_SET_AF4);         // Set AF4 for PB8 and PB9
    }

    // Ensure peripheral disabled and perform a software reset (HAL-like)
    hi2c->regs->CR1 &= ~I2C_CR1_PE;
    hi2c->regs->CR1 |= I2C_CR1_SWRST;
    short_delay(1000);
    hi2c->regs->CR1 &= ~I2C_CR1_SWRST;
    
    // Timing configuration (assumes APB1 ~16 MHz unless system clock changed)
    regs->CR2 = I2C_CR2_FREQ;                                          // Peripheral clock frequency (MHz)
    if (speed == I2C_STANDARD_MODE) {
        hi2c->regs->CCR = 0x28;                                              // Standard mode, 100kHz
        hi2c->regs->TRISE = 0x09;                                            // Maximum rise time
    }
    else if (speed == I2C_FAST_MODE) {
        hi2c->regs->CCR = 0x0D;                                              // Fast mode, 400kHz
        hi2c->regs->TRISE = 0x03;                                            // Maximum rise time
    }

    // Own address register (bit 14 must be kept at 1 in 7-bit mode per RM)
    hi2c->regs->OAR1 = 0x4000;                                               // 7-bit, address 0
    // Enable peripheral
    hi2c->regs->CR1 |= I2C_CR1_PE;
}

int scan_i2c_bus(I2C_HandleType *hi2c)
{
    int acked_addresses = 0;
    for (uint8_t address = 1; address < 128; ++address) {
        I2C_Start(hi2c);
        int ack = I2C_SendAddress(hi2c, address, 0); // Write mode
        I2C_Stop(hi2c);
        if (ack) {
            acked_addresses++;
        }
    }
    return acked_addresses;
}

int I2C_Read(I2C_HandleType *hi2c)
{
    //1. wait until RXNE is set in the SR register
    while(!(hi2c->regs->SR1 & I2C_SR1_RXNE));

    //2. read data from DR register
    return (hi2c->regs->DR & 0xFF);
}

void I2C_Write(I2C_HandleType *hi2c, int data)
{
    //1. wait until TXE is set in the SR register
    while(!(hi2c->regs->SR1 & I2C_SR1_TXE));

    //2. write to DR register
    hi2c->regs->DR = (data & 0xFF);
}

void I2C_Stop(I2C_HandleType *hi2c)
{
    // Generate STOP condition and wait for bus to become free
    hi2c->regs->CR1 |= I2C_CR1_STOP;
    // Optionally poll BUSY flag clear (simple delay loop here)
    for(volatile int i=0; i<1000; ++i) { __asm__("nop"); }
}

void I2C_Start(I2C_HandleType *hi2c)
{
    // Wait until bus is free then generate START and wait for SB
    while (hi2c->regs->SR2 & I2C_SR2_BUSY);
    hi2c->regs->CR1 |= I2C_CR1_START;
    while (!(hi2c->regs->SR1 & I2C_SR1_SB));
}

void I2C_Restart(I2C_HandleType *hi2c)
{
    // Generate repeated START and wait for SB
    hi2c->regs->CR1 |= I2C_CR1_START;
    while (!(hi2c->regs->SR1 & I2C_SR1_SB));
}

int I2C_SendAddress(I2C_HandleType *hi2c, uint8_t address, int read)
{
    // Send 7-bit address and R/W bit
    hi2c->regs->DR = (uint32_t)((address << 1) | (read ? 1 : 0));
    // Wait for either ADDR (ACK) or AF (NACK)
    while (1) {
        uint32_t sr1 = hi2c->regs->SR1;
        if (sr1 & I2C_SR1_ADDR) {
            // Clear ADDR by reading SR1 then SR2
            volatile uint32_t tmp = hi2c->regs->SR1; (void)tmp;
            tmp = hi2c->regs->SR2; (void)tmp;
            return 1; // ACK
        }
        if (sr1 & I2C_SR1_AF) {
            // Clear AF and generate STOP to release bus
            hi2c->regs->SR1 &= ~I2C_SR1_AF;
            hi2c->regs->CR1 |= I2C_CR1_STOP;
            return 0; // NACK
        }
    }
}

void I2C_EnableAck(I2C_HandleType *hi2c)
{
    hi2c->regs->CR1 |= I2C_CR1_ACK;
}

void I2C_DisableAck(I2C_HandleType *hi2c)
{
    hi2c->regs->CR1 &= ~I2C_CR1_ACK;
}
