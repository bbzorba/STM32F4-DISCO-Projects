#include "powerLed.h"

powerLED::powerLED(LEDColor_Type _color,
                   LEDState_Type _state,
                   LEDDiameter_Type _diameter,
                   LEDCurrent_Type _current,
                   LEDVoltage_Type _voltage)
    : LED(_color, _state),
      diameter(_diameter), current(_current), voltage(_voltage)
{
    // No manual vtable needed; C++ virtual mechanism handles dynamic dispatch.
}


void powerLED::powerLED_setDiameter(LEDDiameter_Type _diameter){
    this->diameter = _diameter;
}

void powerLED::powerLED_setCurrent(LEDCurrent_Type _current){
    this->current = _current;
}

void powerLED::powerLED_setVoltage(LEDVoltage_Type _voltage){
    this->voltage = _voltage;
}

led_elec_type powerLED::powerLED_computePower(void){
    return (this->current * this->voltage); // Power in W
}

led_elec_type powerLED::powerLED_getDiameter(USART *usart){
    char buf[64];
    int n = snprintf(buf, sizeof(buf), "Power LED diameter: %u mm\r\n", (unsigned)this->diameter);
    if (n > 0) {
        usart->USART_WriteString(buf);
    }
    return this->diameter;
}

led_elec_type powerLED::powerLED_getCurrent(USART *usart){
    char buf[64];
    int n = snprintf(buf, sizeof(buf), "Power LED current: %u mA\r\n", (unsigned)this->current);
    if (n > 0) {
        usart->USART_WriteString(buf);
    }
    return this->current;
}

led_elec_type powerLED::powerLED_getVoltage(USART *usart){
    char buf[64];
    int n = snprintf(buf, sizeof(buf), "Power LED voltage: %u V\r\n", (unsigned)this->voltage);
    if (n > 0) {
        usart->USART_WriteString(buf);
    }
    return this->voltage;
}

// Override virtuals from LED (single implementation)
uint32_t powerLED::computeEfficiency(USART *usart) const {
    const uint32_t input = 120U;
    const uint32_t output = 86U;
    uint32_t eff = ((input * 100U) / output);
    if (current != 0) {
        eff -= (static_cast<uint32_t>(current) / 2U);
    }
    char buf[32];
    const char *color_name = LEDColorToString(getColor());
    usart->USART_WriteString("-- Power LED Efficiency (");
    usart->USART_WriteString(color_name);
    usart->USART_WriteString(") --\r\n");
    int n = snprintf(buf, sizeof(buf), "Efficiency: %lu%%\r\n\r\n", (unsigned long)eff);
    if (n > 0) usart->USART_WriteString(buf);
    return eff;
}

void powerLED::runDiagnostics(USART *usart) {
    const char *color_name = LEDColorToString(getColor());
    usart->USART_WriteString("-- Power LED Diagnostics (");
    usart->USART_WriteString(color_name);
    usart->USART_WriteString(") --\r\n");
    char buf[48];
    for (uint32_t i = 0; i < static_cast<uint32_t>(current); i += 10U) {
        int n = snprintf(buf, sizeof(buf), "Step %lu: power = %u mW\r\n",
                         (unsigned long)i, (unsigned)powerLED_computePower());
        if (n > 0) usart->USART_WriteString(buf);
    }
    usart->USART_WriteString("-- End Diagnostics (");
    usart->USART_WriteString(color_name);
    usart->USART_WriteString(") --\r\n\r\n");
}
