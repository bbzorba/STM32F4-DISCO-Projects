#include "../inc/led.h"

LED::LED(LEDColor_Type _color, LEDState_Type _state) 
//:color(_color), state(_state)
{
    this->color = _color;
    this->state = _state;

    // Enable GPIOD clock
    RCC->AHB1ENR |= LED_PORT_CLK;

    switch (_color)
    {
    case RED:    // PD14
        LED_PORT->MODER &= ~(3U << (14*2));
        LED_PORT->MODER |= LED_RED_MODE_BIT;
        if (_state == LED_ON) {
            LED_PORT->ODR |= LED_PIN_RED;
        } else {
            LED_PORT->ODR &= ~LED_PIN_RED;
        }
        break;
    case GREEN:  // PD12
        LED_PORT->MODER &= ~(3U << (12*2));
        LED_PORT->MODER |= LED_GREEN_MODE_BIT;
        if (_state == LED_ON) {
            LED_PORT->ODR |= LED_PIN_GREEN;
        } else {
            LED_PORT->ODR &= ~LED_PIN_GREEN;
        }
        break;
    case YELLOW: // PD13
        LED_PORT->MODER &= ~(3U << (13*2));
        LED_PORT->MODER |= LED_YELLOW_MODE_BIT;
        if (_state == LED_ON) {
            LED_PORT->ODR |= LED_PIN_YELLOW;
        } else {
            LED_PORT->ODR &= ~LED_PIN_YELLOW;
        }
        break;
    case BLUE:   // PD15
        LED_PORT->MODER &= ~(3U << (15*2));
        LED_PORT->MODER |= LED_BLUE_MODE_BIT;
        if (_state == LED_ON) {
            LED_PORT->ODR |= LED_PIN_BLUE;
        } else {
            LED_PORT->ODR &= ~LED_PIN_BLUE;
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
    case RED:    // PD14
        LED_PORT->MODER &= ~(3U << (14*2));
        LED_PORT->MODER |= LED_RED_MODE_BIT;
        if (this->state == LED_ON)     { LED_PORT->ODR |=  LED_PIN_RED; }
        else if (this->state == LED_TOGGLE) { LED_PORT->ODR ^=  LED_PIN_RED; }
        else                           { LED_PORT->ODR &= ~LED_PIN_RED; }
        break;
    case GREEN:  // PD12
        LED_PORT->MODER &= ~(3U << (12*2));
        LED_PORT->MODER |= LED_GREEN_MODE_BIT;
        if (this->state == LED_ON)     { LED_PORT->ODR |=  LED_PIN_GREEN; }
        else if (this->state == LED_TOGGLE) { LED_PORT->ODR ^=  LED_PIN_GREEN; }
        else                           { LED_PORT->ODR &= ~LED_PIN_GREEN; }
        break;
    case YELLOW: // PD13
        LED_PORT->MODER &= ~(3U << (13*2));
        LED_PORT->MODER |= LED_YELLOW_MODE_BIT;
        if (this->state == LED_ON)     { LED_PORT->ODR |=  LED_PIN_YELLOW; }
        else if (this->state == LED_TOGGLE) { LED_PORT->ODR ^=  LED_PIN_YELLOW; }
        else                           { LED_PORT->ODR &= ~LED_PIN_YELLOW; }
        break;
    case BLUE:   // PD15
        LED_PORT->MODER &= ~(3U << (15*2));
        LED_PORT->MODER |= LED_BLUE_MODE_BIT;
        if (this->state == LED_ON)     { LED_PORT->ODR |=  LED_PIN_BLUE; }
        else if (this->state == LED_TOGGLE) { LED_PORT->ODR ^=  LED_PIN_BLUE; }
        else                           { LED_PORT->ODR &= ~LED_PIN_BLUE; }
        break;
    default:
        break;
    }
}

LEDState_Type LED::getState(USART *usart) const {
    // Read actual pin state from ODR so ON/OFF is accurate even when stored state is TOGGLE
    uint32_t pin_on;
    switch (this->color) {
        case GREEN:  pin_on = LED_PORT->ODR & LED_PIN_GREEN;  break;
        case YELLOW: pin_on = LED_PORT->ODR & LED_PIN_YELLOW; break;
        case RED:    pin_on = LED_PORT->ODR & LED_PIN_RED;    break;
        case BLUE:   pin_on = LED_PORT->ODR & LED_PIN_BLUE;   break;
        default:     pin_on = 0;                              break;
    }
    usart->USART_WriteString("--- LED Status ---\r\nState: ");
    usart->USART_WriteString(pin_on ? "ON" : "OFF");
    usart->USART_WriteString("\r\nColor: ");
    usart->USART_WriteString(LEDColorToString(this->color));
    usart->USART_WriteString("\r\n\r\n");
    return this->state;
}

// Default virtual implementations (accept USART pointer)
void LED::runDiagnostics(USART *usart) {
    (void)usart; // placeholder
}

uint32_t LED::computeEfficiency(USART *usart) const {
    (void)usart; // placeholder
    return 0U;
}
