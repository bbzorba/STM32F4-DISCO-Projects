#include "uart.h"

// Function prototypes
void delay(volatile uint32_t count);

//main function
int main(void) {
    USART_x_Init(USART_2, TX_ONLY, __9600);

    
    while (1) {
        USART_x_Write(USART_2, 'H');
        USART_x_Write(USART_2, 'e');
        USART_x_Write(USART_2, 'l');
        USART_x_Write(USART_2, 'l');
        USART_x_Write(USART_2, 'o');
        USART_x_Write(USART_2, '\n');
        delay(1000000); // ~50ms at 16 MHz
    }
}

void delay(volatile uint32_t count) {
    while(count--) {
        __asm__("nop");
    }
}