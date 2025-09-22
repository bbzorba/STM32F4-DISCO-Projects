#ifndef LED_H
#define LED_H

#include <stdint.h>
#include "uart.h"

typedef enum {
    RED = 0,   // PD12
    GREEN = 1, // PD13
    YELLOW = 2,  // PD14
    BLUE = 3 // PD15
} LEDColor_Type;

typedef enum {
    LED_OFF = 0,
    LED_ON = 1,
    LED_TOGGLE = 2
} LEDState_Type;

//LED's attributes
typedef struct {
    LEDColor_Type color;
    LEDState_Type state;
} LED_Type;

void LED_constructor(LED_Type* const led, LEDColor_Type _color, LEDState_Type _state);
void LED_setState(LED_Type* const led, LEDState_Type _state);
LEDState_Type LED_getState(const LED_Type* const led);

#endif // LED_H