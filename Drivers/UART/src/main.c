#include "uart.h"

// Function prototypes
void delay(volatile uint32_t count);

//main function
int main(void) {
    USART_2_Init(TX_ONLY, __115200);

    
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