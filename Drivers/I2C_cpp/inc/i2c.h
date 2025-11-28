#ifndef __I2C_H
#define __I2C_H

#include <stdio.h>
#include <stdint.h>
#include "../../GPIO_cpp/inc/gpio.h"

// Base addresses
#define APB1PERIPH_BASE    (PERIPH_BASE + 0x00000000U)

// Base addresses of peripherals
#define I2C1_BASE_ADDR          (APB1PERIPH_BASE + 0x5400U)
#define I2C2_BASE_ADDR          (APB1PERIPH_BASE + 0x5800U)
#define I2C3_BASE_ADDR          (APB1PERIPH_BASE + 0x5C00U)

// IO definitions
#define __IO volatile

// RCC register bit definitions
#define RCC_APB1RSTR_I2C1RST    ((uint32_t)(1 << 21))         // Bit 21
#define RCC_APB1RSTR_I2C2RST    ((uint32_t)(1 << 22))         // Bit 22
#define RCC_APB1RSTR_I2C3RST    ((uint32_t)(1 << 23))         // Bit 23
#define RCC_APB1ENR_I2C1EN      ((uint32_t)(1 << 21))         // Bit 21
#define RCC_APB1ENR_I2C2EN      ((uint32_t)(1 << 22))         // Bit 22
#define RCC_APB1ENR_I2C3EN      ((uint32_t)(1 << 23))         // Bit 23
#define RCC_APB1LPENR_I2C1LPEN  ((uint32_t)(1 << 21))         // Bit 21
#define RCC_APB1LPENR_I2C2LPEN  ((uint32_t)(1 << 22))         // Bit 22
#define RCC_APB1LPENR_I2C3LPEN  ((uint32_t)(1 << 23))         // Bit 23

// I2C register bit definitions
#define I2C_CR1_PE              ((uint32_t)0x0001)            // Peripheral Enable
#define I2C_CR1_SMBUS           ((uint32_t)0x0002)            // SMBus Mode
#define I2C_CR1_START           ((uint32_t)0x0100)            // Start Generation
#define I2C_CR1_STOP            ((uint32_t)0x0200)            // Stop generation
#define I2C_CR1_ACK             ((uint32_t)0x0400)            // Acknowledge enable
#define I2C_CR1_SWRST           ((uint32_t)0x8000)            // Software reset
#define I2C_CR2_FREQ            ((uint32_t)0x0010)            // Peripheral Clock Frequency (16 MHz)
#define I2C_SR1_SB              ((uint32_t)0x0001)            // Start Bit (Master mode)
#define I2C_SR1_ADDR            ((uint32_t)0x0002)            // Address sent (master mode)/matched (slave mode)
#define I2C_SR1_BTF             ((uint32_t)0x0004)            // Byte Transfer Finished
#define I2C_SR1_RXNE            ((uint32_t)0x0040)            // Data Register not Empty
#define I2C_SR1_TXE             ((uint32_t)0x0080)            // Data Register Empty
#define I2C_SR1_AF              ((uint32_t)0x0400)            // Acknowledge Failure
#define I2C_SR2_MSL             ((uint32_t)0x0001)            // Master/Slave
#define I2C_SR2_BUSY            ((uint32_t)0x0002)            // Bus Busy
#define I2C_SR2_TRA             ((uint32_t)0x0004)            // Transmitter/Receiver
#define I2C_SR2_GENCALL         ((uint32_t)0x0010)            // General Call Address (Slave mode)
#define I2C_SR2_DUALF           ((uint32_t)0x0080)            // Dual Flag (Slave mode)
#define I2C_SR2_PECERR          ((uint32_t)0x0100)            // PEC Error in reception
#define I2C_SR2_OVR             ((uint32_t)0x0200)            // Overrun/Underrun
#define I2C_SR2_AF              ((uint32_t)0x0400)            // Acknowledge Failure
#define I2C_SR2_ARLO            ((uint32_t)0x0800)            // Arbitration Lost (master mode)
#define I2C_SR2_BERR            ((uint32_t)0x1000)            // Bus Error

/* I2C registers structure */
typedef struct
{
    __IO uint32_t CR1;                                        // Control register 1
    __IO uint32_t CR2;                                        // Control register 2
    __IO uint32_t OAR1;                                       // Own address register 1
    __IO uint32_t OAR2;                                       // Own address register 2
    __IO uint32_t DR;                                         // Data register
    __IO uint32_t SR1;                                        // Status register 1
    __IO uint32_t SR2;                                        // Status register 2
    __IO uint32_t CCR;                                        // Clock control register
    __IO uint32_t TRISE;                                      // TRISE register
    __IO uint32_t FLTR;                                       // Filter register
} I2C_ManualTypeDef;

// I2C configuration enums
typedef enum {
    I2C_STANDARD_MODE = 0,
    I2C_FAST_MODE
} I2C_SpeedType;

class I2C
{
private:
    I2C_ManualTypeDef *regs;
    I2C_SpeedType speed;
public:
    I2C(I2C_ManualTypeDef *regs, I2C_SpeedType speed);
    void I2C_Start();
    void I2C_Restart();
    int I2C_SendAddress(uint8_t address, int read);
    void I2C_Stop();
    void I2C_Write(int data);
    int I2C_Read();
    int I2C_ScanAddresses();
    void I2C_EnableAck();
    void I2C_DisableAck();
};

// I2C peripheral declarations
#define I2C_1                   ((I2C_ManualTypeDef *)I2C1_BASE_ADDR)
#define I2C_2                   ((I2C_ManualTypeDef *)I2C2_BASE_ADDR)
#define I2C_3                   ((I2C_ManualTypeDef *)I2C3_BASE_ADDR)

#endif // __I2C_H