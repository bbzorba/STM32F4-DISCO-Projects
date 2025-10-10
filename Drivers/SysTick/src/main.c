#include "systick.h"
#include "../../GPIO/inc/gpio.h"

// Function prototypes
void delay(volatile uint32_t count);

GPIO_InitTypeDef GPIO_InitStruct;

//main function
int main(void) {
    __LIB_RCC_GPIOA_CLK_ENABLE(); // Enable clock for GPIOA
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT; // Set as output
    GPIO_InitStruct.Pin = GPIO_PIN_5; // Pin 5 (PA5)
    GPIO_InitStruct.Pull = GPIO_NOPULL; // No pull-up or pull-down

    LIB_GPIO_Init(GPIO_A, &GPIO_InitStruct); // Initialize GPIOA with the settings

    SysTick->LOAD = 16000000 - 1; // Load the SysTick timer for 1ms
    SysTick->VAL = 0; // Clear the current value
    SysTick->CTRL = 5; // Enable SysTick with processor clock
    
    while (1) {
        if (SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) {
            LIB_GPIO_TogglePin(GPIO_A, GPIO_PIN_5); // Toggle PA5
        }
    }
}

void delay(volatile uint32_t count) {
    while(count--) {
        __asm__("nop");
    }
}