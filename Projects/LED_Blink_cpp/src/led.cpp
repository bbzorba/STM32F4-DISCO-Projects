#include "led.h"

LED::LED(LEDColor_Type _color, LEDState_Type _state) 
//:color(_color), state(_state)
{
    this->color = _color;
    this->state = _state;

    // Enable GPIOD clock
    RCC->AHB1ENR |= LED_PORT_CLK;

    switch (_color)
    {
    case RED:
        LED_PORT->MODER &= ~(3U << (12*2));
        LED_PORT->MODER |= LED_RED_MODE_BIT;
        if (_state == LED_ON) {
            LED_PORT->ODR |= LED_PIN_RED; // Set PD12
            printf("%d LED state is set to: %d\n\r", this->color, this->state);
        }
        else {
            LED_PORT->ODR &= ~LED_PIN_RED; // Ensure PD12 is off
            printf("%d LED state is set to: %d\n\r", this->color, this->state);
        }
        break;
    case GREEN:
        LED_PORT->MODER &= ~(3U << (13*2));
        LED_PORT->MODER |= LED_GREEN_MODE_BIT;
        if (_state == LED_ON) {
            LED_PORT->ODR |= LED_PIN_GREEN; // Set PD13
            printf("%d LED state is set to: %d\n\r", this->color, this->state);
        }
        else {
            LED_PORT->ODR &= ~LED_PIN_GREEN; // Ensure PD13 is off
            printf("%d LED state is set to: %d\n\r", this->color, this->state);
        }
        break;
    case YELLOW:
        LED_PORT->MODER &= ~(3U << (14*2));
        LED_PORT->MODER |= LED_YELLOW_MODE_BIT;
        if (_state == LED_ON) {
            LED_PORT->ODR |= LED_PIN_YELLOW; // Set PD14
            printf("%d LED state is set to: %d\n\r", this->color, this->state);
        }
        else {
            LED_PORT->ODR &= ~LED_PIN_YELLOW; // Ensure PD14 is off
            printf("%d LED state is set to: %d\n\r", this->color, this->state);
        }
        break;
    case BLUE:
        LED_PORT->MODER &= ~(3U << (15*2));
        LED_PORT->MODER |= LED_BLUE_MODE_BIT;
        if (_state == LED_ON) {
            LED_PORT->ODR |= LED_PIN_BLUE; // Set PD15
            printf("%d LED state is set to: %d\n\r", this->color, this->state);
        }
        else {
            LED_PORT->ODR &= ~LED_PIN_BLUE; // Ensure PD15 is off
            printf("%d LED state is set to: %d\n\r", this->color, this->state);
        }
        break;
    default:
        break;
    }
}

void LED::setState(LEDState_Type _state){
    this->state = _state;

    switch (this->color)
    {
    case RED:
        LED_PORT->MODER &= ~(3U << (12*2));
        LED_PORT->MODER |= LED_RED_MODE_BIT;
        if (this->state == LED_ON) {
            LED_PORT->ODR |= LED_PIN_RED; // Set PD12
            printf("%d LED state is set to: %d\n\r", this->color, this->state);
        }
        else if (this->state == LED_TOGGLE) {
            LED_PORT->ODR ^= LED_PIN_RED;
            printf("%d LED toggled, new state: %d\n\r", this->color, (LED_PORT->ODR & LED_PIN_RED) ? LED_ON : LED_OFF);
        }
        else {
            LED_PORT->ODR &= ~LED_PIN_RED; // Ensure PD12 is off
            printf("%d LED state is set to: %d\n\r", this->color, this->state);
        }
        break;
    case GREEN:
        LED_PORT->MODER &= ~(3U << (13*2));
        LED_PORT->MODER |= LED_GREEN_MODE_BIT;
        if (this->state == LED_ON) {
            LED_PORT->ODR |= LED_PIN_GREEN; // Set PD13
            printf("%d LED state is set to: %d\n\r", this->color, this->state);
        }
        else if (this->state == LED_TOGGLE) {
            LED_PORT->ODR ^= LED_PIN_GREEN;
            printf("%d LED toggled, new state: %d\n\r", this->color, (LED_PORT->ODR & LED_PIN_GREEN) ? LED_ON : LED_OFF);
        }
        else {
            LED_PORT->ODR &= ~LED_PIN_GREEN; // Ensure PD13 is off
            printf("%d LED state is set to: %d\n\r", this->color, this->state);
        }
        break;
    case YELLOW:
        LED_PORT->MODER &= ~(3U << (14*2));
        LED_PORT->MODER |= LED_YELLOW_MODE_BIT;
        if (this->state == LED_ON) {
            LED_PORT->ODR |= LED_PIN_YELLOW; // Set PD14
            printf("%d LED state is set to: %d\n\r", this->color, this->state);
        }
        else if (this->state == LED_TOGGLE) {
            LED_PORT->ODR ^= LED_PIN_YELLOW;
            printf("%d LED toggled, new state: %d\n\r", this->color, (LED_PORT->ODR & LED_PIN_YELLOW) ? LED_ON : LED_OFF);
        }
        else {
            LED_PORT->ODR &= ~LED_PIN_YELLOW; // Ensure PD14 is off
            printf("%d LED state is set to: %d\n\r", this->color, this->state);
        }
        break;
    case BLUE:
        LED_PORT->MODER &= ~(3U << (15*2));
        LED_PORT->MODER |= LED_BLUE_MODE_BIT;
        if (this->state == LED_ON) {
            LED_PORT->ODR |= LED_PIN_BLUE; // Set PD15
            printf("%d LED state is set to: %d\n\r", this->color, this->state);
        }
        else if (this->state == LED_TOGGLE) {
            LED_PORT->ODR ^= LED_PIN_BLUE;
            printf("%d LED toggled, new state: %d\n\r", this->color, (LED_PORT->ODR & LED_PIN_BLUE) ? LED_ON : LED_OFF);
        }
        else {
            LED_PORT->ODR &= ~LED_PIN_BLUE; // Ensure PD15 is off
            printf("%d LED state is set to: %d\n\r", this->color, this->state);
        }
        break;
    default:
        break;
    }

    printf("%d LED state is set to: %d\n\r", this->color, this->state);
}

LEDState_Type LED::getState(void) const {

    printf("%d LED state is currently: %d\n\r", this->color, this->state);

    return this->state;
}

// Default virtual implementations
void LED::runDiagnostics() {
    // Base LED diagnostics placeholder
    printf("LED (color=%d) base diagnostics\n\r", this->getColor());
}

uint32_t LED::computeEfficiency() const {
    // Dummy metric for base
    return 0U;
}


