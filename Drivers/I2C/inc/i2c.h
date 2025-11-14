#ifndef __I2C_H
#define __I2C_H

#include <stdio.h>
#include <stdint.h>
#include "../../GPIO/inc/gpio.h"

// Base addresses
#define PERIPH_ADDR_BASE 0x40000000U
#define APB1PERIPH_ADDR_BASE    (PERIPH_ADDR_BASE + 0x00000000U)
#define AHB1PERIPH_ADDR_BASE    (PERIPH_ADDR_BASE + 0x00020000U)

// Base addresses of peripherals
#define I2C1_BASE_ADDR          (APB1PERIPH_ADDR_BASE + 0x5400U)
#define I2C2_BASE_ADDR          (APB1PERIPH_ADDR_BASE + 0x5800U)
#define I2C3_BASE_ADDR          (APB1PERIPH_ADDR_BASE + 0x5C00U)
#define GPIOB_BASE_ADDR         (AHB1PERIPH_ADDR_BASE + 0x0400U)

// I2C peripheral declarations
#define I2C_1                   ((I2C_TypeDef *)I2C1_BASE_ADDR)
#define I2C_2                   ((I2C_TypeDef *)I2C2_BASE_ADDR)
#define I2C_3                   ((I2C_TypeDef *)I2C3_BASE_ADDR)

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
#define RCC_AHB1ENR_GPIOBEN     ((uint32_t)(1 << 1))          // Bit 1

// MODE register bit definitions
#define MODER_PIN0_MASK         ((uint32_t)0x00000003)        // mask for PD0 mode bits (bits 1:0)
#define MODER_PIN6_MASK         ((uint32_t)0x00003000)        // mask for PB6 mode bits (bits 13:12)
#define MODER_PIN7_MASK         ((uint32_t)0x0000C000)        // mask for PB7 mode bits (bits 15:14)
#define MODER_PIN9_MASK         ((uint32_t)0x000C0000)        // mask for PB9 mode bits (bits 19:18)
#define MODER_PIN0_SET          ((uint32_t)0x00000002)        // set AF (10) for PD0
#define MODER_PIN6_SET          ((uint32_t)0x00002000)        // set AF (10) for PB6
#define MODER_PIN7_SET          ((uint32_t)0x00008000)        // set AF (10) for PB7
#define MODER_PIN9_SET          ((uint32_t)0x00080000)        // set AF (10) for PB9

// Alternate Function register bit definitions
#define AFRL_PIN0_MASK           ((uint32_t)0x0000000F)       // mask to clear AFRL_PIN0 bits (bits 3:0)
#define AFRL_PIN6_MASK           ((uint32_t)0x0F000000)       // mask to clear AFRL_PIN6 bits (bits 27:24)
#define AFRL_PIN7_MASK           ((uint32_t)0xF0000000)       // mask to clear AFRL_PIN7 bits (bits 31:28)
#define AFRH_PIN9_MASK           ((uint32_t)0x000000F0)       // mask to clear AFRH_PIN9 bits (bits 7:4)
#define AFRH_PIN9_MASK           ((uint32_t)0x000000F0)       // mask to clear AFRH_PIN9 bits (bits 7:4)
// I2C on STM32F4 uses AF4 on PB6/PB7/PB9
#define AFRL_PIN6_SET_AF4        ((uint32_t)0x04000000)       // set AF4 for PB6 (I2C1 SCL)
#define AFRL_PIN7_SET_AF4        ((uint32_t)0x40000000)       // set AF4 for PB7 (I2C1 SDA option)
#define AFRH_PIN9_SET_AF4        ((uint32_t)0x00000040)       // set AF4 for PB9 (I2C1 SDA preferred)
#define AFRH_PIN9_SET_AF4        ((uint32_t)0x00000040)       // set AF4 for PB9 (I2C1 SDA alt)

// I2C register bit definitions
#define I2C_CR1_PE              ((uint32_t)0x0001)            // Peripheral Enable
#define I2C_CR1_START           ((uint32_t)0x0100)            // Start Generation
#define I2C_CR2_FREQ            ((uint32_t)0x0010)            // Peripheral Clock Frequency (16 MHz)
#define I2C_CR1_STOP            ((uint32_t)0x0200)            // Stop generation
#define I2C_CR1_ACK             ((uint32_t)0x0400)            // Acknowledge enable
#define I2C_SR1_RXNE            ((uint32_t)0x0040)            // Data Register not Empty
#define I2C_SR1_ADDR            ((uint32_t)0x0002)            // Address sent (master mode)/matched (slave mode)
#define I2C_SR1_BTF             ((uint32_t)0x0004)            // Byte Transfer Finished
#define I2C_SR1_TXE             ((uint32_t)0x0080)            // Data Register Empty
#define I2C_SR1_SB              ((uint32_t)0x0001)            // Start Bit (Master mode)
#define I2C_SR1_AF              ((uint32_t)0x0400)            // Acknowledge Failure
#define I2C_CR1_SWRST           ((uint32_t)0x8000)            // Software reset
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
} I2C_TypeDef;

// I2C configuration enums
typedef enum {
    I2C_STANDARD_MODE = 0,
    I2C_FAST_MODE
} I2C_SpeedType;

typedef enum {
    I2C_ACK_ENABLE = 0,
    I2C_ACK_DISABLE
} I2C_AckType;

typedef enum {
    I2C_7BIT_ADDRESS = 0,
    I2C_10BIT_ADDRESS
} I2C_AddressType;

// I2C function prototypes
void I2C_Init(I2C_SpeedType speed);
int I2C_Read(void);
void I2C_Write(int data);
int read_I2C_address(int address);
void I2C_Stop(void);

// Minimal helper APIs for master transactions
void I2C_Start(void);
void I2C_Restart(void);
int  I2C_SendAddress(uint8_t address, int read); // returns 1 if ACK (ADDR set), 0 if NACK (AF)
void I2C_EnableAck(void);
void I2C_DisableAck(void);
// Bus recovery helper (HAL-like): release stuck SCL/SDA by toggling SCL
void I2C_BusRecover(void);

#endif // __I2C_H
