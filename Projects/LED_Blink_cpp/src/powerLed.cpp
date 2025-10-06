#include "powerLed.h"

powerLED::powerLED(LEDColor_Type _color,
                                     LEDState_Type _state,
                                     LEDDiameter_Type _diameter,
                                     LEDCurrent_Type _current,
                                     LEDVoltage_Type _voltage)
        : LED(_color, _state),
            diameter(_diameter), current(_current), voltage(_voltage)
{
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

// Virtual overrides
uint32_t powerLED::computeEfficiency() const {
    const uint32_t input = 120U;
    const uint32_t output = 86U;
    uint32_t eff = ((input * 100U) / output) - (static_cast<uint32_t>(this->current)/2U);
    printf("Power LED Efficiency: %lu%%\n\r", (unsigned long)eff);
    return eff;
}

void powerLED::runDiagnostics() {
    for (int i = 0; i < (int)this->current; i++) {
        printf("Power LED Diagnostics Stage %d\n\r", i);
    }
}