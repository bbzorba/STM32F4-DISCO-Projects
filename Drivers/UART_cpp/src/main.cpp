#include "../inc/uart.h"

char buffer[64];

// Function prototypes
void delay(volatile uint32_t count);

int main(void) {
    USART usart(USART_2, RX_AND_TX, __115200);

    usart.USART_WriteString("Welcome to Echo Mode!\r\n");
    usart.USART_WriteString("Connected on: ");
    usart.USART_WriteString(usart.GetPortName());
    usart.USART_WriteString("\r\nType something and press Enter to echo:\r\n");

    while (1) {
        usart.USART_WriteString("> "); // Prompt
        usart.USART_ReadString(buffer, sizeof(buffer));
        if (buffer[0] != '\0') {
            usart.USART_WriteString( "Echo: ");
            usart.USART_WriteString(buffer);
            usart.USART_WriteString("\r\n");
        }
    }
}

void delay(volatile uint32_t count) {
    while(count--) {
        __asm__("nop");
    }
}