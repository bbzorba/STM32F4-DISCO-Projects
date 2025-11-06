#include "uart.h"

char buffer[64];

// Function prototypes
void delay(volatile uint32_t count);

//main function
int main(void) {
    USART_x_Init(USART_1, RX_AND_TX, __115200);
    writeString(USART_1, "Welcome to Echo Mode!\n");
    
    while (1) {
        readString(USART_1, buffer, 64);
        writeString(USART_1, buffer);
        writeString(USART_1, "\n");
    }
}

void delay(volatile uint32_t count) {
    while(count--) {
        __asm__("nop");
    }
}