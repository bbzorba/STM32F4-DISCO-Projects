#ifndef __SPI_H
#define __SPI_H

#include <stdio.h>
#include <stdint.h>
#include "../../GPIO/inc/gpio.h"

// IO definitions
#define __IO volatile

// Base addresses
#define PERIPH_ADDR_BASE 0x40000000U
#define APB1PERIPH_ADDR_BASE    (PERIPH_ADDR_BASE + 0x00000000U)
#define APB2PERIPH_ADDR_BASE    (PERIPH_ADDR_BASE + 0x00010000U)
#define AHB1PERIPH_ADDR_BASE    (PERIPH_ADDR_BASE + 0x00020000U)

// Base addresses of peripherals
#define SPI_1_BASE     (APB2PERIPH_ADDR_BASE + 0x00003000U)
#define SPI_2_BASE     (APB1PERIPH_ADDR_BASE + 0x00003800U)
#define SPI_3_BASE     (APB1PERIPH_ADDR_BASE + 0x00003C00U)

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

// SPI peripheral declarations
#define SPI_1 ((SPI_Manual_TypeDef *)SPI_1_BASE)
#define SPI_2 ((SPI_Manual_TypeDef *)SPI_2_BASE)
#define SPI_3 ((SPI_Manual_TypeDef *)SPI_3_BASE)

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
} SPI_Manual_TypeDef;

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
    SPI_MODE_0 = 0, // CPOL=0, CPHA=0
    SPI_MODE_1,     // CPOL=0, CPHA=1
    SPI_MODE_2,     // CPOL=1, CPHA=0
    SPI_MODE_3      // CPOL=1, CPHA=1
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

// SPI clock polarity enums
typedef enum {
    SPI_CPOL_LOW = 0,
    SPI_CPOL_HIGH
} SPI_ClockPolarityType;

// SPI clock phase enums
typedef enum {
    SPI_CPHA_1EDGE = 0,
    SPI_CPHA_2EDGE
} SPI_ClockPhaseType;

typedef struct
{
    SPI_Manual_TypeDef *regs;           // pointer to hardware register block
    SPI_BaudRateType baudrate;          // selected baud rate enum
    SPI_ModeType mode;                  // SPI mode (clock polarity and phase)
    SPI_DirectionType direction;        // SPI direction
    SPI_DataSizeType datasize;          // SPI data size
    SPI_ClockPolarityType cpol;         // Clock polarity
    SPI_ClockPhaseType cpha;             // Clock phase
}SPI_TypeDef; // High-level handle ("object")

// SPI function prototypes
void SPI_Init(SPI_Manual_TypeDef *SPIx, SPI_BaudRateType baudrate);
void SPI_x_Write(SPI_Manual_TypeDef *SPIx, uint8_t data);
uint8_t SPI_x_Read(SPI_Manual_TypeDef *SPIx);


#endif // __SPI_H