#include "hc06.h"
#include <string.h>

// Function prototypes
void delay(volatile uint32_t count);


//main function
int main(void) {
    // Initialize USART for both RX and TX at 9600 baud
    HC06 hc06(USART_3, RX_AND_TX, __9600);

    // Send a greeting so you can verify TX path and pairing
    static char hello[] = "Hello HC-06 (echo mode)\r\nConnected on ";
    hc06.SendData((const uint8_t*)hello, strlen(hello));
    const char *port = hc06.GetPortName();
    hc06.SendData((const uint8_t*)port, strlen(port));
    hc06.SendData((const uint8_t*)"\r\n", 2);

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
