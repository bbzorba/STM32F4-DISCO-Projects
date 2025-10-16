#include "uart.h"

void USART_2_Init(UART_COMType comtype, UART_BaudRateType baudrate)
{
    RCC->RCC_APB1ENR |= RCC_APB1ENR_USART_2EN;                      // Enable clock access to USART2
    RCC->RCC_AHB1ENR |= RCC_AHB1ENR_GPIOAEN;                        // Enable clock access to GPIOA
    USART_2->CR1 |= USART_2_CR1_DIS;                                // Disable USART2

    switch (comtype)
    {
    case TX_ONLY:
        GPIO_A->MODER   |= GPIOA_MODER_PIN2;                         // Set PA2 to Alternate Function (TX)
        GPIO_A->AFR[0]  |= GPIOA_AFR_TX;                             // Set AF7 for PA2 (TX)

        USART_2->CR1 |= (USART_CR1_TX_EN);                          // Enable transmitter only, 8 bit data
        USART_2->CR1 |= USART_CR1_EN;                               // Enable USART2
        break;

    case RX_ONLY:
        GPIO_A->MODER   |= GPIOA_MODER_PIN3;                         // Set PA3 to Alternate Function (RX)
        GPIO_A->AFR[0]  |= GPIOA_AFR_RX;                             // Set AF7 for PA3 (RX)

        USART_2->CR1 |= (USART_CR1_RX_EN);                          // Enable receiver only, 8 bit data
        USART_2->CR1 |= USART_CR1_EN;                               // Enable USART2
        break;

    case RX_AND_TX:
        GPIO_A->MODER   |= (GPIOA_MODER_PIN2 | GPIOA_MODER_PIN3);    // Set PA2 and PA3 to Alternate Function (TX and RX)
        GPIO_A->AFR[0]  |= GPIOA_AFR_VALUE;                          // Set AF7 for PA2 and PA3 (TX and RX)

        USART_2->CR1 |= (USART_CR1_TX_EN | USART_CR1_RX_EN);        // Enable transmitter and receiver
        USART_2->CR1 |= USART_CR1_EN;                               // Enable USART2
        break;

    default:
        break;
    }

    switch (baudrate)
    {
    case __115200:
        USART_2->BRR = BRR_CNF1_115200;                             // Set baud rate to 115200
        break;

    case __9600:
        USART_2->BRR = BRR_CNF2_9600;                               // Set baud rate to 9600
        break;

    default:
        break;
    }

    USART_2->CR2 = CR2_CNF1;                                        // 1 stop bit, no-op
    USART_2->CR3 = CR3_CNF1;                                        // No flow control, no-op
    USART_2->CR1 |= USART_CR1_EN;                                   // Enable USART2

}

void USART_2_Write(int ch)
{
    //1. wait until Transmitter Empty flag is set in the SR register
    while(!(USART_2->SR & USART_SR_TX_EMP));

    //2. write to DR register
    USART_2->DR = (ch & 0xFF);
}

char USART_2_Read(void)
{
    //1. wait until Receiver not Empty flag is set in the SR register
    while(!(USART_2->SR & USART_SR_RX_NOT_EMP));

    //2. read data from DR register
    return (USART_2->DR & 0xFF);
}

int fgetc(FILE *f) {
    int c;
    c = USART_2_Read();                                             //read a character from USART_2
    if (c == '\r') {                                                // if received character is carriage return
        USART_2_Write(c);                                       // send carriage return
        c = '\n';                                                   // change carriage return to newline
    }
    USART_2_Write(c);                                           // send the character
    return c;
}

int fputc(int c, FILE *f) {
    USART_2_Write(c);                                           // write a character to USART_2
    return c;
}
