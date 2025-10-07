#include "stm32f4xx.h"
#include "powerLed.h"
#include "medicalLED.h"
#include "led.h"

// Function prototypes
void delay(volatile uint32_t count);

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
    //USART2_Init();

    redLED_state = redLED.getState();
    greenLED_wavelength = greenLED.getWavelength();
    yellowLED_voltage = yellowLED.powerLED_getVoltage();
    blueLED_current = blueLED->powerLED_getCurrent();

    while (1) {
        redLED.setState(LED_TOGGLE);
        greenLED.setState(LED_TOGGLE);
        yellowLED.setState(LED_TOGGLE);
        blueLED->setState(LED_TOGGLE);
        delay(800000); // ~50ms at 16 MHz
    }

    //delete blueLED;
    return 0;
}


void delay(volatile uint32_t count) {
    while(count--) {
        __asm__("nop");
    }
}
