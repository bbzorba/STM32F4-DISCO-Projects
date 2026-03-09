#include "medicalLED.h"

medicalLED::medicalLED(LEDWavelength_Type wavelength,
                       LEDColor_Type color,
                       LEDState_Type state)
    : LED(color, state), wavelength(wavelength) {}

void medicalLED::setWavelength(USART *usart, LEDWavelength_Type w) { 
    char buf[64];
    int n = snprintf(buf, sizeof(buf), "Setting Medical LED wavelength to: %d nm\r\n", (int)w);
    if (n > 0) {
        usart->USART_WriteString(buf);
    }
    wavelength = w; 
}

LEDWavelength_Type medicalLED::getWavelength(USART *usart) const {
    char buf[64];
    int n = snprintf(buf, sizeof(buf), "Medical LED wavelength is currently: %d nm\r\n", (int)wavelength);
    if (n > 0) {
        usart->USART_WriteString(buf);
    }
    return wavelength;
}

uint32_t medicalLED::computeEfficiency(USART *usart) const {
    // Simple reused logic based on wavelength bands
    const uint32_t input = 120U;
    const uint32_t output = 86U;
    const uint32_t kInfra = 4U;
    const uint32_t kNear  = 6U;
    uint32_t K = (wavelength == INFRARED) ? kInfra : kNear;
    uint32_t eff = ((input * 100U) / output);
    if (eff > K) eff -= K; else eff = 0;
    char buf[64];
    int n = snprintf(buf, sizeof(buf), "Medical LED Efficiency: %lu%%\r\n", (unsigned long)eff);
    if (n > 0) {
        usart->USART_WriteString(buf);
    }
    return eff;
}

void medicalLED::runDiagnostics(USART *usart) {
    char buf[128];
    int n = snprintf(buf, sizeof(buf), "-- Medical LED Diagnostics (%s) --\r\n", LEDColorToString(getColor()));
    if (n > 0) usart->USART_WriteString(buf);
    for (int i = 0; i < 10; ++i) {
        int n = snprintf(buf, sizeof(buf), "Stage %d\r\n", i);
        if (n > 0) usart->USART_WriteString(buf);
    }
    n = snprintf(buf, sizeof(buf), "-- End Diagnostics (%s) --\r\n\r\n", LEDColorToString(getColor()));
    if (n > 0) usart->USART_WriteString(buf);
}
