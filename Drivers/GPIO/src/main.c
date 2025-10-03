#include "gpio.h"
#define delay 200000

int main(void) {
    GPIOD_Init();

    while(1) {
        GPIOD_TogglePins(delay);
    }
}