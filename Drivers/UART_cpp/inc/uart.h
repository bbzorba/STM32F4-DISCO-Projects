#ifndef __UART_H
#define __UART_H

#include <stdint.h>
#include <stdio.h>
#include "../../GPIO_cpp/inc/gpio.h"

#define __IO volatile

/* USART pins for STM32F4xx series:
 USART1 -> PB6 (TX), PB7 (RX)
 USART2 -> PA2 (TX), PA3 (RX) or PD5 (TX), PD6 (RX)
 USART3 -> PB10 (TX), PB11 (RX) or PD8 (TX), PD9 (RX)
 UART4 -> PA0 (TX), PA1 (RX) or PC10 (TX), PC11 (RX)
 UART5 -> PC12 (TX), PD2 (RX)
 USART6 -> PC6 (TX), PC7 (RX) */

// Base addresses
#define PERIPH_ADDR_BASE 0x40000000U
#define APB1PERIPH_ADDR_BASE (PERIPH_ADDR_BASE + 0x00000000U)
#define APB2PERIPH_ADDR_BASE (PERIPH_ADDR_BASE + 0x00010000U)

// APB1-related USART peripherals
#define USART_2_BASE (APB1PERIPH_ADDR_BASE + 0x00004400U)
#define USART_3_BASE (APB1PERIPH_ADDR_BASE + 0x00004800U)
#define UART_4_BASE (APB1PERIPH_ADDR_BASE + 0x00004C00U)
#define UART_5_BASE (APB1PERIPH_ADDR_BASE + 0x00005000U)

// APB2-related USART peripherals
#define USART_1_BASE (APB2PERIPH_ADDR_BASE + 0x00001000U)
#define USART_6_BASE (APB2PERIPH_ADDR_BASE + 0x00001400U)

// RCC register bit definitions
// APB2 enables: USART1 bit 4, USART6 bit 5 (per RM0090)
#define RCC_APB2ENR_USART_1EN ((uint32_t)0x00000010)        // Bit 4
#define RCC_APB1ENR_USART_2EN ((uint32_t)0x00020000)        // Bit 17
#define RCC_APB1ENR_USART_3EN ((uint32_t)0x00040000)        // Bit 18
#define RCC_APB1ENR_UART_4EN  ((uint32_t)0x00080000)        // Bit 19
#define RCC_APB1ENR_UART_5EN  ((uint32_t)0x00100000)        // Bit 20
#define RCC_APB2ENR_USART_6EN ((uint32_t)0x00000020)        // Bit 5

// Default peripheral clocks (Hz) used for baud calculation when system clock
// is not explicitly configured elsewhere. On STM32F4 Discovery, using HSI by
// default gives APB1=APB2=16 MHz.
#ifndef APB1_CLK_HZ
#define APB1_CLK_HZ 16000000U
#endif
#ifndef APB2_CLK_HZ
#define APB2_CLK_HZ 16000000U
#endif

// USART BRR register bit definitions
#define BRR_CNF1_115200 0x08B                               // 115200 @ 16 MHz, OVER8=0
#define BRR_CNF2_9600  0x683                                // 9600 @ 16 MHz, OVER8=0

//USART CR1, CR2 & CR3 register bit definitions
#define CR2_CNF1 0x0000                                     // 1 stop bit, no-op
#define CR3_CNF1 0x0000                                     // No flow control, no-op
#define USART_2_CR1_DIS 0x0000                              // Disable USART_2
#define USART_CR1_TX_EN 0x0008                              // Enable Transmitter
#define USART_CR1_RX_EN 0x0004                              // Enable Receiver
#define USART_CR1_EN 0x2000                                 // Enable USART

//USART SR & DR register bit definitions
#define USART_SR_RX_NOT_EMP 0x0020                          // USART Status Register - Receiver not Empty
#define USART_SR_TX_EMP 0x0080                              // USART Status Register - Transmitter not Empty

/* USART registers structure */
typedef struct
{
    __IO uint32_t SR;    // Status Register
    __IO uint32_t DR;    // Data Register
    __IO uint32_t BRR;   // Baud Rate Register
    __IO uint32_t CR1;   // Control Register 1
    __IO uint32_t CR2;   // Control Register 2
    __IO uint32_t CR3;   // Control Register 3
    __IO uint32_t GTPR;  // Guard time and prescaler register
} USART_ManualType;

// UART configuration enums
typedef enum {
    RX_ONLY = 0,
    TX_ONLY,
    RX_AND_TX
} UART_COMType;

typedef enum {
    __115200 = 0,
    __9600,
} UART_BaudRateType;

class USART {
private:
    UART_COMType comType;                 // configuration (RX/TX)
    UART_BaudRateType baudRate;           // selected baud enum
    USART_ManualType *regs;               // pointer to hardware register block
public:
    USART(USART_ManualType *regs, UART_COMType _comtype, UART_BaudRateType _baudrate);
    void USART_x_Write(int ch);
    char USART_x_Read();
    void USART_WriteChar(int ch);
    char USART_ReadChar();
    void USART_WriteString(const char *str);
    void USART_ReadString(char *buffer, size_t maxLength);
    const char* GetPortName();
};

// USART peripheral declarations
#define USART_1 ((USART_ManualType *)USART_1_BASE)
#define USART_2 ((USART_ManualType *)USART_2_BASE)
#define USART_3 ((USART_ManualType *)USART_3_BASE)
#define UART_4  ((USART_ManualType *)UART_4_BASE)
#define UART_5  ((USART_ManualType *)UART_5_BASE)
#define USART_6 ((USART_ManualType *)USART_6_BASE)

uint16_t BRR_Oversample_by_16(uint32_t fck_hz, uint32_t baud);

#endif // __UART_H
