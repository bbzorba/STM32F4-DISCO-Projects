#include "uart.h"

char buffer[64];

// Function prototypes
void delay(volatile uint32_t count);

int main(void) {
    USART_TypeDef usart;
    USART_Init(&usart, USART_1, RX_AND_TX, __115200);

    USART_WriteString(&usart, "Welcome to Echo Mode!\r\n");
    USART_WriteString(&usart, "Connected on: ");
    USART_WriteString(&usart, GetPortName(usart.regs));
    USART_WriteString(&usart, "\r\n");
    
    while (1) {
        USART_ReadString(&usart, buffer, sizeof(buffer));
        USART_WriteString(&usart, buffer);
        USART_WriteString(&usart, "\r\n");
    }
}

void delay(volatile uint32_t count) {
    while(count--) {
        __asm__("nop");
    }
}