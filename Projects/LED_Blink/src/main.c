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

    LED_setState(&redLED, LED_ON);
    LED_setState(&greenLED, LED_ON);
    LED_setState(&yellowLED, LED_ON);
    LED_setState(&blueLED, LED_ON);

    while (1) {

    }
}

// Initialization function
void init(void) {
    // Enable GPIOD clock
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
    // Set PD12, PD13, PD14, PD15 as output
    GPIOD->MODER |= (1 << (12*2)) | (1 << (13*2)) | (1 << (14*2)) | (1 << (15*2));
    GPIOD->OTYPER &= ~((1 << 12) | (1 << 13) | (1 << 14) | (1 << 15));
    GPIOD->OSPEEDR |= (3 << (12*2)) | (3 << (13*2)) | (3 << (14*2)) | (3 << (15*2));
    GPIOD->PUPDR &= ~((3 << (12*2)) | (3 << (13*2)) | (3 << (14*2)) | (3 << (15*2)));
}

void delay(volatile uint32_t count) {
    while(count--) {
        __asm__("nop");
    }
}
