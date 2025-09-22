#include "stm32f4xx.h"
#include "vcp.h"

static void delay(volatile uint32_t d){ while(d--) __asm__("nop"); }

int main(void){
    // Enable GPIOD for LEDs
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
    GPIOD->MODER |= (1<<(12*2));

    VCP_Init();

    while(1){
        GPIOD->ODR ^= (1<<12);
        delay(800000);
        VCP_Task();
    }
}
