#include "../inc/powerLed.h"
#include "../inc/medicalLED.h"
#include "../inc/led.h"

// Function prototypes
void delay(volatile uint32_t count);

USART usart(USART_2, RX_AND_TX, __115200);

LED redLED(RED, LED_OFF);
medicalLED greenLED(INFRARED, GREEN, LED_OFF);
powerLED yellowLED(YELLOW, LED_OFF, DIAM_5MM, CURRENT_NORMAL, VOL_NORMAL);
powerLED *blueLED = new powerLED(BLUE, LED_OFF, DIAM_7MM, CURRENT_HIGH, VOL_NORMAL);

//main function
int main(void) {
    usart.USART_WriteString("\r\n Power LED & Medical LED Efficiency Computing & Diagnostics Application\r\n\r\n");

    // Print initial status for all LEDs
    redLED.getState(&usart);
    greenLED.getState(&usart);
    yellowLED.getState(&usart);
    blueLED->getState(&usart);

    // Print one-time measurements
    greenLED.getWavelength(&usart);
    yellowLED.powerLED_getVoltage(&usart);
    yellowLED.powerLED_getCurrent(&usart);

    while (1) {
        redLED.setState(LED_TOGGLE);
        greenLED.setState(LED_TOGGLE);
        yellowLED.setState(LED_TOGGLE);
        blueLED->setState(LED_TOGGLE);

        delay(8000000);

        // Print current state of all LEDs
        redLED.getState(&usart);
        delay(800000);
        greenLED.getState(&usart);
        delay(800000);
        yellowLED.getState(&usart);
        delay(800000);
        blueLED->getState(&usart);

        delay(8000000);

        // Power LED (yellow) efficiency & diagnostics
        yellowLED.computeEfficiency(&usart);
        yellowLED.runDiagnostics(&usart);

        delay(8000000);

        // Power LED (blue) efficiency & diagnostics
        blueLED->computeEfficiency(&usart);
        blueLED->runDiagnostics(&usart);

        delay(8000000);

        // Medical LED efficiency & diagnostics
        greenLED.computeEfficiency(&usart);
        greenLED.runDiagnostics(&usart);

        delay(8000000);
    }

    //delete blueLED;
    return 0;
}


void delay(volatile uint32_t count) {
    while(count--) {
        __asm__("nop");
    }
}
