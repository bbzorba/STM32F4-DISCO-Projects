#include "uart.h"

UARTCom::UARTCom(UART_COMType _comType, UART_BaudRateType _baudRate)
{   
    this->comType = _comType;
    this->baudRate = _baudRate;

    RCC->APB1ENR |= RCC_APB1ENR_USART_2EN;   // Enable clock access to USART2
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;      // Enable clock access to GPIOA
    USART_2->CR1 |= USART_2_CR1_DIS;         // Disable USART2

    switch (_comType)
    {
    case TX_ONLY:
        GPIOA->MODER   |= GPIOA_MODER_PIN2;  // Set PA2 to Alternate Function (TX)
        GPIOA->AFR[0]  |= GPIOA_AFR_TX;   // Set AF7 for PA2 (TX)

        USART_2->CR1 |= (USART_CR1_TE);      // Enable transmitter only, 8 bit data
        USART_2->CR1 |= USART_CR1_UE;      // Enable USART2
        break;
    case RX_ONLY:
        GPIOA->MODER   |= GPIOA_MODER_PIN3;  // Set PA3 to Alternate Function (RX)
        GPIOA->AFR[0]  |= GPIOA_AFR_RX;   // Set AF7 for PA3 (RX)

        USART_2->CR1 |= (USART_CR1_RE);      // Enable receiver only, 8 bit data
        USART_2->CR1 |= USART_CR1_UE;      // Enable USART2
        break;

    case RX_AND_TX:
        GPIOA->MODER   |= (GPIOA_MODER_PIN2 | GPIOA_MODER_PIN3); // Set PA2 and PA3 to Alternate Function (TX and RX)
        GPIOA->AFR[0]  |= GPIOA_AFR_VALUE;   // Set AF7 for PA2 and PA3 (TX and RX)

        USART_2->CR1 |= (USART_CR1_TE | USART_CR1_RE);            // Enable transmitter and receiver
        USART_2->CR1 |= USART_CR1_UE;      // Enable USART2
        break;

    default:
        break;
    }

    switch (_baudRate)
    {
    case __115200:
        USART_2->BRR = BRR_CNF1_115200;              // Set baud rate to 115200
        break;
    case __9600:
        USART_2->BRR = BRR_CNF2_9600;              // Set baud rate to 9600
        break;
    default:
        break;
    }

    USART_2->CR2 = CR2_CNF1;                  // 1 stop bit, no-op
    USART_2->CR3 = CR3_CNF1;                  // No flow control, no-op
    USART_2->CR1 |= USART_CR1_UE;          // Enable USART2

}

void UARTCom::USART_2_Write(int ch)
{
    //1. wait until TXE is set in the SR register
    while(!(USART_2->SR & USART_SR_TXE));

    //2. write to DR register
    USART_2->DR = (ch & 0xFF);
}

char UARTCom::USART_2_Read(void)
{
    //1. wait until RXNE is set in the SR register
    while(!(USART_2->SR & USART_SR_RXNE));

    //2. read data from DR register
    return (USART_2->DR & 0xFF);
}

UART_COMType UARTCom::getComType() const {
    return this->comType;
}

UART_BaudRateType UARTCom::getBaudRate() const {
    return this->baudRate;
}
