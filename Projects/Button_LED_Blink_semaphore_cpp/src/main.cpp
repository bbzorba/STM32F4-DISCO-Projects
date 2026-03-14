#include "../inc/button_led.h"

LED greenLED(GREEN, LED_OFF);
LED redLED(RED, LED_OFF);
LED yellowLED(YELLOW, LED_OFF);
LED blueLED(BLUE, LED_OFF);

// NULL-terminated array passed to blink functions so each is called only once.
LED *all_leds[] = {&greenLED, &yellowLED, &redLED, &blueLED, nullptr};

Button userButton(GPIO_A, GPIO_PIN_0,
                  GPIO_MODE_INPUT, GPIO_PULLDOWN, GPIO_SPEED_LOW, 1);
                  
USART serial_screen(USART_2, RX_AND_TX, __115200);

int main(void) {

    serial_screen.USART_WriteString("\r\n Button LED Blink Application with Synchronization\r\n");

    while (1) {
        /* Sync holds the mutex for 5 cycles (~2.5 s).  If the button is
         * pressed mid-blink, async is attempted inside sync and gets
         * "mutex busy".  It succeeds here, after the mutex is released.  */
        blink_LEDS_sync(serial_screen, all_leds);
        blink_LEDS_async(serial_screen, all_leds);
    }
}