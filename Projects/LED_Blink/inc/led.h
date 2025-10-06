#ifndef LED_H
#define LED_H

#include <stdint.h>
#include "../../Drivers/UART/inc/uart.h"
#include "stm32f4xx.h"

#define LED_PORT GPIOD
#define LED_PORT_CLK (1U << 3) // AHB1ENR GPIODEN
// STM32F4-Discovery onboard LEDs:
// PD12 = Green, PD13 = Orange, PD14 = Red, PD15 = Blue
// Project kept YELLOW label for Orange LED.
// Color enums below map: GREEN->PD12, YELLOW(orange)->PD13, RED->PD14, BLUE->PD15
// Adjust pin and mode bit macros accordingly.
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
    GREEN = 0,  // PD12
    YELLOW = 1, // PD13 (Orange on board silkscreen)
    RED = 2,    // PD14
    BLUE = 3    // PD15
} LEDColor_Type;

typedef enum {
    LED_OFF = 0,
    LED_ON = 1,
    LED_TOGGLE = 2
} LEDState_Type;

//LED's attributes
typedef struct {
    struct led_vtable const *vptr; //virtual pointer (constant => immutable)
    LEDColor_Type color;
    LEDState_Type state;
} LED_Type;

//LED virtual table
struct led_vtable
{
    void (*run_diagnostics)(LED_Type const * const led);
    uint32_t (*compute_efficiency)(LED_Type const * const led);
};


void LED_constructor(LED_Type * const led, LEDColor_Type _color, LEDState_Type _state);
void LED_setState(LED_Type * const led, LEDState_Type _state);
LEDState_Type LED_getState(const LED_Type * const led);


//method 1
static inline void LED_run_diagnostics(LED_Type const * const led) {
        (led->vptr->run_diagnostics)(led);
}

static inline uint32_t LED_compute_efficiency(LED_Type const * const led) {
        return (led->vptr->compute_efficiency)(led);
}

//method 2
#define LED_RUN_DIAGNOSTICS(led) (*(led)->vptr->run_diagnostics(led))
#define LED_COMPUTE_EFFICIENCY(led) (*(led)->vptr->compute_efficiency(led))

void runSystemsDiagnostics(LED_Type const *led_modules[]);
void computeLEDsEfficiency(LED_Type const *led_modules[]);

#endif // LED_H