/* 
 USART pins for STM32F4xx series:
 USART1 -> PB6 (TX), PB7 (RX)
 USART2 -> PA2 (TX), PA3 (RX) or PD5 (TX), PD6 (RX)
 USART3 -> PB10 (TX), PB11 (RX) or PD8 (TX), PD9 (RX) or PC10 (TX), PC11(RX)
 UART4 -> PA0 (TX), PA1 (RX) or PC10 (TX), PC11 (RX)
 UART5 -> PC12 (TX), PD2 (RX)
 USART6 -> PC6 (TX), PC7 (RX)*/

#include "uart.h"

USART::USART(USART_Manual_TypeDef *uartx, UART_COMType _comType, UART_BaudRateType _baudRate)
{   
    this->USARTx = uartx;
    this->comType = _comType;
    this->baudRate = _baudRate;

    // USART TX pin configuration
    if (_comType == TX_ONLY || _comType == RX_AND_TX) 
    {
        if (this->USARTx == USART_1) {
            RCC->RCC_APB2ENR |= RCC_APB2ENR_USART_1EN;                      // USART1 clock (APB2)

            // configuration for USART1 TX on PB6
            RCC->RCC_AHB1ENR |= RCC_AHB1ENR_GPIOBEN;                        // GPIOB clock
            this->USARTx->CR1 = 0x0000;                                           // Disable USART before configuration
            GPIO_B->MODER &= ~MODER_PIN6_MASK;                               // clear PB6
            GPIO_B->MODER |=  MODER_PIN6_SET;                               // Set AF for PB6 (bits 13:12)
            GPIO_B->AFR[0] &= ~AFRL_PIN6_MASK;                              // clear AFRL[27:24]
            GPIO_B->AFR[0] |=  AFRL_PIN6_SET_AF7;                           // AF7 for PB6

            // configuration for USART1 TX on PA9
            RCC->RCC_AHB1ENR |= RCC_AHB1ENR_GPIOAEN;                        // GPIOA clock
            GPIO_A->MODER &= ~MODER_PIN9_MASK;                          // clear PA9
            GPIO_A->MODER |=  MODER_PIN9_SET;                          // AF for PA9
            GPIO_A->AFR[1] &= ~AFRH_PIN9_MASK;                         // clear AFRH[11:8]
            GPIO_A->AFR[1] |=  AFRH_PIN9_SET_AF7;                         // AF7 for PA9
        }
        else if (this->USARTx == USART_2) {
            RCC->RCC_APB1ENR |= RCC_APB1ENR_USART_2EN;                      // USART2 clock (APB1)
            
            // configuration for USART2 TX on PA2
            RCC->RCC_AHB1ENR |= RCC_AHB1ENR_GPIOAEN;                        // GPIOA clock
            this->USARTx->CR1 = 0x0000;                                           // Disable USART before configuration
            GPIO_A->MODER &= ~MODER_PIN2_MASK;                          // clear PA2
            GPIO_A->MODER |=  MODER_PIN2_SET;                          // AF for PA2
            GPIO_A->AFR[0] &= ~AFRL_PIN2_MASK;                         // clear AFRL[11:8]
            GPIO_A->AFR[0] |=  AFRL_PIN2_SET_AF7;                         // AF7 for PA2

            // configuration for USART2 TX on PD5
            RCC->RCC_AHB1ENR |= RCC_AHB1ENR_GPIODEN;                        // GPIOD clock
            GPIO_D->MODER &= ~MODER_PIN5_MASK;                          // clear PD5
            GPIO_D->MODER |=  MODER_PIN5_SET;                          // AF for PD5
            GPIO_D->AFR[0] &= ~AFRL_PIN5_MASK;                         // clear AFRL[23:20]
            GPIO_D->AFR[0] |=  AFRL_PIN5_SET_AF7;                         // AF7 for PD5
        }
        else if (this->USARTx == USART_3) {
            RCC->RCC_APB1ENR |= RCC_APB1ENR_USART_3EN;                      // USART3 clock (APB1)

            // configuration for USART3 TX on PB10
            RCC->RCC_AHB1ENR |= RCC_AHB1ENR_GPIOBEN;                        // GPIOB clock
            this->USARTx->CR1 = 0x0000;                                           // Disable USART before configuration
            GPIO_B->MODER &= ~MODER_PIN10_MASK;                         // clear PB10
            GPIO_B->MODER |=  MODER_PIN10_SET;                         // AF for PB10
            GPIO_B->AFR[1] &= ~AFRH_PIN10_MASK;                       // clear AFRH[11:8]
            GPIO_B->AFR[1] |=  AFRH_PIN10_SET_AF7;                       // AF7 for PB10

            // configuration for USART3 TX on PD8
            RCC->RCC_AHB1ENR |= RCC_AHB1ENR_GPIODEN;                        // GPIOD clock
            GPIO_D->MODER &= ~MODER_PIN8_MASK;                          // clear PD8
            GPIO_D->MODER |=  MODER_PIN8_SET;                          // AF for PD8
            GPIO_D->AFR[1] &= ~AFRH_PIN8_MASK;                         // clear AFRH[3:0]
            GPIO_D->AFR[1] |=  AFRH_PIN8_SET_AF7;                         // AF7 for PD8

            // configuration for USART3 TX on PC10
            RCC->RCC_AHB1ENR |= RCC_AHB1ENR_GPIOCEN;                        // GPIOC clock
            GPIO_C->MODER &= ~MODER_PIN10_MASK;                         // clear PC10
            GPIO_C->MODER |=  MODER_PIN10_SET;                         // AF for PC10
            GPIO_C->AFR[1] &= ~AFRH_PIN10_MASK;                         // clear AFRH[11:8]
            GPIO_C->AFR[1] |=  AFRH_PIN10_SET_AF7;                         // AF7 for PC10
        }
        else if (this->USARTx == UART_4) {
            RCC->RCC_APB1ENR |= RCC_APB1ENR_UART_4EN;                       // UART4 clock (APB1)

            // configuration for UART4 TX on PA0
            RCC->RCC_AHB1ENR |= RCC_AHB1ENR_GPIOAEN;                        // GPIOA clock
            this->USARTx->CR1 = 0x0000;                                           // Disable USART before configuration
            GPIO_A->MODER &= ~MODER_PIN0_MASK;                          // clear PA0
            GPIO_A->MODER |=  MODER_PIN0_SET;                          // AF for PA0
            GPIO_A->AFR[0] &= ~AFRL_PIN0_MASK;                         // clear AFRL[3:0]
            GPIO_A->AFR[0] |=  AFRL_PIN0_SET_AF8;                         // AF8 for PA0

            // configuration for UART4 TX on PC10
            RCC->RCC_AHB1ENR |= RCC_AHB1ENR_GPIOCEN;                        // GPIOC clock
            GPIO_C->MODER &= ~MODER_PIN10_MASK;                         // clear PC10
            GPIO_C->MODER |=  MODER_PIN10_SET;                         // AF for PC10
            GPIO_C->AFR[1] &= ~AFRH_PIN10_MASK;                         // clear AFRH[11:8]
            GPIO_C->AFR[1] |=  AFRH_PIN10_SET_AF8;                         // AF8 for PC10
        }
        else if (this->USARTx == UART_5) {
            RCC->RCC_APB1ENR |= RCC_APB1ENR_UART_5EN;                       // UART5 clock (APB1)

            // configuration for UART5 TX on PC12
            RCC->RCC_AHB1ENR |= RCC_AHB1ENR_GPIOCEN;                        // GPIOC clock
            this->USARTx->CR1 = 0x0000;                                           // Disable USART before configuration
            GPIO_C->MODER &= ~MODER_PIN12_MASK;                         // clear PC12
            GPIO_C->MODER |=  MODER_PIN12_SET;                         // AF for PC12
            GPIO_C->AFR[1] &= ~AFRH_PIN12_MASK;                         // clear AFRH[15:12]
            GPIO_C->AFR[1] |=  AFRH_PIN12_SET_AF8;                         // AF8 for PC12
        }
        else if (this->USARTx == USART_6) {
            RCC->RCC_APB2ENR |= RCC_APB2ENR_USART_6EN;                      // USART6 clock (APB2)

            // configuration for USART6 TX on PC6
            RCC->RCC_AHB1ENR |= RCC_AHB1ENR_GPIOCEN;                        // GPIOC clock
            this->USARTx->CR1 = 0x0000;                                           // Disable USART before configuration
            GPIO_C->MODER &= ~MODER_PIN6_MASK;                          // clear PC6
            GPIO_C->MODER |=  MODER_PIN6_SET;                          // AF for PC6
            GPIO_C->AFR[0] &= ~AFRL_PIN6_MASK;                         // clear AFRL[27:24]
            GPIO_C->AFR[0] |=  AFRL_PIN6_SET_AF8;                         // AF8 for PC6
        }
    }

    // USART RX pin configuration
    if (_comType == RX_ONLY || _comType == RX_AND_TX) 
    {
        if (this->USARTx == USART_1) {
            RCC->RCC_APB2ENR |= RCC_APB2ENR_USART_1EN;                      // USART1 clock (APB2)

            // configuration for USART1 RX on PB7
            RCC->RCC_AHB1ENR |= RCC_AHB1ENR_GPIOBEN;                        // GPIOB clock
            this->USARTx->CR1 = 0x0000;                                           // Disable USART before configuration
            GPIO_B->MODER &= ~MODER_PIN7_MASK;                          // clear PB7
            GPIO_B->MODER |=  MODER_PIN7_SET;                          // AF for PB7
            GPIO_B->AFR[0] &= ~AFRL_PIN7_MASK;                         // clear AFRL[31:28]
            GPIO_B->AFR[0] |=  AFRL_PIN7_SET_AF7;                           // AF7 for PB7

            // configuration for USART1 RX on PA10
            RCC->RCC_AHB1ENR |= RCC_AHB1ENR_GPIOAEN;                        // GPIOA clock
            GPIO_A->MODER &= ~MODER_PIN10_MASK;                         // clear PA10
            GPIO_A->MODER |=  MODER_PIN10_SET;                         // AF for PA10
            GPIO_A->AFR[1] &= ~AFRH_PIN10_MASK;                         // clear AFRH[11:8]
            GPIO_A->AFR[1] |=  AFRH_PIN10_SET_AF7;                      // AF7 for PA10
        }
        else if (this->USARTx == USART_2) {
            RCC->RCC_APB1ENR |= RCC_APB1ENR_USART_2EN;                      // USART2 clock (APB1)
            
            // configuration for USART2 RX on PA3
            RCC->RCC_AHB1ENR |= RCC_AHB1ENR_GPIOAEN;                        // GPIOA clock
            this->USARTx->CR1 = 0x0000;                                           // Disable USART before configuration
            GPIO_A->MODER &= ~MODER_PIN3_MASK;                         // clear PA3
            GPIO_A->MODER |=  MODER_PIN3_SET;                         // AF for PA3
            GPIO_A->AFR[0] &= ~AFRL_PIN3_MASK;                         // clear AFRL[15:12]
            GPIO_A->AFR[0] |=  AFRL_PIN3_SET_AF7;                         // AF7 for PA3

            // configuration for USART2 RX on PD6
            RCC->RCC_AHB1ENR |= RCC_AHB1ENR_GPIODEN;                        // GPIOD clock
            GPIO_D->MODER &= ~MODER_PIN6_MASK;                          // clear PD6
            GPIO_D->MODER |=  MODER_PIN6_SET;                          // AF for PD6
            GPIO_D->AFR[0] &= ~AFRL_PIN6_MASK;                         // clear AFRL[27:24]
            GPIO_D->AFR[0] |=  AFRL_PIN6_SET_AF7;                         // AF7 for PD6
        }
        else if (this->USARTx == USART_3) {
            RCC->RCC_APB1ENR |= RCC_APB1ENR_USART_3EN;                      // USART3 clock (APB1)

            // configuration for USART3 RX on PB11
            RCC->RCC_AHB1ENR |= RCC_AHB1ENR_GPIOBEN;                        // GPIOB clock
            this->USARTx->CR1 = 0x0000;                                           // Disable USART before configuration
            GPIO_B->MODER &= ~MODER_PIN11_MASK;                         // clear PB11
            GPIO_B->MODER |=  MODER_PIN11_SET;                         // AF for PB11
            GPIO_B->AFR[1] &= ~AFRH_PIN11_MASK;                       // clear AFRH[15:12]
            GPIO_B->AFR[1] |=  AFRH_PIN11_SET_AF7;                       // AF7 for PB11

            // configuration for USART3 RX on PD9
            RCC->RCC_AHB1ENR |= RCC_AHB1ENR_GPIODEN;                        // GPIOD clock
            GPIO_D->MODER &= ~MODER_PIN9_MASK;                          // clear PD9
            GPIO_D->MODER |=  MODER_PIN9_SET;                          // AF for PD9
            GPIO_D->AFR[1] &= ~AFRH_PIN9_MASK;                         // clear AFRH[15:12]
            GPIO_D->AFR[1] |=  AFRH_PIN9_SET_AF7;                         // AF7 for PD9

            //configuration for USART3 RX on PC11
            RCC->RCC_AHB1ENR |= RCC_AHB1ENR_GPIOCEN;                        // GPIOC clock
            GPIO_C->MODER &= ~MODER_PIN11_MASK;                         // clear PC11
            GPIO_C->MODER |=  MODER_PIN11_SET;                         // AF for PC11
            GPIO_C->AFR[1] &= ~AFRH_PIN11_MASK;                         // clear AFRH[15:12]
            GPIO_C->AFR[1] |=  AFRH_PIN11_SET_AF7;                         // AF7 for PC11
        }
        else if (this->USARTx == UART_4) {
            RCC->RCC_APB1ENR |= RCC_APB1ENR_UART_4EN;                       // UART4 clock (APB1)

            // configuration for UART4 RX on PA1
            RCC->RCC_AHB1ENR |= RCC_AHB1ENR_GPIOAEN;                        // GPIOA clock
            this->USARTx->CR1 = 0x0000;                                           // Disable USART before configuration
            // Enable pull-up on RX to keep line idle-high and reduce noise
            GPIO_A->MODER &= ~MODER_PIN1_MASK;                          // clear PA1
            GPIO_A->MODER |=  MODER_PIN1_SET;                          // AF for PA1
            GPIO_A->AFR[0] &= ~AFRL_PIN1_MASK;                         // clear AFRL[7:4]
            GPIO_A->AFR[0] |=  AFRL_PIN1_SET_AF8;                         // AF8 for PA1

            // configuration for UART4 RX on PC11
            RCC->RCC_AHB1ENR |= RCC_AHB1ENR_GPIOCEN;                        // GPIOC clock
            GPIO_C->MODER &= ~MODER_PIN11_MASK;                         // clear PC11
            GPIO_C->MODER |=  MODER_PIN11_SET;                         // AF for PC11
            GPIO_C->AFR[1] &= ~AFRH_PIN11_MASK;                         // clear AFRH[15:12]
            GPIO_C->AFR[1] |=  AFRH_PIN11_SET_AF8;                         // AF8 for PC11
        }
        else if (this->USARTx == UART_5) {
            RCC->RCC_APB1ENR |= RCC_APB1ENR_UART_5EN;                       // UART5 clock (APB1)

            // configuration for UART5 RX on PD2
            RCC->RCC_AHB1ENR |= RCC_AHB1ENR_GPIODEN;                        // GPIOD clock
            this->USARTx->CR1 = 0x0000;                                           // Disable USART before configuration
            GPIO_D->MODER &= ~MODER_PIN2_MASK;                          // clear PD2
            GPIO_D->MODER |=  MODER_PIN2_SET;                          // AF for PD2
            GPIO_D->AFR[0] &= ~AFRL_PIN2_MASK;                         // clear AFRL[11:8]
            GPIO_D->AFR[0] |=  AFRL_PIN2_SET_AF8;                         // AF8 for PD2
        }
        else if (this->USARTx == USART_6) {
            RCC->RCC_APB2ENR |= RCC_APB2ENR_USART_6EN;                      // USART6 clock (APB2)

            // configuration for USART6 RX on PC7
            RCC->RCC_AHB1ENR |= RCC_AHB1ENR_GPIOCEN;                        // GPIOC clock
            this->USARTx->CR1 = 0x0000;                                           // Disable USART before configuration
            GPIO_C->MODER &= ~MODER_PIN7_MASK;                          // clear PC7
            GPIO_C->MODER |=  MODER_PIN7_SET;                          // AF for PC7
            GPIO_C->AFR[0] &= ~AFRL_PIN7_MASK;                         // clear AFRL[31:28]
            GPIO_C->AFR[0] |=  AFRL_PIN7_SET_AF8;                         // AF8 for PC7
        }
    }

    // 4) Clear any stale status by a dummy SR/DR read, then set baud
    (void)this->USARTx->SR; (void)this->USARTx->DR;
    // 4) Baud rate before enabling UE; derive from bus clock.
    uint32_t baud_val = (_baudRate == __115200) ? 115200U : 9600U;
    uint32_t fck_hz = (this->USARTx == USART_1 || this->USARTx == USART_6) ? APB2_CLK_HZ : APB1_CLK_HZ;
    this->USARTx->BRR = this->BRR_Oversample_by_16(fck_hz, baud_val);

    // 5) 1 stop bit, no flow control
    this->USARTx->CR2 = CR2_CNF1;
    this->USARTx->CR3 = CR3_CNF1;

    // 6) Enable TX/RX as requested, then UE last
    switch (_comType) {
        case TX_ONLY:
            this->USARTx->CR1 = USART_CR1_TX_EN;
            break;
        case RX_ONLY:
            this->USARTx->CR1 = USART_CR1_RX_EN;
            break;
        case RX_AND_TX:
        default:
            this->USARTx->CR1 = (USART_CR1_TX_EN | USART_CR1_RX_EN);
            break;
    }
    
    this->USARTx->CR1 |= USART_CR1_EN;                                            // Enable USART at the end of initialization

}

// Compute BRR for oversampling by 16
uint16_t USART::BRR_Oversample_by_16(uint32_t fck_hz, uint32_t baud) {
    return (uint16_t)( (fck_hz + (baud / 2U)) / baud );
}

void USART::USART_x_Write(int ch)
{
    //1. wait until Transmitter Empty flag is set in the SR register
    while(!(this->USARTx->SR & USART_SR_TX_EMP));

    //2. write to DR register
    this->USARTx->DR = (ch & 0xFF);
}

char USART::USART_x_Read()
{
    //1. wait until Receiver not Empty flag is set in the SR register
    while(!(this->USARTx->SR & USART_SR_RX_NOT_EMP));

    //2. read data from DR register
    return (this->USARTx->DR & 0xFF);
}

int USART::get_char(FILE *f) {
    int c;
    c = this->USART_x_Read();                                               //read a character from USART_x
    if (c == '\r') {                                                        // if received character is carriage return
        this->USART_x_Write(c);                                           // send carriage return
        c = '\n';                                                           // change carriage return to newline
    }
    this->USART_x_Write(c);                                               // send the character
    return c;
}

int USART::send_char(int c, FILE *f) {
    this->USART_x_Write(c);                                               // write a character to USART_x
    return c;
}

void USART::writeString(const char *str) {
    this->USART_x_Write('\n');
    this->USART_x_Write('r'); // Carriage return before newline
    this->USART_x_Write('x');
    this->USART_x_Write(':');
    this->USART_x_Write(' ');
    this->USART_x_Write('\n');
    while (*str) {
        this->USART_x_Write(*str++);
    }
}

void USART::readString(char *buffer, size_t maxLength) {
    size_t index = 0;
    char c;
    while (index < (maxLength - 1)) { // Leave space for null terminator
        c = this->USART_x_Read();
        if (c == '\n' || c == '\r') { // Stop on newline or carriage return
            break;
        }
        buffer[index++] = c;
    }
    buffer[index] = '\0'; // Null-terminate the string
}


UART_COMType USART::getComType() const {
    return this->comType;
}

UART_BaudRateType USART::getBaudRate() const {
    return this->baudRate;
}
