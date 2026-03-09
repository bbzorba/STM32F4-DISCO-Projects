#include "medicalLED.h"
#include <stdio.h>

void MedicalLED_constructor(MedicalLED_Type* const mled,
                            LEDWavelength_Type _wavelength,
                            LEDColor_Type _color, 
                            LEDState_Type _state) {
    static struct led_vtable const vtable = {
        MedicalLED_runDiagnostics,
        MedicalLED_computeEfficiency
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

uint32_t MedicalLED_computeEfficiency(USART_HandleType * usart, const LED_Type* const led_base) {
    const MedicalLED_Type *mled = (const MedicalLED_Type *)led_base;
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

    char buf[64];
    int n = snprintf(buf, sizeof(buf), "Medical LED Efficiency: %d%%\r\n",
                     efficiency);
    if (n > 0) {
        USART_WriteString(usart, buf);
    }
    
    return (uint32_t)efficiency;
}

void MedicalLED_runDiagnostics(USART_HandleType * usart, const LED_Type* const led_base) {
    const MedicalLED_Type *mled = (const MedicalLED_Type *)led_base;
    // Run example diagnostics for MedicalLED
    for(int i = 0; i < 20; i++) {
        char buf[64];
        int n = snprintf(buf, sizeof(buf), "Medical LED Diagnostics Stage %d: \r\n", i);
        if (n > 0) {
            USART_WriteString(usart, buf);
        }
        (void)mled; // placeholder for diagnostic use
        // Do some diagnostic checks here
    }
}
