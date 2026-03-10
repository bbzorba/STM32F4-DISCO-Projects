#ifndef BUTTON_LED_H
#define BUTTON_LED_H

/* All base types (LED_Type, LEDColor_Type, LEDState_Type, pin macros, etc.)
 * come from the LED_Blink library header -- no redefinitions needed here.  */
#include "../../Projects/LED_Blink/inc/led.h"
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

#define SYSCFG  ((SYSCFG_TypeDef *)SYSCFG_BASE)
#define EXTI    ((EXTI_TypeDef  *)EXTI_BASE)

/* Wraps LED_constructor (from LED_Blink lib) and initialises USER button on first call. */
void Button_LED_constructor(LED_Type * const led, LEDColor_Type _color, LEDState_Type _state);

/* Public blink API -- pass a NULL-terminated array of LED pointers. */
void blink_LEDS_sync(USART_HandleType *usart, LED_Type * const leds[]);
void blink_LEDS_async(USART_HandleType *usart, LED_Type * const leds[]);

/* Initialise PA0 (USER button on STM32F4-Discovery) as EXTI0 rising-edge interrupt. */
void Button_Init(void);

#endif // BUTTON_LED_H