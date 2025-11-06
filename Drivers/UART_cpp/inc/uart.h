#ifndef __UART_H
#define __UART_H

#include <stdint.h>
#include <stdio.h>
#include "../../GPIO/inc/gpio.h"

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
#define RCC_AHB1ENR_GPIOAEN  ((uint32_t)0x00000001)         // Bit 0
#define RCC_AHB1ENR_GPIOBEN  ((uint32_t)0x00000002)         // Bit 1
#define RCC_AHB1ENR_GPIOCEN  ((uint32_t)0x00000004)         // Bit 2
#define RCC_AHB1ENR_GPIODEN  ((uint32_t)0x00000008)         // Bit 3
#define RCC_AHB1ENR_GPIOEEN  ((uint32_t)0x00000010)         // Bit 4

// Default peripheral clocks (Hz) used for baud calculation when system clock
// is not explicitly configured elsewhere. On STM32F4 Discovery, using HSI by
// default gives APB1=APB2=16 MHz.
#ifndef APB1_CLK_HZ
#define APB1_CLK_HZ 16000000U
#endif
#ifndef APB2_CLK_HZ
#define APB2_CLK_HZ 16000000U
#endif

// GPIO MODER register Mask bit definitions
#define MODER_PIN0_MASK          ((uint32_t)0x00000003)        // mask to clear MODER_PIN0 bits (bits 1:0)
#define MODER_PIN1_MASK          ((uint32_t)0x0000000C)        // mask to clear MODER_PIN1 bits (bits 3:2)
#define MODER_PIN2_MASK          ((uint32_t)0x00000030)        // mask to clear MODER_PIN2 bits (bits 5:4)
#define MODER_PIN3_MASK          ((uint32_t)0x000000C0)        // mask to clear MODER_PIN3 bits (bits 7:6)
#define MODER_PIN4_MASK          ((uint32_t)0x00000300)        // mask to clear MODER_PIN4 bits (bits 9:8)
#define MODER_PIN5_MASK          ((uint32_t)0x00000C00)        // mask to clear MODER_PIN5 bits (bits 11:10)
#define MODER_PIN6_MASK          ((uint32_t)0x00003000)        // mask to clear MODER_PIN6 bits (bits 13:12)
#define MODER_PIN7_MASK          ((uint32_t)0x0000C000)        // mask to clear MODER_PIN7 bits (bits 15:14)
#define MODER_PIN8_MASK          ((uint32_t)0x00030000)        // mask to clear MODER_PIN8 bits (bits 17:16)
#define MODER_PIN9_MASK          ((uint32_t)0x000C0000)        // mask to clear MODER_PIN9 bits (bits 19:18)
#define MODER_PIN10_MASK         ((uint32_t)0x00300000)        // mask to clear MODER_PIN10 bits (bits 21:20)
#define MODER_PIN11_MASK         ((uint32_t)0x00C00000)        // mask to clear MODER_PIN11 bits (bits 23:22)
#define MODER_PIN12_MASK         ((uint32_t)0x03000000)        // mask to clear MODER_PIN12 bits (bits 25:24)
#define MODER_PIN13_MASK         ((uint32_t)0x0C000000)        // mask to clear MODER_PIN13 bits (bits 27:26)
#define MODER_PIN14_MASK         ((uint32_t)0x30000000)        // mask to clear MODER_PIN14 bits (bits 29:28)
#define MODER_PIN15_MASK         ((uint32_t)0xC0000000)        // mask to clear MODER_PIN15 bits (bits 31:30)

// GPIO MODER register Set bit definitions for Alternate Function mode (10b)
#define MODER_PIN0_SET           ((uint32_t)0x00000002)        // set AF for PIN0 (bits 1:0)
#define MODER_PIN1_SET           ((uint32_t)0x00000008)        // set AF for PIN1 (bits 3:2)
#define MODER_PIN2_SET           ((uint32_t)0x00000020)        // set AF for PIN2 (bits 5:4)
#define MODER_PIN3_SET           ((uint32_t)0x00000080)        // set AF for PIN3 (bits 7:6)
#define MODER_PIN4_SET           ((uint32_t)0x00000200)        // set AF for PIN4 (bits 9:8)
#define MODER_PIN5_SET           ((uint32_t)0x00000800)        // set AF for PIN5 (bits 11:10)
#define MODER_PIN6_SET           ((uint32_t)0x00002000)        // set AF for PIN6 (bits 13:12)
#define MODER_PIN7_SET           ((uint32_t)0x00008000)        // set AF for PIN7 (bits 15:14)
#define MODER_PIN8_SET           ((uint32_t)0x00020000)        // set AF for PIN8 (bits 17:16)
#define MODER_PIN9_SET           ((uint32_t)0x00080000)        // set AF for PIN9 (bits 19:18)
#define MODER_PIN10_SET          ((uint32_t)0x00200000)        // set AF for PIN10 (bits 21:20)
#define MODER_PIN11_SET          ((uint32_t)0x00800000)        // set AF for PIN11 (bits 23:22)
#define MODER_PIN12_SET          ((uint32_t)0x02000000)        // set AF for PIN12 (bits 25:24)
#define MODER_PIN13_SET          ((uint32_t)0x08000000)        // set AF for PIN13 (bits 27:26)
#define MODER_PIN14_SET          ((uint32_t)0x20000000)        // set AF for PIN14 (bits 29:28)
#define MODER_PIN15_SET          ((uint32_t)0x80000000)        // set AF for PIN15 (bits 31:30)

// GPIO AFRL register Mask bit definitions
#define AFRL_PIN0_MASK           ((uint32_t)0x0000000F)        // mask to clear AFRL_PIN0 bits (bits 3:0)
#define AFRL_PIN1_MASK           ((uint32_t)0x000000F0)        // mask to clear AFRL_PIN1 bits (bits 7:4)
#define AFRL_PIN2_MASK           ((uint32_t)0x00000F00)        // mask to clear AFRL_PIN2 bits (bits 11:8)
#define AFRL_PIN3_MASK           ((uint32_t)0x0000F000)        // mask to clear AFRL_PIN3 bits (bits 15:12)
#define AFRL_PIN4_MASK           ((uint32_t)0x000F0000)        // mask to clear AFRL_PIN4 bits (bits 19:16)
#define AFRL_PIN5_MASK           ((uint32_t)0x00F00000)        // mask to clear AFRL_PIN5 bits (bits 23:20)
#define AFRL_PIN6_MASK           ((uint32_t)0x0F000000)        // mask to clear AFRL_PIN6 bits (bits 27:24)
#define AFRL_PIN7_MASK           ((uint32_t)0xF0000000)        // mask to clear AFRL_PIN7 bits (bits 31:28)
#define AFRH_PIN8_MASK           ((uint32_t)0x0000000F)        // mask to clear AFRH_PIN8 bits (bits 3:0)
#define AFRH_PIN9_MASK           ((uint32_t)0x000000F0)        // mask to clear AFRH_PIN9 bits (bits 7:4)
#define AFRH_PIN10_MASK          ((uint32_t)0x00000F00)        // mask to clear AFRH_PIN10 bits (bits 11:8)
#define AFRH_PIN11_MASK          ((uint32_t)0x0000F000)        // mask to clear AFRH_PIN11 bits (bits 15:12)
#define AFRH_PIN12_MASK          ((uint32_t)0x000F0000)        // mask to clear AFRH_PIN12 bits (bits 19:16)
#define AFRH_PIN13_MASK          ((uint32_t)0x00F00000)        // mask to clear AFRH_PIN13 bits (bits 23:20)
#define AFRH_PIN14_MASK          ((uint32_t)0x0F000000)        // mask to clear AFRH_PIN14 bits (bits 27:24)
#define AFRH_PIN15_MASK          ((uint32_t)0xF0000000)        // mask to clear AFRH_PIN15 bits (bits 31:28)

// GPIO AFR register Set bit definitions for AF7 (0111b) [USART1, USART2, USART3]
#define AFRL_PIN0_SET_AF7            ((uint32_t)0x00000007)        // set AF7 for PIN0 (bits 3:0)
#define AFRL_PIN1_SET_AF7            ((uint32_t)0x00000070)        // set AF7 for PIN1 (bits 7:4)
#define AFRL_PIN2_SET_AF7            ((uint32_t)0x00000700)        // set AF7 for PIN2 (bits 11:8)
#define AFRL_PIN3_SET_AF7            ((uint32_t)0x00007000)        // set AF7 for PIN3 (bits 15:12)
#define AFRL_PIN4_SET_AF7            ((uint32_t)0x00070000)        // set AF7 for PIN4 (bits 19:16)
#define AFRL_PIN5_SET_AF7            ((uint32_t)0x00700000)        // set AF7 for PIN5 (bits 23:20)
#define AFRL_PIN6_SET_AF7            ((uint32_t)0x07000000)        // set AF7 for PIN6 (bits 27:24)
#define AFRL_PIN7_SET_AF7            ((uint32_t)0x70000000)        // set AF7 for PIN7 (bits 31:28)
#define AFRH_PIN8_SET_AF7            ((uint32_t)0x00000007)        // set AF7 for PIN8 (bits 3:0)
#define AFRH_PIN9_SET_AF7            ((uint32_t)0x00000070)        // set AF7 for PIN9 (bits 7:4)
#define AFRH_PIN10_SET_AF7           ((uint32_t)0x00000700)        // set AF7 for PIN10 (bits 11:8)
#define AFRH_PIN11_SET_AF7           ((uint32_t)0x00007000)        // set AF7 for PIN11 (bits 15:12)
#define AFRH_PIN12_SET_AF7           ((uint32_t)0x00070000)        // set AF7 for PIN12 (bits 19:16)
#define AFRH_PIN13_SET_AF7           ((uint32_t)0x00700000)        // set AF7 for PIN13 (bits 23:20)
#define AFRH_PIN14_SET_AF7           ((uint32_t)0x07000000)        // set AF7 for PIN14 (bits 27:24)
#define AFRH_PIN15_SET_AF7           ((uint32_t)0x70000000)        // set AF7 for PIN15 (bits 31:28)

// GPIO AFR register Set bit definitions for AF8 (1000b) [UART4, UART5, USART6]
#define AFRL_PIN0_SET_AF8        ((uint32_t)0x00000008)        // set AF8 for PIN0 (bits 3:0)
#define AFRL_PIN1_SET_AF8        ((uint32_t)0x00000080)        // set AF8 for PIN1 (bits 7:4)
#define AFRL_PIN2_SET_AF8        ((uint32_t)0x00000800)        // set AF8 for PIN2 (bits 11:8)
#define AFRL_PIN3_SET_AF8        ((uint32_t)0x00008000)        // set AF8 for PIN3 (bits 15:12)
#define AFRL_PIN4_SET_AF8        ((uint32_t)0x00080000)        // set AF8 for PIN4 (bits 19:16)
#define AFRL_PIN5_SET_AF8        ((uint32_t)0x00800000)        // set AF8 for PIN5 (bits 23:20)
#define AFRL_PIN6_SET_AF8        ((uint32_t)0x08000000)        // set AF8 for PIN6 (bits 27:24)
#define AFRL_PIN7_SET_AF8        ((uint32_t)0x80000000)        // set AF8 for PIN7 (bits 31:28)
#define AFRH_PIN8_SET_AF8        ((uint32_t)0x00000008)        // set AF8 for PIN8 (bits 3:0)
#define AFRH_PIN9_SET_AF8        ((uint32_t)0x00000080)        // set AF8 for PIN9 (bits 7:4)
#define AFRH_PIN10_SET_AF8       ((uint32_t)0x00000800)        // set AF8 for PIN10 (bits 11:8)
#define AFRH_PIN11_SET_AF8       ((uint32_t)0x00008000)        // set AF8 for PIN11 (bits 15:12)
#define AFRH_PIN12_SET_AF8       ((uint32_t)0x00080000)        // set AF8 for PIN12 (bits 19:16)
#define AFRH_PIN13_SET_AF8       ((uint32_t)0x00800000)        // set AF8 for PIN13 (bits 23:20)
#define AFRH_PIN14_SET_AF8       ((uint32_t)0x08000000)        // set AF8 for PIN14 (bits 27:24)
#define AFRH_PIN15_SET_AF8       ((uint32_t)0x80000000)        // set AF8 for PIN15 (bits 31:28)

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
} USART_Manual_TypeDef;

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

// USART2 peripheral declaration
#define USART_1 ((USART_Manual_TypeDef *)USART_1_BASE)
#define USART_2 ((USART_Manual_TypeDef *)USART_2_BASE)
#define USART_3 ((USART_Manual_TypeDef *)USART_3_BASE)
#define UART_4  ((USART_Manual_TypeDef *)UART_4_BASE)
#define UART_5  ((USART_Manual_TypeDef *)UART_5_BASE)
#define USART_6 ((USART_Manual_TypeDef *)USART_6_BASE)

class USART
{
private:
    USART_Manual_TypeDef *USARTx;
    UART_COMType comType;
    UART_BaudRateType baudRate;
public:
    // Construct and initialize a USART instance
    USART(USART_Manual_TypeDef *USARTx, UART_COMType comtype, UART_BaudRateType baudrate);

    void USART_Init(USART_Manual_TypeDef *USARTx, UART_COMType comtype, UART_BaudRateType baudrate);
    void USART_x_Write(int ch);
    char USART_x_Read(void);
    uint16_t BRR_Oversample_by_16(uint32_t fck_hz, uint32_t baud);
    void writeString(const char *str);
    void readString(char *buffer, size_t maxLength);

    // Optional stdio-style helpers
    int get_char(FILE *f);
    int send_char(int c, FILE *f);

    // Accessors
    UART_COMType getComType() const;
    UART_BaudRateType getBaudRate() const;
};

#endif // __UART_H
