#include "medicalLED.h"

MedicalLED::MedicalLED(LEDWavelength_Type wavelength,
                       LEDColor_Type color,
                       LEDState_Type state)
    : LED(color, state), wavelength(wavelength) {}

uint32_t MedicalLED::computeEfficiency() const {
    // Simple reused logic based on wavelength bands
    const uint32_t input = 120U;
    const uint32_t output = 86U;
    const uint32_t kInfra = 4U;
    const uint32_t kNear  = 6U;
    uint32_t K = (wavelength == INFRARED) ? kInfra : kNear;
    uint32_t eff = ((input * 100U) / output);
    if (eff > K) eff -= K; else eff = 0;
    printf("Medical LED Efficiency: %lu%%\n\r", (unsigned long)eff);
    return eff;
}

void MedicalLED::runDiagnostics() {
    for (int i = 0; i < 10; ++i) {
        printf("Medical LED Diagnostics Stage %d wavelength=%u nm\n\r", i, (unsigned)wavelength);
    }
}
