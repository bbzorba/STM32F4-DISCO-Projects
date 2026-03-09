#ifndef MEDICAL_LED_H
#define MEDICAL_LED_H

#include <stdint.h>
#include "led.h"
#include "../../Drivers/UART_cpp/inc/uart.h"

typedef enum {
    INFRARED = 910, // nm
    NEAR_INFRARED = 880,
} LEDWavelength_Type;

class medicalLED : public LED {
private:
    LEDWavelength_Type wavelength;
public:
    medicalLED(LEDWavelength_Type _wavelength, 
               LEDColor_Type _color, 
               LEDState_Type _state);

    void setWavelength(USART *usart, LEDWavelength_Type w);
    LEDWavelength_Type getWavelength(USART *usart) const;

    // Polymorphic overrides
    uint32_t computeEfficiency(USART *usart) const override;
    void runDiagnostics(USART *usart) override;
};

#endif // MEDICAL_LED_H