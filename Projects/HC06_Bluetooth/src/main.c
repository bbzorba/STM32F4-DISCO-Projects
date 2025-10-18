#include "hc06.h"

// Function prototypes
void delay(volatile uint32_t count);


//main function
int main(void) {
    // Initialize USART2 for both RX and TX at 9600 baud
    HC06 hc06;

    HC06_Init(&hc06, RX_AND_TX, __9600);

    // Send a greeting so you can verify TX path and pairing
    static const char hello[] = "Hello HC-06 (echo mode)\r\n";
    HC06_SendData(&hc06, (uint16_t*)hello, sizeof(hello) - 1);

    // Echo loop: anything received will be sent back with CRLF
    for (;;) {
        uint16_t c = (uint16_t)USART_2_Read();
        HC06_SendData(&hc06, &c, 1);
        uint16_t crlf[2] = {'\r', '\n'};
        HC06_SendData(&hc06, crlf, 2);
    }
}

void delay(volatile uint32_t count) {
    while(count--) {
        __asm__("nop");
    }
}
