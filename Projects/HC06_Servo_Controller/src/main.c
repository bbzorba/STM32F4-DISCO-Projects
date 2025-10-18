#include "hc06_servo_controller.h"

// Function prototypes
void delay(volatile uint32_t count);

// Single-byte RX buffer
static uint8_t rx;

//main function
int main(void) {

    // Initialize servo motor object
    Servo servo_PE5;
    servo_constructor(&servo_PE5, SERVO_180_TYPE, SERVO_DEFAULT_ANGLE, TIM_9, GPIO_E, RCC, 5, 3, GPIOE_EN);
    Servo_SetAngle(&servo_PE5, SERVO_DEFAULT_ANGLE);
    Servo_Start(&servo_PE5);
    
    // Initialize HC-06 Bluetooth module
    HC06 hc06;
    HC06_Init(&hc06, RX_AND_TX, __9600);
    
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
        move_servo_to_direction(&servo_PE5, rx);
    }
}

void delay(volatile uint32_t count) {
    while(count--) {
        __asm__("nop");
    }
}
