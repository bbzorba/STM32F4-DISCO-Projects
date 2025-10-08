#include "uart.h"

// Function prototypes
void delay(volatile uint32_t count);

//main function
int main(void) {
    USART2_Init();

    test_setup();
    
    while (1) {

    }
}

void delay(volatile uint32_t count) {
    while(count--) {
        __asm__("nop");
    }
}
