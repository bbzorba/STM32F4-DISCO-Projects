#include "powerLed.h"


void powerLED_constructor(powerLED_Type * const pled, 
                          LEDColor_Type _color, 
                          LEDState_Type _state,
                          LEDDiameter_Type _diameter,
                          LEDCurrent_Type _current,
                          LEDVoltage_Type _voltage){
                            
                            LED_constructor(&pled->super,_color,_state);
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

}

led_elec_type PowerLED_getDiameter(powerLED_Type * const pled){

}

led_elec_type PowerLED_getCurrent(powerLED_Type * const pled){

}

led_elec_type PowerLED_getVoltage(powerLED_Type * const pled){

}
