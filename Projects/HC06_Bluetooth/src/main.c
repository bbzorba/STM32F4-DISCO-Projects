#include "hc06.h"

// Function prototypes
void delay(volatile uint32_t count);


//main function
int main(void) {
    // Initialize USART2 for both RX and TX at 9600 baud
    USART_2_Init(RX_AND_TX, __9600);

    // Send a greeting so you can verify TX path and pairing
    static const uint8_t hello[] = "Hello HC-06 (echo mode)\r\n";
    HC06_SendData((uint8_t*)hello, sizeof(hello) - 1);

    // Echo loop: anything received will be sent back with CRLF
    for (;;) {
        uint8_t c = (uint8_t)USART_2_Read();
        HC06_SendData(&c, 1);
        uint8_t crlf[2] = {'\r', '\n'};
        HC06_SendData(crlf, 2);
    }
}

void delay(volatile uint32_t count) {
    while(count--) {
        __asm__("nop");
    }
}
