#include "hc06.h"
#include <stdio.h>

// Function prototypes
void delay(volatile uint32_t count);


//main function
int main(void) {
    // Initialize USART2 for both RX and TX at 9600 baud
    HC06 hc06;

    HC06_Init(&hc06, UART_4, RX_AND_TX, __9600);

    // Send a greeting including the active USART port name
    char msg[96];
    int n = snprintf(msg, sizeof(msg), "Hello HC-06 (echo mode)\r\nConnected on %s\r\n", HC06_GetPortName(&hc06));
    if (n > 0) {
        if (n > (int)sizeof(msg)) n = (int)sizeof(msg);
    HC06_SendData(&hc06, (const uint8_t*)msg, (size_t)n);
    }

    // Echo loop: anything received will be sent back with CRLF
    while (1) {
        uint8_t c;
        HC06_ReceiveData(&hc06, &c, 1);
        HC06_SendData(&hc06, &c, 1);
        const uint8_t crlf[2] = {'\r', '\n'};
        HC06_SendData(&hc06, crlf, 2);
    }
}

void delay(volatile uint32_t count) {
    while(count--) {
        __asm__("nop");
    }
}
