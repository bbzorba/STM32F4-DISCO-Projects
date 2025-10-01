#include "stm32f4xx.h"
#include "powerLed.h"
#include "led.h"

// Function prototypes
void delay(volatile uint32_t count);

LED redLED(RED, LED_ON);
powerLED greenLED(GREEN, LED_ON, DIAM_5MM, CURRENT_NORMAL, VOL_NORMAL);

LEDState_Type redLED_state;
led_elec_type greenLED_power;
led_elec_type yellowLED_voltage;
led_elec_type blueLED_current;

//main function
int main(void) {
    //USART2_Init();
    
    powerLED yellowLED(YELLOW, LED_ON, DIAM_5MM, CURRENT_NORMAL, VOL_NORMAL);
    powerLED *blueLED = new powerLED(BLUE, LED_ON, DIAM_7MM, CURRENT_HIGH, VOL_NORMAL);

    redLED_state = redLED.getState();
    greenLED_power = greenLED.powerLED_computePower();
    yellowLED_voltage = yellowLED.powerLED_getVoltage();
    blueLED_current = blueLED->powerLED_getCurrent();

    redLED.setState(LED_OFF);
    greenLED.setState(LED_OFF);
    yellowLED.setState(LED_OFF);
    blueLED->setState(LED_OFF);

    delete blueLED;
    while (1) {
        
    }
}

void delay(volatile uint32_t count) {
    while(count--) {
        __asm__("nop");
    }
}
