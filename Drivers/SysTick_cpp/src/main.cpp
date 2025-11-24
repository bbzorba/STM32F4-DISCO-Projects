#include "../inc/systick.h"
#include "../../GPIO_cpp/inc/gpio.h"

SYSTICK systick_timer; // Create an instance of the SYSTICK class

GPIO_InitTypeDef GPIO_InitStruct;
GPIO GPIO_D_PORT(GPIO_D, &GPIO_InitStruct);

//main function
int main(void) {
    // Configure PD15 as output (push-pull by default when bit4 of Mode=0)
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Pin = GPIO_PIN_15;
    GPIO_InitStruct.Pull = GPIO_NOPULL;

    // C++ GPIO constructor expects: (port enum, GPIO port pointer, init params)
    GPIO GPIO_D_PORT(GPIO_D, &GPIO_InitStruct);

    systick_timer.SysTick_delay(1); // Delay for 1 second
    
    while (1) {
        systick_timer.SysTick_delay_ms(500); // Delay for 500 ms
        GPIO_D_PORT.GPIO_TogglePin(GPIO_PIN_15); // Toggle PD15
        
    }
}
