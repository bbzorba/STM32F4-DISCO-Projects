#ifndef __SPI_H
#define __SPI_H

#include <stdio.h>
#include <stdint.h>
#include "../../GPIO/inc/gpio.h"

// IO definitions
#define __IO volatile

// Base addresses
#define APB1PERIPH_BASE    (PERIPH_BASE + 0x00000000U)
#define APB2PERIPH_BASE    (PERIPH_BASE + 0x00010000U)
#define AHB1PERIPH_BASE    (PERIPH_BASE + 0x00020000U)

// Base addresses of peripherals
#define SPI_1_BASE     (APB2PERIPH_BASE + 0x00003000U)
#define SPI_2_BASE     (APB1PERIPH_BASE + 0x00003800U)
#define SPI_3_BASE     (APB1PERIPH_BASE + 0x00003C00U)

// RCC register bit definitions
#define RCC_APB2ENR_SPI_1EN    ((uint32_t)0x00001000)        // Bit 12
#define RCC_APB1ENR_SPI_2EN    ((uint32_t)0x00004000)        // Bit 14
#define RCC_APB1ENR_SPI_3EN    ((uint32_t)0x00008000)        // Bit 15

// SPI CR1 register bit definitions
#define SPI_CR1_CPHA            ((uint32_t)0x0001)            // Clock Phase
#define SPI_CR1_CPOL            ((uint32_t)0x0002)            // Clock Polarity
#define SPI_CR1_MSTR            ((uint32_t)0x0004)            // Master Selection
#define SPI_CR1_BR              ((uint32_t)0x0038)            // Baud Rate Control
#define SPI_CR1_SPE             ((uint32_t)0x0040)            // SPI Enable
#define SPI_CR1_LSBFIRST        ((uint32_t)0x0080)            // Frame Format
#define SPI_CR1_SSI             ((uint32_t)0x0200)            // Internal Slave Select
#define SPI_CR1_SSM             ((uint32_t)0x0100)            // Software Slave Management
#define SPI_CR1_RXONLY          ((uint32_t)0x0400)            // Receive Only
#define SPI_CR1_DFF             ((uint32_t)0x0800)            // Data Frame Format
#define SPI_CR1_CRCNEXT         ((uint32_t)0x1000)            // CRC Transfer Next
#define SPI_CR1_CRCEN           ((uint32_t)0x2000)            // Hardware CRC Calculation Enable
#define SPI_CR1_BIDIOE          ((uint32_t)0x4000)            // Output Enable in Bidirectional Mode
#define SPI_CR1_BIDIMODE        ((uint32_t)0x8000)            // Bidirectional Data Mode Enable
#define SPI_CR1_BR_0           ((uint32_t)0x0008)             // Baud Rate bit 0
#define SPI_CR1_BR_1           ((uint32_t)0x0010)             // Baud Rate bit 1
#define SPI_CR1_BR_2           ((uint32_t)0x0020)             // Baud Rate bit 2

// SPI SR register bit definitions
#define SPI_SR_RXNE             ((uint32_t)0x0001)            // Receive buffer Not Empty
#define SPI_SR_TXE              ((uint32_t)0x0002)            // Transmit buffer Empty
#define SPI_SR_CHSIDE           ((uint32_t)0x0004)            // Channel Side
#define SPI_SR_UDR              ((uint32_t)0x0008)            // Underrun flag
#define SPI_SR_CRCERR           ((uint32_t)0x0010)            // CRC Error flag
#define SPI_SR_MODF             ((uint32_t)0x0020)            // Mode fault
#define SPI_SR_OVR              ((uint32_t)0x0040)            // Overrun flag
#define SPI_SR_BSY              ((uint32_t)0x0080)            // Busy flag
#define SPI_SR_FRE              ((uint32_t)0x0100)            // TI frame format error flag

// SPI configuration enums
typedef enum {
    SPI_BAUDRATE_DIV2 = 0,
    SPI_BAUDRATE_DIV4,
    SPI_BAUDRATE_DIV8,
    SPI_BAUDRATE_DIV16,
    SPI_BAUDRATE_DIV32,
    SPI_BAUDRATE_DIV64,
    SPI_BAUDRATE_DIV128,
    SPI_BAUDRATE_DIV256
} SPI_BaudRateType;

// SPI mode enums
typedef enum {
    SPI_MODE_SLAVE = 0,
    SPI_MODE_MASTER = 1,
} SPI_ModeType;

// SPI direction enums
typedef enum {
    SPI_DIRECTION_2LINES = 0,
    SPI_DIRECTION_2LINES_RXONLY,
    SPI_DIRECTION_1LINE
} SPI_DirectionType;

// SPI data size enums
typedef enum {
    SPI_DATASIZE_8BIT = 0,
    SPI_DATASIZE_16BIT
} SPI_DataSizeType;

typedef enum {
    SPI_CLOCK_POLARITY_LOW = 0,
    SPI_CLOCK_POLARITY_HIGH
}SPI_ClockPolarityType;

typedef enum {
    SPI_CLOCK_PHASE_1EDGE = 0,
    SPI_CLOCK_PHASE_2EDGE
}SPI_ClockPhaseType;

typedef enum {
    SPI1_PORTA = 0,
    SPI1_PORTB,
    SPI2_PORTB,
    SPI2_PORTC,
    SPI3_PORTB,
    SPI3_PORTC
} SPI_PinConfigType;

/* SPI registers structure */
typedef struct
{
    __IO uint32_t CR1;    // Control Register 1
    __IO uint32_t CR2;    // Control Register 2
    __IO uint32_t SR;     // Status Register
    __IO uint32_t DR;     // Data Register
    __IO uint32_t CRCPR;  // CRC Polynomial Register
    __IO uint32_t RXCRCR; // RX CRC Register
    __IO uint32_t TXCRCR; // TX CRC Register
    __IO uint32_t I2SCFGR;// I2S Configuration Register
    __IO uint32_t I2SPR;  // I2S Prescaler Register
} SPI_ManualType;

typedef struct
{
    SPI_ManualType *regs;           // pointer to hardware register block
    SPI_PinConfigType pinConfig;        // selected pin configuration
    SPI_BaudRateType baudrate;          // selected baud rate enum
    SPI_ModeType mode;                  // SPI mode (master/slave)
    SPI_DirectionType direction;        // SPI direction
    SPI_DataSizeType datasize;          // SPI data size
}SPI_HandleType; // High-level handle ("object")

// SPI peripheral declarations
#define SPI_1 ((SPI_ManualType *)SPI_1_BASE)
#define SPI_2 ((SPI_ManualType *)SPI_2_BASE)
#define SPI_3 ((SPI_ManualType *)SPI_3_BASE)

// SPI function prototypes (high-level handle based)
void SPI_Init(SPI_HandleType *spi, SPI_ManualType *regs, SPI_PinConfigType pinConfig, SPI_ModeType mode, SPI_BaudRateType baudrate, SPI_DirectionType direction);
int  SPI_WriteRead(SPI_HandleType *spi, const uint8_t *txData, uint8_t *rxData, size_t length);
void SPI_DeInit(SPI_HandleType *spi);
void SPI_SetMode(SPI_HandleType *spi, uint8_t mode);
void SPI_CS_Init(GPIO_HandleTypeDef *GPIOx, GPIO_InitTypeDef *GPIO_InitStruct, GPIO_ManualTypeDef *GPIO_ManualStruct, uint16_t pin);
void SPI_CS_Low(GPIO_HandleTypeDef *GPIOx, uint16_t CS_pin);
void SPI_CS_High(GPIO_HandleTypeDef *GPIOx, uint16_t CS_pin);

#endif // __SPI_H