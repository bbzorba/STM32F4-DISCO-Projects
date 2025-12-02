#include "../inc/hc06.h"
#include <stdio.h>

// Function prototypes
void delay(volatile uint32_t count);

//main function
int main(void) {
    // Initialize USART2 for both RX and TX at 9600 baud
    HC06 hc06(USART_3, RX_AND_TX, __9600);

    // Send a greeting including the active USART port name
    char msg[96];
    int n = snprintf(msg, sizeof(msg), "Hello HC-06 (echo mode)\r\nConnected on %s\r\n", hc06.HC06_GetPortName());
    if (n > 0) {
        size_t to_send = (size_t)((n < (int)sizeof(msg)) ? n : (int)sizeof(msg));
        hc06.HC06_SendData((const uint8_t*)msg, to_send);
    }

    // Echo loop: anything received will be sent back with CRLF
    while (1) {
        uint8_t c;
        hc06.HC06_ReceiveData(&c, 1);
        hc06.HC06_SendData(&c, 1);
        const uint8_t crlf[2] = {'\r', '\n'};
        hc06.HC06_SendData(crlf, 2);
    }
}

void delay(volatile uint32_t count) {
    while(count--) {
        __asm__("nop");
    }
}
