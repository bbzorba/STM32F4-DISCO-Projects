#include "uart.h"

// Function prototypes
void delay(volatile uint32_t count);

//main function
int main(void) {

    UARTCom uart(TX_ONLY, __115200);

    printf("UART Communication Type: %d\n", uart.getComType());
    printf("UART Baud Rate: %d\n", uart.getBaudRate());

    while (1) {
        uart.USART_2_Write('H');
        uart.USART_2_Write('e');
        uart.USART_2_Write('l');
        uart.USART_2_Write('l');
        uart.USART_2_Write('o');
        uart.USART_2_Write('\n');
        delay(1000000); // ~50ms at 16 MHz
    }
}

void delay(volatile uint32_t count) {
    while(count--) {
        __asm__("nop");
    }
}