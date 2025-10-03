#include "gpio.h"

#define MODER_2_OUT (1 << (12 * 2)) // Set PD12 to output mode (01)
#define MODER_3_OUT (1 << (13 * 2)) // Set PD13 to output mode (01)
#define MODER_4_OUT (1 << (14 * 2)) // Set PD14 to output mode (01)
#define MODER_5_OUT (1 << (15 * 2)) // Set PD15 to output mode (01)

#define BLUE_LED_PIN (1 << 15)           // Pin 15
#define GREEN_LED_PIN (1 << 14)          // Pin 14
#define RED_LED_PIN (1 << 13)            // Pin 13
#define YELLOW_LED_PIN (1 << 12)         // Pin 12

void GPIOD_Init(void) {
    RCC->RCC_AHB1ENR |= GPIOD_EN; // Enable GPIOD clock (bit 3 in AHB1ENR)
    GPIOD->MODER |= MODER_2_OUT | MODER_3_OUT | MODER_4_OUT | MODER_5_OUT; // Set PD12, PD13, PD14, PD15 to output mode
}

void delay(volatile int count) {
    for (volatile int i = 0; i < count; i++);
}

void GPIOD_TogglePins(volatile int count) {
    GPIOD->ODR ^= BLUE_LED_PIN; // Toggle PD15
    delay(count);
    GPIOD->ODR ^= GREEN_LED_PIN; // Toggle PD14
    delay(count);
    GPIOD->ODR ^= RED_LED_PIN; // Toggle PD13
    delay(count);
    GPIOD->ODR ^= YELLOW_LED_PIN; // Toggle PD12
    delay(count);
}

