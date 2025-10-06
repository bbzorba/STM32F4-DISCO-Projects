#ifndef _POWERLED_H
#define _POWERLED_H

#include "led.h"

typedef uint8_t led_elec_type;
typedef uint8_t led_dim_type;

typedef enum{

    CURRENT_LOW = 10, //10 mA
    CURRENT_NORMAL = 20,
    CURRENT_HIGH = 30,
    CURRENT_VERY_HIGH = 60
}LEDCurrent_Type;

typedef enum{
    DIAM_2MM = 2,
    DIAM_5MM = 5,
    DIAM_7MM = 7
}LEDDiameter_Type;

typedef enum{
    VOL_LOW = 3, //3V
    VOL_NORMAL = 5,
    VOL_HIGH = 9
}LEDVoltage_Type;


class powerLED : public LED {
private:
    LEDDiameter_Type diameter;
    LEDCurrent_Type current;
    LEDVoltage_Type voltage;

public:
    powerLED(LEDColor_Type _color, 
             LEDState_Type _state,
             LEDDiameter_Type _diameter,
             LEDCurrent_Type _current,
             LEDVoltage_Type _voltage);
    void powerLED_setDiameter(LEDDiameter_Type _diameter);
    void powerLED_setCurrent(LEDCurrent_Type _current);
    void powerLED_setVoltage(LEDVoltage_Type _voltage);
    led_elec_type powerLED_computePower(void);
    led_elec_type powerLED_getDiameter(void);
    led_elec_type powerLED_getCurrent(void);
    led_elec_type powerLED_getVoltage(void);

    // Polymorphic overrides
    void runDiagnostics() override;
    uint32_t computeEfficiency() const override;
};

#endif