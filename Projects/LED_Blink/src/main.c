#include "../inc/powerLed.h"
#include "../inc/medicalLED.h"

// Function prototypes
void delay(volatile uint32_t count);

USART_HandleType usart;

LED_Type greenLED;
MedicalLED_Type redLED;

powerLED_Type yellowLED;
powerLED_Type blueLED;

led_elec_type yellow_led_power;
led_elec_type blue_led_power;

//main function
int main(void) {
    USART_constructor(&usart, USART_2, RX_AND_TX, __115200);
    USART_WriteString(&usart, "\r\n Power LED & Medical LED Efficiency Computing & Diagnostics Application\r\n");

    LED_constructor(&greenLED, GREEN, LED_OFF);
    // Use a valid LEDColor_Type (e.g., RED) instead of casting wavelength enum
    MedicalLED_constructor(&redLED, INFRARED, RED, LED_OFF);
    powerLED_constructor(&yellowLED, YELLOW, LED_OFF, DIAM_5MM,CURRENT_NORMAL, VOL_NORMAL);
    powerLED_constructor(&blueLED, BLUE, LED_OFF, DIAM_7MM,CURRENT_HIGH, VOL_NORMAL);

    yellow_led_power = PowerLED_computePower(&yellowLED);
    blue_led_power = PowerLED_computePower(&blueLED);

    while (1) {
        LED_setState(&redLED.super, LED_TOGGLE); // operate on base
        LED_setState(&greenLED, LED_TOGGLE);
        // Cast powerLED_Type* to LED_Type* to match LED_setState signature and silence warnings
        LED_setState((LED_Type*)&yellowLED, LED_TOGGLE);
        LED_setState((LED_Type*)&blueLED, LED_TOGGLE);

        delay(8000000);

        PowerLED_computeEfficiency(&usart, &yellowLED.super);
        PowerLED_runDiagnostics(&usart, &yellowLED.super);

        delay(8000000);

        PowerLED_computeEfficiency(&usart, &blueLED.super);
        PowerLED_runDiagnostics(&usart, &blueLED.super);

        delay(8000000);

        MedicalLED_computeEfficiency(&usart, &redLED.super);
        MedicalLED_runDiagnostics(&usart, &redLED.super);

        delay(8000000);
    }
}

void delay(volatile uint32_t count) {
    while(count--) {
        __asm__("nop");
    }
}
