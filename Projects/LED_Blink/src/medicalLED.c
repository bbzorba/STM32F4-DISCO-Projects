#include "medicalLED.h"

void MedicalLED_constructor(MedicalLED_Type* const mled,
                            LEDWavelength_Type _wavelength,
                            LEDColor_Type _color, 
                            LEDState_Type _state) {
    static struct led_vtable const vtable = {
        (void (*)(LED_Type const * const))&MedicalLED_runDiagnostics,
        (uint32_t (*)(LED_Type const * const))&MedicalLED_computeEfficiency
    };
    
    // Initialize base LED part
    LED_constructor(&mled->super, _color, _state);
    // Initialize MedicalLED specific part
    mled->wavelength = _wavelength;
    mled->super.vptr = &vtable; // Override vtable to MedicalLED's
}

LEDWavelength_Type MedicalLED_getWavelength(const MedicalLED_Type* const mled) {
    return mled->wavelength;
}

void MedicalLED_setWavelength(MedicalLED_Type* const mled, LEDWavelength_Type _wavelength) {
    mled->wavelength = _wavelength;
}

uint32_t MedicalLED_computeEfficiency(const MedicalLED_Type* const mled) {
    // Example efficiency calculation
    const int8_t input = 120;
    const int8_t output = 86;
    const int8_t k1 = 4;
    const int8_t k2 = 6;

    int8_t K;
    if (mled->wavelength == INFRARED) {
        K = k1;
    } else { // NEAR_INFRARED
        K = k2;
    }
    int8_t efficiency = ((input/output)*100)-K;
    printf("Medical LED Efficiency: %d%%\n\r", efficiency);
    return efficiency;
}

void MedicalLED_runDiagnostics(const MedicalLED_Type* const mled) {
    // Run example diagnostics for MedicalLED
    for(int i = 0; i < 20; i++) {
        printf("Medical LED Diagnostics Stage %d: ", i);
        // Do some diagnostic checks here
    }
}