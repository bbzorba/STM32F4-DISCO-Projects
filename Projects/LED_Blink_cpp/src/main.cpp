#include "stm32f4xx.h"
#include "led.h"

// Function prototypes
void delay(volatile uint32_t count);

Led redLED(RED, LED_ON);

//main function
int main(void) {
    //USART2_Init();

    Led greenLED(GREEN, LED_ON);

    Led *yellowLED = new Led(YELLOW, LED_ON);
    Led *blueLED = new Led(BLUE, LED_ON);

    redLED.getState(); // Should print: "0 LED state is currently: 0"
    greenLED.getState(); // Should print: "1 LED state is currently: 0"
    yellowLED->getState(); // Should print: "2 LED state is currently: 0"
    blueLED->getState(); // Should print: "3 LED state is currently: 0"

    redLED.setState(LED_TOGGLE);
    greenLED.setState(LED_TOGGLE);
    yellowLED->setState(LED_TOGGLE);
    blueLED->setState(LED_TOGGLE);
    
    delete yellowLED;
    delete blueLED;

    while (1) {
        redLED.setState(LED_TOGGLE);
        greenLED.setState(LED_TOGGLE);
        delay(800000); // ~50ms at 16 MHz
    }
}

void delay(volatile uint32_t count) {
    while(count--) {
        __asm__("nop");
    }
}
