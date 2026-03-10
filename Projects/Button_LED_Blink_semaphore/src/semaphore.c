#include "../inc/semaphore.h"
#include "../inc/button_led.h"  /* provides __disable_irq() & __enable_irq() via CMSIS chain */

volatile uint8_t g_mutex     = 0;  /* 0 = free, 1 = held */
volatile uint8_t g_semaphore = 0;  /* binary semaphore: 0 = no pending press, 1 = press pending */

/* Non-blocking mutex try: returns 1 if acquired, 0 if busy. */
int mutex_try_acquire(void) {
    int acquired;
    __disable_irq();
    acquired = (g_mutex == 0);
    if (acquired) g_mutex = 1;
    __enable_irq();
    return acquired;
}

void mutex_acquire(void) {
    while (!mutex_try_acquire()) {}
}

void mutex_release(void) {
    __disable_irq();
    g_mutex = 0;
    __enable_irq();
}

/* Semaphore give: safe to call from ISR.
 * Capped at 1 (binary) so that bounce edges from a single press
 * are all idempotent -- exactly like the mutex-project flag.   */
void semaphore_give(void) {
    __disable_irq();
    if (g_semaphore == 0) g_semaphore = 1;
    __enable_irq();
}

/* Semaphore take: atomically consumes one count.  Returns 1 if an
 * event was pending, 0 if the semaphore was empty (nothing to do). */
int semaphore_take(void) {
    int taken;
    __disable_irq();
    taken = (g_semaphore > 0);
    if (taken) g_semaphore--;
    __enable_irq();
    return taken;
}
