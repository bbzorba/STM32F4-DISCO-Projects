#include "hc06_servo_controller.h"

// Function prototypes
void delay(volatile uint32_t count);

//main function
int main(void) {
    Servo_GPIO_Init(GPIO_E);
    Servo_PWM_Init(TIM_9, RCC);
    Servo_Start(TIM_9);
    USART_2_Init(RX_AND_TX, __9600);

    // Optional: announce ready over Bluetooth
    const char hello[] = "HC-06 ready. Send l/m/r\r\n";
    for (unsigned i = 0; i < sizeof(hello)-1; ++i) USART_2_Write(hello[i]);

    while (1) {
        uint8_t command = USART_2_Read();
        // Echo for debugging
        USART_2_Write(command);
        USART_2_Write('\r');
        USART_2_Write('\n');
        move_servo_to_direction(command);
    }
}

void delay(volatile uint32_t count) {
    while(count--) {
        __asm__("nop");
    }
}
