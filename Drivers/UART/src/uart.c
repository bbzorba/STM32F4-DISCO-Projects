/*
    Author: Baris Berk Zorba
    Date: June 2024
    UART driver for STM32F4xx series
    This driver provides an interface for initializing and using the UART peripherals
    on the STM32F4xx microcontrollers.
    Verified with HC06 Bluetooth module and STM32F4 Discovery board.
*/

#include "uart.h"

/* 
 USART pins for STM32F4xx series:
 USART1 -> PB6 (TX), PB7 (RX) or PA9 (TX), PA10 (RX)
 USART2 -> PA2 (TX), PA3 (RX) or PD5 (TX), PD6 (RX)
 USART3 -> PB10 (TX), PB11 (RX) or PD8 (TX), PD9 (RX)
 UART4 -> PA0 (TX), PA1 (RX) or PC10 (TX), PC11 (RX)
 UART5 -> PC12 (TX), PD2 (RX)
 USART6 -> PC6 (TX), PC7 (RX)*/

void USART_x_Init(USART_Manual_TypeDef *USARTx, UART_COMType comtype, UART_BaudRateType baudrate)
{
    // USART TX pin configuration
    if (comtype == TX_ONLY || comtype == RX_AND_TX) 
    {
        if (USARTx == USART_1) {
            RCC->RCC_APB2ENR |= RCC_APB2ENR_USART_1EN;                      // USART1 clock (APB2)

            // configuration for USART1 TX on PB6
            RCC->RCC_AHB1ENR |= RCC_AHB1ENR_GPIOBEN;                        // GPIOB clock
            USARTx->CR1 = 0x0000;                                           // Disable USART before configuration
            GPIO_B->MODER &= ~(0x3U << (6U * 2U));                          // clear PB6
            GPIO_B->MODER |=  (0x2U << (6U * 2U));                          // Set AF for PB6 (bits 13:12)
            GPIO_B->AFR[0] &= ~(0xFU << (6U * 4U));                         // clear AFRL[27:24]
            GPIO_B->AFR[0] |=  (0x7U << (6U * 4U));                         // AF7 for PB6

            // configuration for USART1 TX on PA9
            RCC->RCC_AHB1ENR |= RCC_AHB1ENR_GPIOAEN;                        // GPIOA clock
            GPIO_A->MODER &= ~(0x3U << (9U * 2U));                          // clear PA9
            GPIO_A->MODER |=  (0x2U << (9U * 2U));                          // AF for PA9
            GPIO_A->AFR[1] &= ~(0xFU << (1U * 4U));                         // clear AFRH[11:8]
            GPIO_A->AFR[1] |=  (0x7U << (1U * 4U));                         // AF7 for PA9
        }
        else if (USARTx == USART_2) {
            RCC->RCC_APB1ENR |= RCC_APB1ENR_USART_2EN;                      // USART2 clock (APB1)
            
            // configuration for USART2 TX on PA2
            RCC->RCC_AHB1ENR |= RCC_AHB1ENR_GPIOAEN;                        // GPIOA clock
            USARTx->CR1 = 0x0000;                                           // Disable USART before configuration
            GPIO_A->MODER &= ~(0x3U << (2U * 2U));                          // clear PA2
            GPIO_A->MODER |=  (0x2U << (2U * 2U));                          // AF for PA2
            GPIO_A->AFR[0] &= ~(0xFU << (2U * 4U));                         // clear AFRL[11:8]
            GPIO_A->AFR[0] |=  (0x7U << (2U * 4U));                         // AF7 for PA2

            // configuration for USART2 TX on PD5
            RCC->RCC_AHB1ENR |= RCC_AHB1ENR_GPIODEN;                        // GPIOD clock
            GPIO_D->MODER &= ~(0x3U << (5U * 2U));                          // clear PD5
            GPIO_D->MODER |=  (0x2U << (5U * 2U));                          // AF for PD5
            GPIO_D->AFR[0] &= ~(0xFU << (5U * 4U));                         // clear AFRL[23:20]
            GPIO_D->AFR[0] |=  (0x7U << (5U * 4U));                         // AF7 for PD5
        }
        else if (USARTx == USART_3) {
            RCC->RCC_APB1ENR |= RCC_APB1ENR_USART_3EN;                      // USART3 clock (APB1)

            // configuration for USART3 TX on PB10
            RCC->RCC_AHB1ENR |= RCC_AHB1ENR_GPIOBEN;                        // GPIOB clock
            USARTx->CR1 = 0x0000;                                           // Disable USART before configuration
            GPIO_B->MODER &= ~(0x3U << (10U * 2U));                         // clear PB10
            GPIO_B->MODER |=  (0x2U << (10U * 2U));                         // AF for PB10
            // PB10 is in AFRH: index (10-8)=2 -> bits [11:8]
            GPIO_B->AFR[1] &= ~(0xFU << ((10U - 8U) * 4U));                 // clear AFRH[11:8]
            GPIO_B->AFR[1] |=  (0x7U << ((10U - 8U) * 4U));                 // AF7 for PB10

            // configuration for USART3 TX on PD8
            RCC->RCC_AHB1ENR |= RCC_AHB1ENR_GPIODEN;                        // GPIOD clock
            GPIO_D->MODER &= ~(0x3U << (8U * 2U));                          // clear PD8
            GPIO_D->MODER |=  (0x2U << (8U * 2U));                          // AF for PD8
            GPIO_D->AFR[1] &= ~(0xFU << (0U * 4U));                         // clear AFRH[3:0]
            GPIO_D->AFR[1] |=  (0x7U << (0U * 4U));                         // AF7 for PD8
        }
        else if (USARTx == UART_4) {
            RCC->RCC_APB1ENR |= RCC_APB1ENR_UART_4EN;                       // UART4 clock (APB1)

            // configuration for UART4 TX on PA0
            RCC->RCC_AHB1ENR |= RCC_AHB1ENR_GPIOAEN;                        // GPIOA clock
            USARTx->CR1 = 0x0000;                                           // Disable USART before configuration
            GPIO_A->MODER &= ~(0x3U << (0U * 2U));                          // clear PA0
            GPIO_A->MODER |=  (0x2U << (0U * 2U));                          // AF for PA0
            GPIO_A->AFR[0] &= ~(0xFU << (0U * 4U));                         // clear AFRL[3:0]
            GPIO_A->AFR[0] |=  (0x8U << (0U * 4U));                         // AF8 for PA0

            // configuration for UART4 TX on PC10
            RCC->RCC_AHB1ENR |= RCC_AHB1ENR_GPIOCEN;                        // GPIOC clock
            GPIO_C->MODER &= ~(0x3U << (10U * 2U));                         // clear PC10
            GPIO_C->MODER |=  (0x2U << (10U * 2U));                         // AF for PC10
            GPIO_C->AFR[1] &= ~(0xFU << (2U * 4U));                         // clear AFRH[11:8]
            GPIO_C->AFR[1] |=  (0x8U << (2U * 4U));                         // AF8 for PC10
        }
        else if (USARTx == UART_5) {
            RCC->RCC_APB1ENR |= RCC_APB1ENR_UART_5EN;                       // UART5 clock (APB1)
            RCC->RCC_AHB1ENR |= RCC_AHB1ENR_GPIOCEN;                        // GPIOC clock
            USARTx->CR1 = 0x0000;                                           // Disable USART before configuration
            GPIO_C->MODER &= ~(0x3U << (12U * 2U));                         // clear PC12
            GPIO_C->MODER |=  (0x2U << (12U * 2U));                         // AF for PC12
            GPIO_C->AFR[1] &= ~(0xFU << (4U * 4U));                         // clear AFRH[15:12]
            GPIO_C->AFR[1] |=  (0x8U << (4U * 4U));                         // AF8 for PC12
        }
        else if (USARTx == USART_6) {
            RCC->RCC_APB2ENR |= RCC_APB2ENR_USART_6EN;                      // USART6 clock (APB2)
            RCC->RCC_AHB1ENR |= RCC_AHB1ENR_GPIOCEN;                        // GPIOC clock
            USARTx->CR1 = 0x0000;                                           // Disable USART before configuration
            GPIO_C->MODER &= ~(0x3U << (6U * 2U));                          // clear PC6
            GPIO_C->MODER |=  (0x2U << (6U * 2U));                          // AF for PC6
            GPIO_C->AFR[0] &= ~(0xFU << (6U * 4U));                         // clear AFRL[27:24]
            GPIO_C->AFR[0] |=  (0x8U << (6U * 4U));                         // AF8 for PC6
        }
    }

    // USART RX pin configuration
    if (comtype == RX_ONLY || comtype == RX_AND_TX) 
    {
        if (USARTx == USART_1) {
            RCC->RCC_APB2ENR |= RCC_APB2ENR_USART_1EN;                      // USART1 clock (APB2)

            // configuration for USART1 RX on PB7
            RCC->RCC_AHB1ENR |= RCC_AHB1ENR_GPIOBEN;                        // GPIOB clock
            USARTx->CR1 = 0x0000;                                           // Disable USART before configuration
            GPIO_B->MODER &= ~(0x3U << (7U * 2U));                          // clear PB7
            GPIO_B->MODER |=  (0x2U << (7U * 2U));                          // AF for PB7
            GPIO_B->AFR[0] &= ~(0xFU << (7U * 4U));                         // clear AFRL[31:28]
            GPIO_B->AFR[0] |=  (0x7U << (7U * 4U));                         // AF7 for PB7

            // configuration for USART1 RX on PA10
            RCC->RCC_AHB1ENR |= RCC_AHB1ENR_GPIOAEN;                        // GPIOA clock
            GPIO_A->MODER &= ~(0x3U << (10U * 2U));                         // clear PA10
            GPIO_A->MODER |=  (0x2U << (10U * 2U));                         // AF for PA10
            GPIO_A->AFR[1] &= ~(0xFU << (2U * 4U));                         // clear AFRH[11:8]
            GPIO_A->AFR[1] |=  (0x7U << (2U * 4U));                         // AF7 for PA10
        }
        else if (USARTx == USART_2) {
            RCC->RCC_APB1ENR |= RCC_APB1ENR_USART_2EN;                      // USART2 clock (APB1)
            
            // configuration for USART2 RX on PA3
            RCC->RCC_AHB1ENR |= RCC_AHB1ENR_GPIOAEN;                        // GPIOA clock
            USARTx->CR1 = 0x0000;                                           // Disable USART before configuration
            GPIO_A->MODER &= ~(0x3U << (3U * 2U));                          // clear PA3
            GPIO_A->MODER |=  (0x2U << (3U * 2U));                          // AF for PA3
            GPIO_A->AFR[0] &= ~(0xFU << (3U * 4U));                         // clear AFRL[15:12]
            GPIO_A->AFR[0] |=  (0x7U << (3U * 4U));                         // AF7 for PA3
            
            // configuration for USART2 RX on PD6
            RCC->RCC_AHB1ENR |= RCC_AHB1ENR_GPIODEN;                        // GPIOD clock
            GPIO_D->MODER &= ~(0x3U << (6U * 2U));                          // clear PD6
            GPIO_D->MODER |=  (0x2U << (6U * 2U));                          // AF for PD6
            GPIO_D->AFR[0] &= ~(0xFU << (6U * 4U));                         // clear AFRL[27:24]
            GPIO_D->AFR[0] |=  (0x7U << (6U * 4U));                         // AF7 for PD6
        }
        else if (USARTx == USART_3) {
            RCC->RCC_APB1ENR |= RCC_APB1ENR_USART_3EN;                      // USART3 clock (APB1)

            // configuration for USART3 RX on PB11
            RCC->RCC_AHB1ENR |= RCC_AHB1ENR_GPIOBEN;                        // GPIOB clock
            USARTx->CR1 = 0x0000;                                           // Disable USART before configuration
            GPIO_B->MODER &= ~(0x3U << (11U * 2U));                         // clear PB11
            GPIO_B->MODER |=  (0x2U << (11U * 2U));                         // AF for PB11
            // PB11 is in AFRH: index (11-8)=3 -> bits [15:12]
            GPIO_B->AFR[1] &= ~(0xFU << ((11U - 8U) * 4U));                 // clear AFRH[15:12]
            GPIO_B->AFR[1] |=  (0x7U << ((11U - 8U) * 4U));                 // AF7 for PB11

            // configuration for USART3 RX on PD9
            RCC->RCC_AHB1ENR |= RCC_AHB1ENR_GPIODEN;                        // GPIOD clock
            GPIO_D->MODER &= ~(0x3U << (9U * 2U));                          // clear PD9
            GPIO_D->MODER |=  (0x2U << (9U * 2U));                          // AF for PD9
            GPIO_D->AFR[1] &= ~(0xFU << (1U * 4U));                         // clear AFRH[15:12]
            GPIO_D->AFR[1] |=  (0x7U << (1U * 4U));                         // AF7 for PD9
        }
        else if (USARTx == UART_4) {
            RCC->RCC_APB1ENR |= RCC_APB1ENR_UART_4EN;                       // UART4 clock (APB1)

            // configuration for UART4 RX on PA1
            RCC->RCC_AHB1ENR |= RCC_AHB1ENR_GPIOAEN;                        // GPIOA clock
            USARTx->CR1 = 0x0000;                                           // Disable USART before configuration
            GPIO_A->MODER &= ~(0x3U << (1U * 2U));                          // clear PA1
            GPIO_A->MODER |=  (0x2U << (1U * 2U));                          // AF for PA1
            GPIO_A->AFR[0] &= ~(0xFU << (1U * 4U));                         // clear AFRL[7:4]
            GPIO_A->AFR[0] |=  (0x8U << (1U * 4U));                         // AF8 for PA1

            // configuration for UART4 RX on PC11
            RCC->RCC_AHB1ENR |= RCC_AHB1ENR_GPIOCEN;                        // GPIOC clock
            GPIO_C->MODER &= ~(0x3U << (11U * 2U));                         // clear PC11
            GPIO_C->MODER |=  (0x2U << (11U * 2U));                         // AF for PC11
            GPIO_C->AFR[1] &= ~(0xFU << (3U * 4U));                         // clear AFRH[15:12]
            GPIO_C->AFR[1] |=  (0x8U << (3U * 4U));                         // AF8 for PC11
        }
        else if (USARTx == UART_5) {
            RCC->RCC_APB1ENR |= RCC_APB1ENR_UART_5EN;                       // UART5 clock (APB1)
            RCC->RCC_AHB1ENR |= RCC_AHB1ENR_GPIODEN;                        // GPIOD clock
            USARTx->CR1 = 0x0000;                                           // Disable USART before configuration
            GPIO_D->MODER &= ~(0x3U << (2U * 2U));                          // clear PD2
            GPIO_D->MODER |=  (0x2U << (2U * 2U));                          // AF for PD2
            GPIO_D->AFR[0] &= ~(0xFU << (2U * 4U));                         // clear AFRL[11:8]
            GPIO_D->AFR[0] |=  (0x8U << (2U * 4U));                         // AF8 for PD2
        }
        else if (USARTx == USART_6) {
            RCC->RCC_APB2ENR |= RCC_APB2ENR_USART_6EN;                      // USART6 clock (APB2)
            RCC->RCC_AHB1ENR |= RCC_AHB1ENR_GPIOCEN;                        // GPIOC clock
            USARTx->CR1 = 0x0000;                                           // Disable USART before configuration
            GPIO_C->MODER &= ~(0x3U << (7U * 2U));                          // clear PC7
            GPIO_C->MODER |=  (0x2U << (7U * 2U));                          // AF for PC7
            GPIO_C->AFR[0] &= ~(0xFU << (7U * 4U));                         // clear AFRL[31:28]
            GPIO_C->AFR[0] |=  (0x8U << (7U * 4U));                         // AF8 for PC7
        }
    }

    // 4) Baud rate before enabling UE
    switch (baudrate) {
        case __115200:
            USARTx->BRR = BRR_CNF1_115200;
            break;
        case __9600:
        default:
            USARTx->BRR = BRR_CNF2_9600;
            break;
    }

    // 5) 1 stop bit, no flow control
    USARTx->CR2 = CR2_CNF1;
    USARTx->CR3 = CR3_CNF1;

    // 6) Enable TX/RX as requested, then UE last
    switch (comtype) {
        case TX_ONLY:
            USARTx->CR1 = USART_CR1_TX_EN;
            break;
        case RX_ONLY:
            USARTx->CR1 = USART_CR1_RX_EN;
            break;
        case RX_AND_TX:
        default:
            USARTx->CR1 = (USART_CR1_TX_EN | USART_CR1_RX_EN);
            break;
    }
    
    USARTx->CR1 |= USART_CR1_EN;                                            // Enable USART at the end of initialization
}


void USART_x_Write(USART_Manual_TypeDef *USARTx, int ch)
{
    //1. wait until Transmitter Empty flag is set in the SR register
    while(!(USARTx->SR & USART_SR_TX_EMP));

    //2. write to DR register
    USARTx->DR = (ch & 0xFF);
}

char USART_x_Read(USART_Manual_TypeDef *USARTx)
{
    //1. wait until Receiver not Empty flag is set in the SR register
    while(!(USARTx->SR & USART_SR_RX_NOT_EMP));

    //2. read data from DR register
    return (USARTx->DR & 0xFF);
}

int get_char(USART_Manual_TypeDef *USARTx, FILE *f) {
    int c;
    c = USART_x_Read(USARTx);                                               //read a character from USART_x
    if (c == '\r') {                                                        // if received character is carriage return
        USART_x_Write(USARTx, c);                                           // send carriage return
        c = '\n';                                                           // change carriage return to newline
    }
    USART_x_Write(USARTx, c);                                               // send the character
    return c;
}

int send_char(USART_Manual_TypeDef *USARTx, int c, FILE *f) {
    USART_x_Write(USARTx, c);                                               // write a character to USART_x
    return c;
}
