#include "stm32f4xx.h"

extern int main(void);
void Reset_Handler(void);
void Default_Handler(void);

__attribute__((section(".isr_vector")))
void (* const vector_table[])(void) = {
    (void (*)(void))0x20020000, // Initial stack pointer (128KB SRAM)
    Reset_Handler,              // Reset handler
    Default_Handler,            // NMI
    Default_Handler,            // HardFault
    Default_Handler,            // MemManage
    Default_Handler,            // BusFault
    Default_Handler,            // UsageFault
    0, 0, 0, 0,                 // Reserved
    Default_Handler,            // SVCall
    Default_Handler,            // DebugMonitor
    0,                          // Reserved
    Default_Handler,            // PendSV
    Default_Handler             // SysTick
    // ... (add more if needed)
};

void Reset_Handler(void) {
    // Early force CS (PE3) low before any peripheral init to prefer SPI mode
    // Enable GPIOE clock
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
    // Configure PE3 as general purpose output, low speed is fine
    GPIOE->MODER &= ~(3U << (3U * 2U));
    GPIOE->MODER |=  (1U << (3U * 2U));
    // Drive low on ODR (active-low CS)
    GPIOE->ODR &= ~(1U << 3U);

    main();
    while (1);
}

void Default_Handler(void) {
    while (1);
}