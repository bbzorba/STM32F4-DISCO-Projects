#include "stm32f4xx.h"

// Function prototypes
void init(void);
void delay(volatile uint32_t count);

//main function
int main(void) {
    init();
    while (1) {
        GPIOD->ODR ^= (1 << 12); // Toggle LED
        delay(1000000);
        GPIOD->ODR ^= (1 << 13); // Toggle LEDs
        delay(1000000);
        GPIOD->ODR ^= (1 << 14); // Toggle LEDs
        delay(1000000);
        GPIOD->ODR ^= (1 << 15); // Toggle LEDs
        delay(1000000);
    }
}

// Initialization function
void init(void) {
    // Enable GPIOD clock
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
    // Set PD12, PD13, PD14, PD15 as output
    GPIOD->MODER |= (1 << (12*2)) | (1 << (13*2)) | (1 << (14*2)) | (1 << (15*2));
    GPIOD->OTYPER &= ~((1 << 12) | (1 << 13) | (1 << 14) | (1 << 15));
    GPIOD->OSPEEDR |= (3 << (12*2)) | (3 << (13*2)) | (3 << (14*2)) | (3 << (15*2));
    GPIOD->PUPDR &= ~((3 << (12*2)) | (3 << (13*2)) | (3 << (14*2)) | (3 << (15*2)));
}

void delay(volatile uint32_t count) {
    while(count--) {
        __asm__("nop");
    }
}
