#include "stm32f4xx.h"

extern int main(void);
void Reset_Handler(void);
void Default_Handler(void);

/* USART/UART IRQ handlers defined in uart.c — declared weak so the linker
   falls back to Default_Handler if uart.c is not linked. */
void USART1_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void USART2_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void USART3_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void UART4_IRQHandler(void)  __attribute__((weak, alias("Default_Handler")));
void UART5_IRQHandler(void)  __attribute__((weak, alias("Default_Handler")));
void USART6_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));

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
    Default_Handler,            // SysTick
    /* --- External (peripheral) interrupts: IRQ0 - IRQ71 --- */
    Default_Handler,            // IRQ  0: WWDG
    Default_Handler,            // IRQ  1: PVD
    Default_Handler,            // IRQ  2: TAMP_STAMP
    Default_Handler,            // IRQ  3: RTC_WKUP
    Default_Handler,            // IRQ  4: FLASH
    Default_Handler,            // IRQ  5: RCC
    Default_Handler,            // IRQ  6: EXTI0
    Default_Handler,            // IRQ  7: EXTI1
    Default_Handler,            // IRQ  8: EXTI2
    Default_Handler,            // IRQ  9: EXTI3
    Default_Handler,            // IRQ 10: EXTI4
    Default_Handler,            // IRQ 11: DMA1_Stream0
    Default_Handler,            // IRQ 12: DMA1_Stream1
    Default_Handler,            // IRQ 13: DMA1_Stream2
    Default_Handler,            // IRQ 14: DMA1_Stream3
    Default_Handler,            // IRQ 15: DMA1_Stream4
    Default_Handler,            // IRQ 16: DMA1_Stream5
    Default_Handler,            // IRQ 17: DMA1_Stream6
    Default_Handler,            // IRQ 18: ADC
    Default_Handler,            // IRQ 19: CAN1_TX
    Default_Handler,            // IRQ 20: CAN1_RX0
    Default_Handler,            // IRQ 21: CAN1_RX1
    Default_Handler,            // IRQ 22: CAN1_SCE
    Default_Handler,            // IRQ 23: EXTI9_5
    Default_Handler,            // IRQ 24: TIM1_BRK_TIM9
    Default_Handler,            // IRQ 25: TIM1_UP_TIM10
    Default_Handler,            // IRQ 26: TIM1_TRG_COM_TIM11
    Default_Handler,            // IRQ 27: TIM1_CC
    Default_Handler,            // IRQ 28: TIM2
    Default_Handler,            // IRQ 29: TIM3
    Default_Handler,            // IRQ 30: TIM4
    Default_Handler,            // IRQ 31: I2C1_EV
    Default_Handler,            // IRQ 32: I2C1_ER
    Default_Handler,            // IRQ 33: I2C2_EV
    Default_Handler,            // IRQ 34: I2C2_ER
    Default_Handler,            // IRQ 35: SPI1
    Default_Handler,            // IRQ 36: SPI2
    USART1_IRQHandler,          // IRQ 37: USART1
    USART2_IRQHandler,          // IRQ 38: USART2
    USART3_IRQHandler,          // IRQ 39: USART3
    Default_Handler,            // IRQ 40: EXTI15_10
    Default_Handler,            // IRQ 41: RTC_Alarm
    Default_Handler,            // IRQ 42: OTG_FS_WKUP
    Default_Handler,            // IRQ 43: TIM8_BRK_TIM12
    Default_Handler,            // IRQ 44: TIM8_UP_TIM13
    Default_Handler,            // IRQ 45: TIM8_TRG_COM_TIM14
    Default_Handler,            // IRQ 46: TIM8_CC
    Default_Handler,            // IRQ 47: DMA1_Stream7
    Default_Handler,            // IRQ 48: FSMC
    Default_Handler,            // IRQ 49: SDIO
    Default_Handler,            // IRQ 50: TIM5
    Default_Handler,            // IRQ 51: SPI3
    UART4_IRQHandler,           // IRQ 52: UART4
    UART5_IRQHandler,           // IRQ 53: UART5
    Default_Handler,            // IRQ 54: TIM6_DAC
    Default_Handler,            // IRQ 55: TIM7
    Default_Handler,            // IRQ 56: DMA2_Stream0
    Default_Handler,            // IRQ 57: DMA2_Stream1
    Default_Handler,            // IRQ 58: DMA2_Stream2
    Default_Handler,            // IRQ 59: DMA2_Stream3
    Default_Handler,            // IRQ 60: DMA2_Stream4
    Default_Handler,            // IRQ 61: ETH
    Default_Handler,            // IRQ 62: ETH_WKUP
    Default_Handler,            // IRQ 63: CAN2_TX
    Default_Handler,            // IRQ 64: CAN2_RX0
    Default_Handler,            // IRQ 65: CAN2_RX1
    Default_Handler,            // IRQ 66: CAN2_SCE
    Default_Handler,            // IRQ 67: OTG_FS
    Default_Handler,            // IRQ 68: DMA2_Stream5
    Default_Handler,            // IRQ 69: DMA2_Stream6
    Default_Handler,            // IRQ 70: DMA2_Stream7
    USART6_IRQHandler,          // IRQ 71: USART6
};

void Reset_Handler(void) {
    main();
    while (1);
}

void Default_Handler(void) {
    while (1);
}