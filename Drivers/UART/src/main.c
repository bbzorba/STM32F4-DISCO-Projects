#include "../inc/uart.h"

char buffer[64];

// Function prototypes
void delay(volatile uint32_t count);

int main(void) {
    USART_HandleType usart;
    USART_constructor(&usart, USART_2, RX_AND_TX, __115200);

    USART_WriteString(&usart, "Welcome to Echo Mode!\r\n");
    USART_WriteString(&usart, "Connected on: ");
    USART_WriteString(&usart, GetPortName(&usart));
    USART_WriteString(&usart, "\r\nType something and press Enter to echo:\r\n");

    while (1) {
        USART_WriteString(&usart, "> "); // Prompt
        USART_ReadString(&usart, buffer, sizeof(buffer));
        if (buffer[0] != '\0') {
            USART_WriteString(&usart, "Echo: ");
            USART_WriteString(&usart, buffer);
            USART_WriteString(&usart, "\r\n");
        }
    }
}

void delay(volatile uint32_t count) {
    while(count--) {
        __asm__("nop");
    }
}