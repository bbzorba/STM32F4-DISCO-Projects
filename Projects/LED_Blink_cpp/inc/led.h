#ifndef LED_H
#define LED_H

#include <stdint.h>
#include "../../Drivers/GPIO_cpp/inc/gpio.h"
#include "../../Drivers/UART_cpp/inc/uart.h"

// STM32F4-Discovery onboard LEDs mapping:
// PD12 = GREEN, PD13 = ORANGE (YELLOW), PD14 = RED, PD15 = BLUE
// Column names below follow color names used in code (so `RED` refers to the red LED on PD14).
#define LED_PORT GPIO_D
#define LED_PORT_CLK (1U << 3) // AHB1ENR GPIODEN
#define LED_PIN_GREEN  (1U << 12) // PD12 (GREEN)
#define LED_PIN_YELLOW (1U << 13) // PD13 (ORANGE/YELLOW)
#define LED_PIN_RED    (1U << 14) // PD14 (RED)
#define LED_PIN_BLUE   (1U << 15) // PD15 (BLUE)

// MODER bits: each pin N uses bits (N*2) and (N*2+1); output mode = 01b => set (N*2) bit.
#define LED_GREEN_MODE_BIT  (1U << (12*2)) // (1<<24)
#define LED_YELLOW_MODE_BIT (1U << (13*2)) // (1<<26)
#define LED_RED_MODE_BIT    (1U << (14*2)) // (1<<28)
#define LED_BLUE_MODE_BIT   (1U << (15*2)) // (1<<30)

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
    LEDState_Type getState(USART *usart) const;

    // Virtual interface (mirrors C vtable concept)
    virtual void runDiagnostics(USART *usart);
    virtual uint32_t computeEfficiency(USART *usart) const; // returns a computed metric (dummy by default)

protected:
    LEDColor_Type getColor() const { return color; }
    LEDState_Type getInternalState() const { return state; }
    void setInternalState(LEDState_Type s) { state = s; }
};

// Helper: human-readable LED color name
static inline const char *LEDColorToString(LEDColor_Type c) {
    switch (c) {
        case RED: return "RED";
        case GREEN: return "GREEN";
        case YELLOW: return "YELLOW";
        case BLUE: return "BLUE";
        default: return "UNKNOWN";
    }
}

#endif // LED_H