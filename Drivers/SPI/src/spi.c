#include "spi.h"

static void short_delay(volatile int n) { while (n--) { __asm__("nop"); } }
