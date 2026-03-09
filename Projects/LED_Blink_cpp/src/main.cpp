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

LEDState_Type redLED_state;
LEDWavelength_Type greenLED_wavelength;
led_elec_type yellowLED_voltage;
led_elec_type blueLED_current;

//main function
int main(void) {

    redLED_state = redLED.getState(&usart);
    greenLED_wavelength = greenLED.getWavelength(&usart);
    yellowLED_voltage = yellowLED.powerLED_getVoltage(&usart);
    blueLED_current = blueLED->powerLED_getCurrent(&usart);

    while (1) {
        redLED.setState(LED_TOGGLE);
        greenLED.setState(LED_TOGGLE);
        yellowLED.setState(LED_TOGGLE);
        blueLED->setState(LED_TOGGLE);

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
