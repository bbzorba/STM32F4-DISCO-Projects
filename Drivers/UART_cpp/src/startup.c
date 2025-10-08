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
    main();
    while (1);
}

void Default_Handler(void) {
    while (1);
}