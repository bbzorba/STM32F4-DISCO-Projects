#include "powerLed.h"


void powerLED_constructor(powerLED_Type * const pled, 
                          LEDColor_Type _color, 
                          LEDState_Type _state,
                          LEDDiameter_Type _diameter,
                          LEDCurrent_Type _current,
                          LEDVoltage_Type _voltage){
                            
                            static struct led_vtable const vtable = {
                            (void (*)(LED_Type const * const))&PowerLED_runDiagnostics,
                            (uint32_t (*)(LED_Type const * const))&PowerLED_computeEfficiency
                            };
                            LED_constructor(&pled->super,_color,_state);
                            pled->super.vptr = &vtable; // Override vtable to PowerLED's
                            pled->diameter = _diameter;
                            pled->voltage = _voltage;
                            pled->current = _current;
                          }

void PowerLED_setDiameter(powerLED_Type * const pled, LEDDiameter_Type _diameter){
    pled->diameter = _diameter;
}

void PowerLED_setCurrent(powerLED_Type * const pled, LEDCurrent_Type _current){
    pled->current = _current;
}

void PowerLED_setVoltage(powerLED_Type * const pled, LEDVoltage_Type _voltage){
    pled->voltage = _voltage;
}

led_elec_type PowerLED_computePower(powerLED_Type * const pled){
    return (pled->current * pled->voltage) / 1000; // Power in Watts (mA * V)
}

led_elec_type PowerLED_getDiameter(powerLED_Type * const pled){
    return pled->diameter;
}

led_elec_type PowerLED_getCurrent(powerLED_Type * const pled){
    return pled->current;
}

led_elec_type PowerLED_getVoltage(powerLED_Type * const pled){
    return pled->voltage;
}

uint32_t PowerLED_computeEfficiency(const powerLED_Type* const pled) {
    const int8_t input = 120;
    const int8_t output = 86;
    int8_t eff = ((input/output)*100) - (pled->current)/2;
    printf("Power LED Efficiency: %d%%\n\r", eff);

    return eff;
}

void PowerLED_runDiagnostics(const powerLED_Type* const pled) {
    for (int i = 0; i < (int)pled->current; i++)
    {
        printf("Power LED Diagnostics Stage %d: ", i);
        // Do some diagnostic checks here
    }
    
}
