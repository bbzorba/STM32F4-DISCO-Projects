#include "led.h"

static void run_diagnostics(LED_Type const * const led);
static uint32_t compute_efficiency(LED_Type const * const led);

void LED_constructor(LED_Type* const led, LEDColor_Type _color, LEDState_Type _state){

    // Initialize virtual table pointer
    static struct led_vtable const vtable = {
        &run_diagnostics,
        &compute_efficiency
    };

    led->vptr = &vtable;
    led->color = _color;
    led->state = _state;

    // Enable GPIOD clock
    RCC->AHB1ENR |= LED_PORT_CLK;

    switch (_color)
    {
    case GREEN: // PD12
        LED_PORT->MODER &= ~(3U << (12*2));
        LED_PORT->MODER |= LED_GREEN_MODE_BIT;
        if (_state == LED_ON) {
            LED_PORT->ODR |= LED_PIN_GREEN; // Set PD12
        } else {
            LED_PORT->ODR &= ~LED_PIN_GREEN;
        }
        break;
    case YELLOW: // PD13 (Orange)
        LED_PORT->MODER &= ~(3U << (13*2));
        LED_PORT->MODER |= LED_YELLOW_MODE_BIT;
        if (_state == LED_ON) {
            LED_PORT->ODR |= LED_PIN_YELLOW; // Set PD13
        } else {
            LED_PORT->ODR &= ~LED_PIN_YELLOW;
        }
        break;
    case RED: // PD14
        LED_PORT->MODER &= ~(3U << (14*2));
        LED_PORT->MODER |= LED_RED_MODE_BIT;
        if (_state == LED_ON) {
            LED_PORT->ODR |= LED_PIN_RED; // Set PD14
        } else {
            LED_PORT->ODR &= ~LED_PIN_RED;
        }
        break;
    case BLUE: // PD15
        LED_PORT->MODER &= ~(3U << (15*2));
        LED_PORT->MODER |= LED_BLUE_MODE_BIT;
        if (_state == LED_ON) {
            LED_PORT->ODR |= LED_PIN_BLUE; // Set PD15
        } else {
            LED_PORT->ODR &= ~LED_PIN_BLUE;
        }
        break;
    default:
        break;
    }
}

void LED_setState(LED_Type* const led, LEDState_Type _state){
    led->state = _state;

    switch (led->color)
    {
    case GREEN: // PD12
        LED_PORT->MODER &= ~(3U << (12*2));
        LED_PORT->MODER |= LED_GREEN_MODE_BIT;
        if (led->state == LED_ON) {
            LED_PORT->ODR |= LED_PIN_GREEN;
        } else if (led->state == LED_TOGGLE) {
            LED_PORT->ODR ^= LED_PIN_GREEN;
        } else {
            LED_PORT->ODR &= ~LED_PIN_GREEN;
        }
        break;
    case YELLOW: // PD13 (Orange)
        LED_PORT->MODER &= ~(3U << (13*2));
        LED_PORT->MODER |= LED_YELLOW_MODE_BIT;
        if (led->state == LED_ON) {
            LED_PORT->ODR |= LED_PIN_YELLOW;
        } else if (led->state == LED_TOGGLE) {
            LED_PORT->ODR ^= LED_PIN_YELLOW;
        } else {
            LED_PORT->ODR &= ~LED_PIN_YELLOW;
        }
        break;
    case RED: // PD14
        LED_PORT->MODER &= ~(3U << (14*2));
        LED_PORT->MODER |= LED_RED_MODE_BIT;
        if (led->state == LED_ON) {
            LED_PORT->ODR |= LED_PIN_RED;
        } else if (led->state == LED_TOGGLE) {
            LED_PORT->ODR ^= LED_PIN_RED;
        } else {
            LED_PORT->ODR &= ~LED_PIN_RED;
        }
        break;
    case BLUE: // PD15
        LED_PORT->MODER &= ~(3U << (15*2));
        LED_PORT->MODER |= LED_BLUE_MODE_BIT;
        if (led->state == LED_ON) {
            LED_PORT->ODR |= LED_PIN_BLUE;
        } else if (led->state == LED_TOGGLE) {
            LED_PORT->ODR ^= LED_PIN_BLUE;
        } else {
            LED_PORT->ODR &= ~LED_PIN_BLUE;
        }
        break;
    default:
        break;
    }
}

LEDState_Type LED_getState(const LED_Type* const led){

    printf("%d LED state is currently: %d\n\r", led->color, led->state);

    return led->state;
}

static void run_diagnostics(LED_Type const * const led) {
   (void)led; // Suppress unused parameter warning
}

static uint32_t compute_efficiency(LED_Type const * const led) {
    (void)led; // Suppress unused parameter warning
    return 0U; // Dummy value
}

//Polymorphic functions implementations (child classes will also run these functions)
void runSystemsDiagnostics(LED_Type const *led_modules[]) {
    for (int i = 0; led_modules[i] != (LED_Type *)0; i++) {
        LED_run_diagnostics(led_modules[i]);
    }
}

void computeLEDsEfficiency(LED_Type const *led_modules[]) {
    for (int i = 0; led_modules[i] != (LED_Type *)0; i++) {
        LED_compute_efficiency(led_modules[i]);
    }
}
