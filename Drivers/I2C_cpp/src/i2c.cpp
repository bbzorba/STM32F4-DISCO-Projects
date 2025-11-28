#include "../inc/i2c.h"

static void short_delay(volatile int n) { while (n--) { __asm__("nop"); } }

I2C::I2C(I2C_ManualTypeDef *regs, I2C_SpeedType speed)
{
    this->regs = regs;
    this->speed = speed;
    
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
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
        // Configure PA8 (SCL) and PC9 (SDA) as AF4 for I2C3
        GPIO_A->MODER   &= ~(MODER_PIN8_MASK);              // Clear mode bits of PA8
        GPIO_C->MODER   &= ~(MODER_PIN9_MASK);              // Clear mode bits of PC9
        GPIO_A->MODER   |= (MODER_PIN8_SET);                // Alternate function for PA8
        GPIO_C->MODER   |= (MODER_PIN9_SET);                // Alternate function for PC9
        GPIO_A->AFR[1]  &= ~(AFRH_PIN8_MASK);              // Clear AFRH for PA8
        GPIO_C->AFR[1]  &= ~(AFRH_PIN9_MASK);              // Clear AFRH for PC9
        GPIO_A->AFR[1]  |= (AFRH_PIN8_SET_AF4);             // Set AF4 for PA8
        GPIO_C->AFR[1]  |= (AFRH_PIN9_SET_AF4);             // Set AF4 for PC9
    }
    else if (!regs) {
        return; // Invalid I2C peripheral
    }

    // Ensure peripheral disabled and perform a software reset (HAL-like)
    this->regs->CR1 &= ~I2C_CR1_PE;
    this->regs->CR1 |= I2C_CR1_SWRST;
    short_delay(1000);
    this->regs->CR1 &= ~I2C_CR1_SWRST;
    
    // Timing configuration (assumes APB1 ~16 MHz unless system clock changed)
    this->regs->CR2 = I2C_CR2_FREQ;                                          // Peripheral clock frequency (MHz)
    if (speed == I2C_STANDARD_MODE) {
        this->regs->CCR = 0x28;                                              // Standard mode, 100kHz
        this->regs->TRISE = 0x09;                                            // Maximum rise time
    }
    else if (speed == I2C_FAST_MODE) {
        this->regs->CCR = 0x0D;                                              // Fast mode, 400kHz
        this->regs->TRISE = 0x03;                                            // Maximum rise time
    }

    // Own address register (bit 14 must be kept at 1 in 7-bit mode per RM)
    this->regs->OAR1 = 0x4000;                                               // 7-bit, address 0
    // Enable peripheral
    this->regs->CR1 |= I2C_CR1_PE;
}

int I2C::I2C_ScanAddresses()
{
    int acked_addresses = 0;
    for (uint8_t address = 1; address < 128; ++address) {
        I2C_Start();
        int ack = I2C_SendAddress(address, 0); // Write mode
        I2C_Stop();
        if (ack) {
            acked_addresses++;
        }
    }
    return acked_addresses;
}

int I2C::I2C_Read()
{
    //1. wait until RXNE is set in the SR register
    while(!(this->regs->SR1 & I2C_SR1_RXNE));

    //2. read data from DR register
    return (this->regs->DR & 0xFF);
}

void I2C::I2C_Write(int data)
{
    //1. wait until TXE is set in the SR register
    while(!(this->regs->SR1 & I2C_SR1_TXE));

    //2. write to DR register
    this->regs->DR = (data & 0xFF);
}

void I2C::I2C_Stop()
{
    // Generate STOP condition and wait for bus to become free
    this->regs->CR1 |= I2C_CR1_STOP;
    // Optionally poll BUSY flag clear (simple delay loop here)
    for(volatile int i=0; i<1000; ++i) { __asm__("nop"); }
}

void I2C::I2C_Start()
{
    // Wait until bus is free then generate START and wait for SB
    while (this->regs->SR2 & I2C_SR2_BUSY);
    this->regs->CR1 |= I2C_CR1_START;
    while (!(this->regs->SR1 & I2C_SR1_SB));
}

void I2C::I2C_Restart()
{
    // Generate repeated START and wait for SB
    this->regs->CR1 |= I2C_CR1_START;
    while (!(this->regs->SR1 & I2C_SR1_SB));
}

int I2C::I2C_SendAddress(uint8_t address, int read)
{
    // Send 7-bit address and R/W bit
    this->regs->DR = (uint32_t)((address << 1) | (read ? 1 : 0));
    // Wait for either ADDR (ACK) or AF (NACK)
    while (1) {
        uint32_t sr1 = this->regs->SR1;
        if (sr1 & I2C_SR1_ADDR) {
            // Clear ADDR by reading SR1 then SR2
            volatile uint32_t tmp = this->regs->SR1; (void)tmp;
            tmp = this->regs->SR2; (void)tmp;
            return 1; // ACK
        }
        if (sr1 & I2C_SR1_AF) {
            // Clear AF and generate STOP to release bus
            this->regs->SR1 &= ~I2C_SR1_AF;
            this->regs->CR1 |= I2C_CR1_STOP;
            return 0; // NACK
        }
    }
}

void I2C::I2C_EnableAck()
{
    this->regs->CR1 |= I2C_CR1_ACK;
}

void I2C::I2C_DisableAck()
{
    this->regs->CR1 &= ~I2C_CR1_ACK;
}
