#include "hc06.h"

// Function prototypes
void delay(volatile uint32_t count);


//main function
int main(void) {
    // Initialize USART2 for both RX and TX at 9600 baud
    HC06 hc06(RX_AND_TX, __9600);

    // Send a greeting so you can verify TX path and pairing
    static const uint8_t hello[] = "Hello HC-06 (echo mode)\r\n";
    hc06.SendData(hello, sizeof(hello)-1);

    // Echo loop: anything received will be sent back with CRLF
    while (1) {
    uint8_t c = (uint8_t)hc06.ReceiveData();
    hc06.SendData(&c, 1);
    const uint8_t crlf[2] = {'\r', '\n'};
    hc06.SendData(crlf, 2);
    }
}

void delay(volatile uint32_t count) {
    while(count--) {
        __asm__("nop");
    }
}
