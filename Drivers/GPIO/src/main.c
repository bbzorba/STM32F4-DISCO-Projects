#include "gpio.h"

int main(void) {
    GPIO_Init();

    while(1) {
        GPIO_TogglePin();
        delay(100000);
    }
}