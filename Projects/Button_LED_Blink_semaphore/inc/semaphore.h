#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <stdint.h>

/* ---- Mutex (mutual exclusion for LED register access) ---- */
extern volatile uint8_t g_mutex;
int  mutex_try_acquire(void);
void mutex_acquire(void);
void mutex_release(void);

/* ---- Binary semaphore (ISR → task event signaling) ----
 * Behaves like the mutex-project flag: give is idempotent (capped at 1)
 * so button bounce edges don't accumulate multiple counts.              */
extern volatile uint8_t g_semaphore;
void semaphore_give(void);  /* call from ISR: sets count to 1 (no-op if already 1) */
int  semaphore_take(void);  /* call from task: atomically clears and returns 1 if
                             * a press was pending, else 0                         */

#endif // SEMAPHORE_H
