#include "../inc/systick.h"

GPIO_InitTypeDef GPIO_InitStruct;

//main function
int main(void) {
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT; // Set as output
    GPIO_InitStruct.Pin = GPIO_PIN_15; // Pin 15 (PD15)
    GPIO_InitStruct.Pull = GPIO_NOPULL; // No pull-up or pull-down
    GPIO_InitStruct.Speed = GPIO_SPEED_MEDIUM;   // Medium speed
    
    GPIO BLUE_LED(GPIO_D, &GPIO_InitStruct);

    SYSTICK SysTick_Instance(SysTick, SYSTICK_OK);

    SysTick_Instance.delay(1); // Delay for 1 second
    
    while (1) {
        BLUE_LED.GPIO_TogglePin(GPIO_PIN_15); // Toggle PD15
        SysTick_Instance.delay_ms(500); // Delay for 500 ms
    }
}
