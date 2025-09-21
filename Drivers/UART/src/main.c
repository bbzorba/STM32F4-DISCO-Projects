#include "stm32f4xx.h"

// Function prototypes
void init(void);
void delay(volatile uint32_t count);

//main function
int main(void) {
    init();
    while (1) {

    }
}

// Initialization function
void init(void) {
    // Enable GPIOD clock
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    
}

void delay(volatile uint32_t count) {
    while(count--) {
        __asm__("nop");
    }
}
