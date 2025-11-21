#include "gpio.h"
#define delay 200000

GPIO_InitTypeDef GPIO_InitStruct;

// Simple delay function
void delay_fn(volatile int count) {
    for (volatile int i = 0; i < count; i++);
}

int main(void) {
    GPIO_HandleTypeDef GPIO_LEDS;
    GPIO_Init(&GPIO_LEDS, GPIO_D, &GPIO_InitStruct);
    
    GPIO_D->MODER |= MODER_2_OUT | MODER_3_OUT | MODER_4_OUT | MODER_5_OUT; // Set PD12, PD13, PD14, PD15 to output mode

    while(1) {
        //GPIOD_TogglePins(delay);
        GPIO_TogglePin(GPIO_LEDS, GPIO_D, GPIO_PIN_15);
        delay_fn(delay);
        GPIO_TogglePin(GPIO_LEDS, GPIO_D, GPIO_PIN_14);
        delay_fn(delay);
        GPIO_TogglePin(GPIO_LEDS, GPIO_D, GPIO_PIN_13);
        delay_fn(delay);
        GPIO_TogglePin(GPIO_LEDS, GPIO_D, GPIO_PIN_12);
        delay_fn(delay);
    }
}

