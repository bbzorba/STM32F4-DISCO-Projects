#include "../inc/button_led.h"
#include <stdio.h>

/* -----------------------------------------------------------------------
 * Mutex and async-request flag shared between main and EXTI0 ISR.
 * ----------------------------------------------------------------------- */
static volatile uint8_t g_mutex          = 0;  /* 0 = free, 1 = held       */
static volatile uint8_t g_async_requested = 0;  /* set by EXTI0 ISR         */

/* Non-blocking try: returns 1 if acquired, 0 if busy.
 * Uses a critical section so the read-modify-write is atomic.            */
static int mutex_try_acquire(void) {
    int acquired;
    __disable_irq();
    acquired = (g_mutex == 0);
    if (acquired) g_mutex = 1;
    __enable_irq();
    return acquired;
}

/* Blocking acquire: spins until the mutex is free.                       */
static void mutex_acquire(void) {
    while (!mutex_try_acquire()) {}
}

static void mutex_release(void) {
    __disable_irq();
    g_mutex = 0;
    __enable_irq();
}

/* Simple busy-wait delay (~500 ms at 16 MHz HSI default).               */
static void delay(volatile uint32_t count) {
    while (count--) __asm__("nop");
}

/* -----------------------------------------------------------------------
 * Button_LED_constructor
 *   Delegates GPIO/vtable setup to LED_constructor from the LED_Blink
 *   library, then initialises the USER button (PA0) on the first call.
 * ----------------------------------------------------------------------- */
void Button_LED_constructor(LED_Type* const led, LEDColor_Type _color, LEDState_Type _state) {
    LED_constructor(led, _color, _state);

    static uint8_t button_initialized = 0;
    if (!button_initialized) {
        Button_Init();
        button_initialized = 1;
    }
}

/* -----------------------------------------------------------------------
 * Button_Init
 *   Configures PA0 (USER button on STM32F4-Discovery) as a digital input
 *   with internal pull-down and routes it to EXTI line 0 (rising edge).
 *   EXTI0_IRQn is enabled in NVIC at priority 1 (below USART priority 0).
 * ----------------------------------------------------------------------- */
void Button_Init(void) {
    /* 1. Enable GPIOA clock and set PA0 as input with pull-down. */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    GPIO_A->MODER &= ~(3U << (0 * 2));   /* input mode (00) */
    GPIO_A->PUPDR &= ~(3U << (0 * 2));
    GPIO_A->PUPDR |=  (2U << (0 * 2));   /* pull-down */

    /* 2. Enable SYSCFG clock and route PA to EXTI0
     *    EXTICR[0] bits [3:0] = 0b0000 selects Port A for EXTI0. */
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    SYSCFG->EXTICR[0] &= ~(0x000FU);     /* clear EXTI0 source bits */
                                          /* 0000 = PA already the default */

    /* 3. Configure EXTI line 0: unmask, rising edge. */
    EXTI->IMR  |=  (1U << 0);   /* unmask line 0 */
    EXTI->RTSR |=  (1U << 0);   /* rising-edge trigger */
    EXTI->FTSR &= ~(1U << 0);   /* disable falling-edge */

    /* 4. Enable EXTI0 in NVIC at priority 1. */
    NVIC_SetPriority(EXTI0_IRQn, 1);
    NVIC_EnableIRQ(EXTI0_IRQn);
}

/* -----------------------------------------------------------------------
 * EXTI0_IRQHandler
 *   Fires on USER button rising edge (press). Sets g_async_requested so
 *   that the next call to blink_LEDS_async in main performs the fast blink.
 * ----------------------------------------------------------------------- */
void EXTI0_IRQHandler(void) {
    /* Clear the pending bit immediately to re-arm the interrupt. */
    EXTI->PR = (1U << 0);
    g_async_requested = 1;
}

/* -----------------------------------------------------------------------
 * blink_LEDS_sync
 *   Acquires the mutex (blocks until free), then toggles ALL LEDs at the
 *   same time every ~500 ms for 10 cycles, then releases the mutex.
 * ----------------------------------------------------------------------- */
void blink_LEDS_sync(USART_HandleType *usart, LED_Type * const leds[]) {
    mutex_acquire();
    USART_WriteString(usart, "-- Sync blink start --\r\n");

    /* 5 cycles (~2.5 s total) while the mutex is held continuously.
     * This gives the button time to be pressed mid-blink.            */
    for (int cycle = 0; cycle < 5; ++cycle) {
            char buffer[32];
            snprintf(buffer, sizeof(buffer), "-- cycle no: %d --\r\n\r\n", cycle + 1);
            USART_WriteString(usart, buffer);
        /* Toggle every LED in one pass so they all switch simultaneously. */
        for (int i = 0; leds[i] != NULL; ++i)
            LED_setState(leds[i], LED_TOGGLE);
        USART_WriteString(usart, (LED_PORT->ODR & LED_PIN_GREEN) ? "All LEDs ON\r\n" : "All LEDs OFF\r\n");
        delay(2000000);

        /* Mid-cycle async probe: async finds the mutex busy here and prints
         * "deferring". After sync releases, the main-loop call succeeds.  */
        if (g_async_requested)
            blink_LEDS_async(usart, leds);
    }

    USART_WriteString(usart, "-- Sync blink end --\r\n\r\n");
    mutex_release();
}

/* -----------------------------------------------------------------------
 * blink_LEDS_async
 *   Runs ONLY when the button has been pressed (g_async_requested flag set
 *   by EXTI0_IRQHandler). Tries a non-blocking mutex acquire so it never
 *   clobbers an ongoing sync blink that still holds the lock. If the lock
 *   is busy the request is simply deferred to the next call.
 *   Pattern: 3 rapid ON/OFF flashes (~100 ms each) to distinguish it from
 *   the slow sync blink.
 * ----------------------------------------------------------------------- */
void blink_LEDS_async(USART_HandleType *usart, LED_Type * const leds[]) {
    if (!g_async_requested)
        return;  /* nothing to do -- skip silently */

    if (!mutex_try_acquire()) {
        USART_WriteString(usart, "Async: mutex busy, deferring.\r\n\r\n");
        return;
    }

    USART_WriteString(usart, "-- Async blink (button pressed) --\r\n");

    /* Blink LEDs one after another (sequential), printing state for each.
     * This is the distinguishing behaviour from sync (all-at-once). */
    for (int cycle = 0; cycle < 5; ++cycle) { /* 5 ON/OFF cycles */
        /* Print cycle number at the start of each cycle. */
        char buffer[32];
        snprintf(buffer, sizeof(buffer), "-- cycle no: %d --\r\n\r\n", cycle + 1);
        USART_WriteString(usart, buffer);

        for (int i = 0; leds[i] != NULL; ++i) {
            LED_setState(leds[i], LED_ON);
            delay(200000);  /* ~100 ms ON */
            LED_setState(leds[i], LED_OFF);
            delay(100000);  /* ~50 ms gap before next LED */
        }
    }
    /* Clear request flag BEFORE releasing the mutex. */
    g_async_requested = 0;
    USART_WriteString(usart, "-- Async blink end --\r\n\r\n");
    mutex_release();
}