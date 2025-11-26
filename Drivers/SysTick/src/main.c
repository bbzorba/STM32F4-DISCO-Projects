#include "../inc/systick.h"

GPIO_InitTypeDef GPIO_InitStruct;
GPIO_HandleTypeDef BLUE_LED;

//main function
int main(void) {
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT; // Set as output
    GPIO_InitStruct.Pin = GPIO_PIN_15; // Pin 15 (PD15)
    GPIO_InitStruct.Pull = GPIO_NOPULL; // No pull-up or pull-down
    GPIO_InitStruct.Speed = GPIO_SPEED_MEDIUM;   // Medium speed
    
    GPIO_constructor(&BLUE_LED, GPIO_D, &GPIO_InitStruct);

    SysTick_HandleTypeDef SysTickHandle;
    SysTick_constructor(&SysTickHandle, SysTick, SYSTICK_OK);

    SysTick_delay(&SysTickHandle, 1); // Delay for 1 second
    
    while (1) {
        GPIO_TogglePin(&BLUE_LED, GPIO_PIN_15); // Toggle PD15
        SysTick_delay_ms(&SysTickHandle, 500); // Delay for 500 ms
    }
}
