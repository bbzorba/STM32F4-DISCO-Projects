// Minimal SystemInit to configure clocks for USB FS (48 MHz from PLLQ)
// Minimal SystemInit to configure clocks for USB FS (48 MHz from PLLQ)
#include "stm32f4xx.h"

// CMSIS SystemCoreClock variable required by HAL
uint32_t SystemCoreClock = 16000000UL;

void SystemInit(void) {
    // Enable FPU (if present)
    SCB->CPACR |= ((3UL << (10*2)) | (3UL << (11*2)));

    // Reset RCC configuration
    RCC->CR |= RCC_CR_HSION;
    RCC->CFGR = 0x00000000;
    RCC->CR &= ~(RCC_CR_HSEON | RCC_CR_PLLON);
    RCC->PLLCFGR = 0x24003010; // reset value
    RCC->CIR = 0x00000000;

    // Enable HSE (8 MHz on STM32F4-Discovery) and wait ready
    RCC->CR |= RCC_CR_HSEON;
    while ((RCC->CR & RCC_CR_HSERDY) == 0) {}

    // Configure flash latency and caches for 168 MHz
    FLASH->ACR = FLASH_ACR_ICEN | FLASH_ACR_DCEN | FLASH_ACR_PRFTEN | FLASH_ACR_LATENCY_5WS;

    // PLL configuration: VCO = (HSE / M) * N = (8/8)*336 = 336 MHz
    // SYSCLK = VCO / P = 336 / 2 = 168 MHz, USB = VCO / Q = 336 / 7 = 48 MHz
    uint32_t pll_m = 8;
    uint32_t pll_n = 336;
    uint32_t pll_p = 2; // encoded as (pll_p/2 -1) << 16
    uint32_t pll_q = 7;
    RCC->PLLCFGR = (pll_m) | (pll_n << 6) | (((pll_p/2)-1) << 16) | (RCC_PLLCFGR_PLLSRC_HSE) | (pll_q << 24);

    // Enable PLL and wait
    RCC->CR |= RCC_CR_PLLON;
    while ((RCC->CR & RCC_CR_PLLRDY) == 0) {}

    // Set prescalers: AHB=1, APB2=2, APB1=4
    RCC->CFGR |= RCC_CFGR_HPRE_DIV1 | RCC_CFGR_PPRE2_DIV2 | RCC_CFGR_PPRE1_DIV4;

    // Select PLL as system clock
    RCC->CFGR &= ~RCC_CFGR_SW;
    RCC->CFGR |= RCC_CFGR_SW_PLL;
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL) {}

    // Set vector table offset
    SCB->VTOR = FLASH_BASE;

    // Update SystemCoreClock
    SystemCoreClock = 168000000UL;
}
