#include "gpio.h"

void GPIO_Init(void) {
    RCC->RCC_AHB1ENR |= (1 << 3); // Enable GPIOD clock (bit 3 in AHB1ENR)
    GPIOD->MODER |= MODER_5_OUT; // Set PD15 to output mode
}

void GPIO_TogglePin(void) {
    GPIOD->ODR ^= LED_PIN; // Toggle PD15
}

void delay(volatile int count) {
    for (volatile int i = 0; i < count; i++);
}