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

led_elec_type powerLED::powerLED_getDiameter(void){
    return this->diameter;
}

led_elec_type powerLED::powerLED_getCurrent(void){
    return this->current;
}

led_elec_type powerLED::powerLED_getVoltage(void){
    return this->voltage;
}

// Override virtuals from LED (single implementation)
uint32_t powerLED::computeEfficiency() const {
    const uint32_t input = 120U;
    const uint32_t output = 86U;
    uint32_t eff = ((input * 100U) / output);
    if (current != 0) {
        // Slightly adjust by current class (arbitrary demo logic)
        eff -= (static_cast<uint32_t>(current) / 2U);
    }
    printf("Power LED Efficiency: %lu%%\n\r", (unsigned long)eff);
    return eff;
}

void powerLED::runDiagnostics() {
    printf("powerLED diag start (color=%d)\n\r", getColor());
    for (uint32_t i = 0; i < static_cast<uint32_t>(current); i += 10U) {
        printf("Diag step %lu power=%u\n\r", (unsigned long)i, (unsigned)powerLED_computePower());
    }
    printf("powerLED diag end\n\r");
}
