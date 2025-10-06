#ifndef LED_H
#define LED_H

#include <stdint.h>
#include "../../../Drivers/UART/inc/uart.h"
#include "stm32f4xx.h"

#define LED_PORT GPIOD
#define LED_PORT_CLK (1U << 3) // AHB1ENR GPIODEN
#define LED_PIN_RED (1U << 12)   // PD12
#define LED_PIN_GREEN (1U << 13) // PD13
#define LED_PIN_YELLOW (1U << 14) // PD14
#define LED_PIN_BLUE (1U << 15)  // PD15

#define LED_RED_MODE_BIT (1U << 24)   // MODER12[1:0] = 01 (General purpose output mode)
#define LED_GREEN_MODE_BIT (1U << 26) // MODER13[1:0] = 01 (General purpose output mode)
#define LED_YELLOW_MODE_BIT (1U << 28) // MODER14[1:0] = 01 (General purpose output mode)
#define LED_BLUE_MODE_BIT (1U << 30)  // MODER15[1:0] = 01 (General purpose output mode)

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
class LED {
private:
    LEDColor_Type color;
    LEDState_Type state;
public:
    LED(LEDColor_Type _color, LEDState_Type _state);
    virtual ~LED() = default;

    void setState(LEDState_Type _state);
    LEDState_Type getState() const;

    // Virtual interface (mirrors C vtable concept)
    virtual void runDiagnostics();
    virtual uint32_t computeEfficiency() const; // returns a computed metric (dummy by default)

protected:
    LEDColor_Type getColor() const { return color; }
    LEDState_Type getInternalState() const { return state; }
    void setInternalState(LEDState_Type s) { state = s; }
};

#endif // LED_H