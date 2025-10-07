#ifndef MEDICAL_LED_H
#define MEDICAL_LED_H

#include <stdint.h>
#include <new>
#include "led.h"
#include "../../Drivers/UART/inc/uart.h"

typedef enum {
    INFRARED = 910, // nm
    NEAR_INFRARED = 880,
} LEDWavelength_Type;

class MedicalLED : public LED {
private:
    LEDWavelength_Type wavelength;
public:
    MedicalLED(LEDWavelength_Type _wavelength, 
               LEDColor_Type _color, 
               LEDState_Type _state);

    void setWavelength(LEDWavelength_Type w) { wavelength = w; }
    LEDWavelength_Type getWavelength() const { return wavelength; }

    // Polymorphic overrides
    uint32_t computeEfficiency() const override;
    void runDiagnostics() override;
};

#endif // MEDICAL_LED_H