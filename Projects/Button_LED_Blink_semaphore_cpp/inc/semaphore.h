#ifndef MUTEX_H
#define MUTEX_H

#include <stdint.h>

/* ---- Mutex (mutual exclusion for LED register access) ---- */
extern volatile uint8_t g_mutex;
int  mutex_try_acquire(void);

void mutex_acquire(void);
void mutex_release(void);

/* ---- Binary semaphore (ISR -> task event signaling) ----
 * Give is idempotent (capped at 1), so button bounce does not accumulate
 * additional pending events.
 */
extern volatile uint8_t g_semaphore;
void semaphore_give(void);  /* ISR side */
int  semaphore_take(void);  /* task side: returns 1 if event consumed */

#endif // MUTEX_H
