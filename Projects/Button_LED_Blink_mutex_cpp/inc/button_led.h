/* Initialises the GPIO pin and EXTI interrupt for the given port/pin.
 * pin_mask: one of GPIO_PIN_0 … GPIO_PIN_15
 * port    : GPIO_A … GPIO_I
 * nvic_priority: 0 (highest) … 15 (lowest) */

#ifndef BUTTON_LED_H
#define BUTTON_LED_H

/* All base types (LED_Type, LEDColor_Type, LEDState_Type, pin macros, etc.)
 * come from the LED_Blink library header -- no redefinitions needed here.  */
#include "../../Projects/LED_Blink_cpp/inc/led.h"
#include "../inc/mutex.h"
#include <stdio.h>

/* -----------------------------------------------------------------------
 * Minimal SYSCFG / EXTI register definitions (not in gpio.h / uart.h).
 * Base addresses from STM32F4 reference manual.
 * ----------------------------------------------------------------------- */
#define SYSCFG_BASE             (APB2PERIPH_BASE + 0x3800U)
#define EXTI_BASE               (APB2PERIPH_BASE + 0x3C00U)
#define RCC_APB2ENR_SYSCFGEN    (1U << 14)

typedef struct {
    volatile uint32_t MEMRMP;
    volatile uint32_t PMC;
    volatile uint32_t EXTICR[4];
    uint32_t          RESERVED[2];
    volatile uint32_t CMPCR;
} SYSCFG_TypeDef;

typedef struct {
    volatile uint32_t IMR;
    volatile uint32_t EMR;
    volatile uint32_t RTSR;
    volatile uint32_t FTSR;
    volatile uint32_t SWIER;
    volatile uint32_t PR;
} EXTI_TypeDef;

/* Button object: wraps a GPIO handle plus the EXTI configuration
 * computed at construction time so Button_Init is fully generic.   */
class Button {
private:
    GPIO_InitTypeDef   gpio_init;  /* Init struct storage; gpio.init → &this */
    uint8_t            exti_line;  /* 0-15, derived from pin mask          */
    uint8_t            port_code;  /* 0=A, 1=B, 2=C, 3=D …, from port addr */
    uint8_t            nvic_priority;
public:
    Button(GPIO_ManualTypeDef *port, 
           uint32_t pin_mask,
           uint32_t mode, 
           uint32_t pull, 
           uint32_t speed,
           uint8_t nvic_priority);
    void Button_Init(void);
};

#define SYSCFG  ((SYSCFG_TypeDef *)SYSCFG_BASE)
#define EXTI    ((EXTI_TypeDef  *)EXTI_BASE)

/* -----------------------------------------------------------------------
 * IRQ numbers for EXTI lines on STM32F407 and CMSIS NVIC helpers.
 * core_cm4.h provides NVIC_SetPriority / NVIC_EnableIRQ as static
 * inlines, but requires IRQn_Type and __NVIC_PRIO_BITS to be defined
 * first.  We supply the minimal set needed by this driver.
 * ----------------------------------------------------------------------- */
#ifndef __IRQn_Type_DEFINED
#define __IRQn_Type_DEFINED
#ifndef __NVIC_PRIO_BITS
#define __NVIC_PRIO_BITS  4
#endif
typedef enum {
    NonMaskableInt_IRQn   = -14,
    MemoryManagement_IRQn = -12,
    BusFault_IRQn         = -11,
    UsageFault_IRQn       = -10,
    SVCall_IRQn           = -5,
    DebugMonitor_IRQn     = -4,
    PendSV_IRQn           = -2,
    SysTick_IRQn          = -1,
    EXTI0_IRQn            = 6,
    EXTI1_IRQn            = 7,
    EXTI2_IRQn            = 8,
    EXTI3_IRQn            = 9,
    EXTI4_IRQn            = 10,
    EXTI9_5_IRQn          = 23,
    EXTI15_10_IRQn        = 40,
} IRQn_Type;
#endif /* __IRQn_Type_DEFINED */
#include "../../../Drivers/CMSIS/core_cm4.h"

/* Public blink API -- pass a NULL-terminated array of LED pointers. */
void blink_LEDS_async(USART usart, LED* const leds[]);
void blink_LEDS_sync(USART usart, LED* const leds[]);

#endif // BUTTON_LED_H