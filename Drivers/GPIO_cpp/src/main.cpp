#include "gpio.h"
#include <stdint.h>
#define delay 200000

GPIO_InitTypeDef GPIO_D_LEDS_InitStruct;
GPIO_InitTypeDef GPIO_A_Button_InitStruct;

// Simple delay function
void delay_fn(volatile int count) {
    for (volatile int i = 0; i < count; i++);
}

int main(void) {

    GPIO_D_LEDS_InitStruct = __GPIO_PIN_Params(GPIO_PIN_15 | GPIO_PIN_14 | GPIO_PIN_13 | GPIO_PIN_12, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, 0, 0);
    GPIO GPIO_D_LEDS(PORT_D,GPIOD, &GPIO_D_LEDS_InitStruct);

    GPIO_A_Button_InitStruct = __GPIO_PIN_Params(GPIO_PIN_0, GPIO_MODE_INPUT, GPIO_NOPULL, 0, 0);
    GPIO GPIO_A_Button(PORT_A,GPIOA, &GPIO_A_Button_InitStruct);

    uint8_t prevPressed = 0; // simple edge detection
    while(1) {
        uint8_t pressed = (GPIO_A_Button.LIB_GPIO_ReadPin(GPIO_PIN_0) == GPIO_PIN_SET);
        if (pressed && !prevPressed) {
            // On rising edge: perform blink sequence once per press
            GPIO_D_LEDS.LIB_GPIO_TogglePin(GPIO_PIN_15);
            delay_fn(delay);
            GPIO_D_LEDS.LIB_GPIO_TogglePin(GPIO_PIN_14);
            delay_fn(delay);
            GPIO_D_LEDS.LIB_GPIO_TogglePin(GPIO_PIN_13);
            delay_fn(delay);
            GPIO_D_LEDS.LIB_GPIO_TogglePin(GPIO_PIN_12);
            delay_fn(delay);
        }
        prevPressed = pressed;
    }
}