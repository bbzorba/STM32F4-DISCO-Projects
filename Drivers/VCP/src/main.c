#include "stm32f4xx.h"
#include "vcp.h"
#include "uart.h"

static void delay(volatile uint32_t d){ while(d--) __asm__("nop"); }

int main(void){
    // Enable GPIOD for LEDs
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
    GPIOD->MODER |= (1<<(12*2));

    VCP_Init();
    USART2_Init(); // Use ST-LINK VCP (USART2 on PA2/PA3)
    printf("USART2 ready via ST-LINK VCP\r\n");

    while(1){
        GPIOD->ODR ^= (1<<12);
        VCP_Task();
        printf("Hello over USART2 (ST-LINK VCP)\r\n");
        delay(168000000/4); // ~0.5s at 168MHz
    }
}
