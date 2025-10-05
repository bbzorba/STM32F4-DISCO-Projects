#include "stm32f4xx.h"
#include "powerLed.h"
#include "medicalLED.h"

// Function prototypes
void delay(volatile uint32_t count);

LED_Type greenLED;
MedicalLED_Type redLED;

powerLED_Type yellowLED;
powerLED_Type blueLED;

led_elec_type yellow_led_power;
led_elec_type blue_led_power;

//main function
int main(void) {
    //USART2_Init();
    
    LED_constructor(&greenLED, GREEN, LED_OFF);
    MedicalLED_constructor(&redLED, INFRARED, (LEDColor_Type)INFRARED, LED_OFF);
    powerLED_constructor(&yellowLED, YELLOW, LED_OFF, DIAM_5MM,CURRENT_NORMAL, VOL_NORMAL);
    powerLED_constructor(&blueLED, BLUE, LED_OFF, DIAM_7MM,CURRENT_HIGH, VOL_NORMAL);

    yellow_led_power = PowerLED_computePower(&yellowLED);
    blue_led_power = PowerLED_computePower(&blueLED);

    while (1) {
        LED_setState(&redLED, LED_TOGGLE);
        LED_setState(&greenLED, LED_TOGGLE);
        // Cast powerLED_Type* to LED_Type* to match LED_setState signature and silence warnings
        LED_setState((LED_Type*)&yellowLED, LED_TOGGLE);
        LED_setState((LED_Type*)&blueLED, LED_TOGGLE);
        delay(800000); // ~50ms at 16 MHz
    }
}

void delay(volatile uint32_t count) {
    while(count--) {
        __asm__("nop");
    }
}
