#include "systick.h"

// Function prototypes
void delay(volatile uint32_t count);

GPIO_InitTypeDef GPIO_InitStruct;

//main function
int main(void) {
    __LIB_RCC_GPIOD_CLK_ENABLE(); // Enable clock for GPIOD
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT; // Set as output
    GPIO_InitStruct.Pin = GPIO_PIN_15; // Pin 15 (PD15)
    GPIO_InitStruct.Pull = GPIO_NOPULL; // No pull-up or pull-down

    LIB_GPIO_Init(GPIO_D, &GPIO_InitStruct); // Initialize GPIOD with the settings

    SysTick_delay(1); // Delay for 1 second
    
    while (1) {
        SysTick_delay_ms(500); // Delay for 500 ms
        LIB_GPIO_TogglePin(GPIO_D, GPIO_PIN_15); // Toggle PD15
        
    }
}

void delay(volatile uint32_t count) {
    while(count--) {
        __asm__("nop");
    }
}