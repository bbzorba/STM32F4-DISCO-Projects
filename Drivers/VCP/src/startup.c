#include "stm32f4xx.h"

extern int main(void);
void Reset_Handler(void);
void Default_Handler(void);
void SysTick_Handler(void);
void OTG_FS_IRQHandler(void);

// Minimal vector table with core exceptions and USB FS IRQ entry.
// Other entries default to 0 (unused).
__attribute__((section(".isr_vector")))
void (* const vector_table[])(void) = {
    (void (*)(void))0x20020000, // initial stack (128KB RAM end)
    Reset_Handler,              // Reset
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
    SysTick_Handler,            // SysTick
    // Peripheral IRQs start at index 16. Place OTG_FS at 16 + 67.
    [16 + 67] = OTG_FS_IRQHandler,
};

void Reset_Handler(void) {
    extern unsigned long __data_start__;
    extern unsigned long __etext;
    extern unsigned long __data_end__;
    extern unsigned long __bss_start__;
    extern unsigned long __bss_end__;

    unsigned long *src = &__etext;
    unsigned long *dst = &__data_start__;
    while (dst < &__data_end__) { *dst++ = *src++; }
    for (dst = &__bss_start__; dst < &__bss_end__; ) { *dst++ = 0; }

    extern void SystemInit(void);
    SystemInit();
    main();
    while (1) {}
}

void Default_Handler(void) {
    while (1) { __asm__("nop"); }
}

// HAL timebase support
extern void HAL_IncTick(void);
extern void HAL_SYSTICK_IRQHandler(void);
void SysTick_Handler(void) {
    HAL_IncTick();
    HAL_SYSTICK_IRQHandler();
}

// Forward declaration; the actual handler is implemented in VCP code and calls HAL_PCD_IRQHandler
void OTG_FS_IRQHandler(void);
