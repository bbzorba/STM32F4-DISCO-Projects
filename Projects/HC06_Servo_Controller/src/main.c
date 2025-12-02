#include "../inc/hc06_servo_controller.h"

// Function prototypes
void delay(volatile uint32_t count);

// Single-byte RX buffer
static uint8_t rx;

//main function
int main(void) {

    // Initialize servo motor object
    Servo servo_PE5;
    
    GPIO_InitTypeDef servo_gpio_init;
    servo_gpio_init.Pin = (1U << 5); // PE5
    servo_gpio_init.Mode = GPIO_MODE_AF_PP;
    servo_gpio_init.Pull = GPIO_NOPULL;
    servo_gpio_init.Speed = GPIO_SPEED_MEDIUM;
    servo_gpio_init.Alternate = 3; // AF3 TIM9 CH1

    PWM_HandleType pwmHandle;
    servo_constructor(&servo_PE5, SERVO_180_TYPE, SERVO_DEFAULT_ANGLE, RCC, 5, 3, GPIO_E,  &servo_gpio_init, &pwmHandle, PWM_CHANNEL_1, HALF_DC, PWM_PRESCALER_1599U, 200, TIM_9);
    Servo_SetAngle(&servo_PE5, &pwmHandle, SERVO_DEFAULT_ANGLE);
    Servo_Start(&servo_PE5, &pwmHandle);
    
    // Initialize HC-06 Bluetooth module
    HC06 hc06;
    HC06_Init(&hc06, USART_3, RX_AND_TX, __9600);
    
    // Optional: announce ready over Bluetooth
    const uint8_t hello[] = "HC-06 ready. Send L/M/R\r\n";
    HC06_SendData(&hc06, hello, sizeof(hello) - 1);

    while (1) {
        // Blocking read for 1 byte
        HC06_ReceiveData(&hc06, &rx, 1);
        // Echo for debugging
        const uint8_t crlf[2] = {'\r','\n'};
        HC06_SendData(&hc06, &rx, 1);
        HC06_SendData(&hc06, crlf, 2);
        move_servo_to_direction(&servo_PE5, &pwmHandle, rx);
    }
}

void delay(volatile uint32_t count) {
    while(count--) {
        __asm__("nop");
    }
}
