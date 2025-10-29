#include "uart.h"

void USART_x_Init(USART_Manual_TypeDef *USARTx, UART_COMType comtype, UART_BaudRateType baudrate)
{
    // 1) Enable clocks
    RCC->RCC_APB1ENR |= RCC_APB1ENR_USART_2EN;                      // USART2 clock (APB1)
    RCC->RCC_AHB1ENR |= RCC_AHB1ENR_GPIOAEN;                        // GPIOA clock

    // 2) Disable USART before configuration
    USARTx->CR1 = 0x0000;                                          // UE=0, clear CR1 config

    // 3) Configure the GPIO pins for AF for USARTx TX and RX
    if (comtype == TX_ONLY || comtype == RX_AND_TX) {
        GPIO_A->MODER &= ~(0x3U << (2U * 2U));                      // clear PA2
        GPIO_A->MODER |=  (0x2U << (2U * 2U));                      // AF for PA2
        GPIO_A->AFR[0] &= ~(0xFU << (2U * 4U));                     // clear AFRL[11:8]
        GPIO_A->AFR[0] |=  (0x7U << (2U * 4U));                     // AF7 for PA2
    }
    if (comtype == RX_ONLY || comtype == RX_AND_TX) {
        GPIO_A->MODER &= ~(0x3U << (3U * 2U));                      // clear PA3
        GPIO_A->MODER |=  (0x2U << (3U * 2U));                      // AF for PA3
        GPIO_A->AFR[0] &= ~(0xFU << (3U * 4U));                     // clear AFRL[15:12]
        GPIO_A->AFR[0] |=  (0x7U << (3U * 4U));                     // AF7 for PA3
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
    USARTx->CR1 |= USART_CR1_EN;                                   // UE=1
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

int fgetc(FILE *f) {
    int c;
    c = USART_x_Read(USART_2);                                             //read a character from USART_x
    if (c == '\r') {                                                // if received character is carriage return
        USART_x_Write(USART_2, c);                                           // send carriage return
        c = '\n';                                                   // change carriage return to newline
    }
    USART_x_Write(USART_2, c);                                               // send the character
    return c;
}

int fputc(int c, FILE *f) {
    USART_x_Write(USART_2, c);                                               // write a character to USART_x
    return c;
}
