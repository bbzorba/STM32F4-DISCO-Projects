#include "gpio.h"
#define delay 200000

GPIO_InitTypeDef GPIO_InitStruct;

// Simple delay function
void delay_fn(volatile int count) {
    for (volatile int i = 0; i < count; i++);
}

// Initialize LEDs on STM32F4-Discovery
void GPIOD_Init(void) {
    __LIB_RCC_GPIOD_CLK_ENABLE();
    GPIOD->MODER |= MODER_2_OUT | MODER_3_OUT | MODER_4_OUT | MODER_5_OUT; // Set PD12, PD13, PD14, PD15 to output mode
}

// Manual GPIO initialization using GPIO_InitTypeDef
void GPIOD_Init_Manual(void) {
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Pin = GPIO_PIN_15 | GPIO_PIN_14 | GPIO_PIN_13 | GPIO_PIN_12;
    GPIO_InitStruct.Pull = GPIO_NOPULL;

    __LIB_RCC_GPIOD_CLK_ENABLE();
    LIB_GPIO_Init(GPIOD, &GPIO_InitStruct);
}


int main(void) {
    //GPIOD_Init();
    GPIOD_Init_Manual();

    while(1) {
        //GPIOD_TogglePins(delay);
        LIB_GPIO_TogglePin(GPIOD, GPIO_PIN_15);
        delay_fn(delay);
        LIB_GPIO_TogglePin(GPIOD, GPIO_PIN_14);
        delay_fn(delay);
        LIB_GPIO_TogglePin(GPIOD, GPIO_PIN_13);
        delay_fn(delay);
        LIB_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
        delay_fn(delay);
    }
}

