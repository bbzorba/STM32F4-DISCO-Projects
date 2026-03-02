#include "../inc/uart.h"

char buffer[64];

// Function prototypes
void delay(volatile uint32_t count);

int main(void) {
    USART_HandleType usart;
    // Initialize USART2 for RX and TX at 115200
    USART_constructor(&usart, USART_2, RX_AND_TX, __115200);

    // POLLING: simple write using polling API
    USART_WriteString(&usart, "Polling test: Hello from polling UART!\r\n");

    // Now switch to interrupt-driven RX: define a callback that echoes received chars
    void uart_rx_cb(char c) {
        // echo received character back (safe to call from IRQ)
        USART_WriteChar(&usart, c);
    }

    USART_EnableRXInterrupt(&usart, uart_rx_cb);
    USART_WriteString(&usart, "Interrupt echo enabled. Type characters to see them echoed.\r\n");

    while (1) {
        // main loop can do other work; RX will echo via interrupt callback
        delay(1000000);
    }
}

void delay(volatile uint32_t count) {
    while(count--) {
        __asm__("nop");
    }
}