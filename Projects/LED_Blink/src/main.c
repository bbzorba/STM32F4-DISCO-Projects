#include "stm32f4xx.h"
#include "led.h"

// Function prototypes
void delay(volatile uint32_t count);

LED_Type redLED;
LED_Type greenLED;
LED_Type yellowLED;
LED_Type blueLED;

//main function
int main(void) {
    USART2_Init();
    LED_constructor(&redLED, RED, LED_OFF);
    LED_constructor(&greenLED, GREEN, LED_OFF);
    LED_constructor(&yellowLED, YELLOW, LED_OFF);
    LED_constructor(&blueLED, BLUE, LED_OFF);

    LED_getState(&redLED); // Should print: "0 LED state is currently: 0"
    LED_getState(&greenLED); // Should print: "1 LED state is currently: 0"
    LED_getState(&yellowLED); // Should print: "2 LED state is currently: 0"
    LED_getState(&blueLED); // Should print: "3 LED state is currently: 0"

    while (1) {
        LED_setState(&redLED, LED_TOGGLE);
        LED_setState(&greenLED, LED_TOGGLE);
        LED_setState(&yellowLED, LED_TOGGLE);
        LED_setState(&blueLED, LED_TOGGLE);
        delay(800000); // ~50ms at 16 MHz
    }
}

void delay(volatile uint32_t count) {
    while(count--) {
        __asm__("nop");
    }
}
