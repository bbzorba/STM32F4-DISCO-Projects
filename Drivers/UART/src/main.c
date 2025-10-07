#include "uart.h"

// Function prototypes
void USART_2_Init(void);
void delay(volatile uint32_t count);

//main function
int main(void) {
    USART_2_Init();

    
    while (1) {
        USART_2_Write('H');
        USART_2_Write('e');
        USART_2_Write('l');
        USART_2_Write('l');
        USART_2_Write('o');
        USART_2_Write('\n');
        delay(1000000); // ~50ms at 16 MHz
    }
}

void delay(volatile uint32_t count) {
    while(count--) {
        __asm__("nop");
    }
}

void USART_2_Init(void)
{
    //1. enable clock access to UART
    RCC->APB1ENR |= RCC_APB1ENR_USART_2EN;

    //2. enable clock access to GPIOA
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    //3. configure type of alternate function
    //GPIOA->AFR[0]  &= ~(GPIOA_AFR_TYPE);                     // Clear AFRL for PA2 and PA3
    GPIOA->AFR[0]  |= (GPIOA_AFR_VALUE);                     // Set AF7 for USART2

    //4. enable pins for alternate function, PA2 and PA3
    //GPIOA->MODER   &= ~(GPIOA_MODER_ALTERNATE);              // Clear mode bits for PA2 and PA3
    GPIOA->MODER   |= (GPIOA_MODER_PIN2 | GPIOA_MODER_PIN3); // Set mode to Alternate Function

    //5. Configure UART mode
    USART_2->BRR = 0x1A1;                      // Set baud rate to 115200
    USART_2->CR1 |= USART_CR1_TE | USART_CR1_RE;              // Enable transmitter and receiver
    // No parity / flow control adjustments needed
    USART_2->CR2 |= 0;                                        // 1 stop bit, no-op
    USART_2->CR3 |= 0;                                        // No flow control, no-op
    USART_2->CR1 |= USART_CR1_UE;                             // Enable USART_2

}