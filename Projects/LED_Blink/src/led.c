#include "led.h"

void LED_constructor(LED_Type* const led, LEDColor_Type _color, LEDState_Type _state){
    led->color = _color;
    led->state = _state;
}

void LED_setState(LED_Type* const led, LEDState_Type _state){
    led->state = _state;

    printf("%d LED state is set to: %d\n\r", led->color, led->state);

/*     switch (led->color)
    {
    case RED:
        printf("Red LED set to %d\n\r", led->state);
        GPIOD->ODR |= (1 << 12);
        break;
    case GREEN:
        printf("Green LED set to %d\n\r", led->state);
        GPIOD->ODR |= (1 << 13);
        break;
    case YELLOW:
        printf("Yellow LED set to %d\n\r", led->state);
        GPIOD->ODR |= (1 << 14);
        break;
    case BLUE:
        printf("Blue LED set to %d\n\r", led->state);
        GPIOD->ODR |= (1 << 15);
        break;
    default:
        break;
    } */
}

LEDState_Type LED_getState(const LED_Type* const led){

    
    printf("%d LED state is currently: %d\n\r", led->color, led->state);

    return led->state;
}
