#include "stm32f4xx.h"
#include "hc06.h"

// Function prototypes
void delay(volatile uint32_t count);


//main function
int main(void) {
    USART_2_Init(TX_ONLY, __9600);

    while (1) {
        static const uint8_t msg[] = "Hello HC-06\r\n"; // ASCII with CRLF
        HC06_SendData((uint8_t*)msg, sizeof(msg) - 1);   // exclude NUL terminator
        delay(16000000); // ~1s at 16 MHz
    }
}

void delay(volatile uint32_t count) {
    while(count--) {
        __asm__("nop");
    }
}
