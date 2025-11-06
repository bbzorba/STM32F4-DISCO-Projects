#include "uart.h"

char buffer[64];

// Function prototypes
void delay(volatile uint32_t count);

//main function
int main(void) {

    USART usart2(USART_2, RX_AND_TX, __115200);

    printf("USART Communication Type: %d\n", usart2.getComType());
    printf("USART Baud Rate: %d\n", usart2.getBaudRate());

    usart2.writeString("Welcome to Echo Mode!\n");
    
    while (1) {
        usart2.readString(buffer, 64);
        usart2.writeString(buffer);
        usart2.writeString("\n");
    }
}

void delay(volatile uint32_t count) {
    while(count--) {
        __asm__("nop");
    }
}