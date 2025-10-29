#include "hc06_servo_controller.h"

// Function prototypes
void delay(volatile uint32_t count);

// Single-byte RX buffer
static uint8_t rx;

//main function
int main(void) {

    // Initialize servo motor object
    Servo servo_PE5(SERVO_180_TYPE, SERVO_DEFAULT_ANGLE, TIM_9, GPIO_E, RCC, 5, 3, GPIOE_EN);
    servo_PE5.SetAngle(SERVO_DEFAULT_ANGLE);
    servo_PE5.Start();
    
    // Initialize HC-06 Bluetooth module
    HC06 hc06(RX_AND_TX, __9600);
    
    // Optional: announce ready over Bluetooth
    const uint8_t hello[] = "HC-06 ready. Send L/M/R\r\n";
    hc06.SendData(hello, sizeof(hello) - 1);

    while (1) {
        // Blocking read for 1 byte
        hc06.ReceiveData(&rx, 1);
        // Echo for debugging
        const uint8_t crlf[2] = {'\r','\n'};
        hc06.SendData(&rx, 1);
        hc06.SendData(crlf, 2);
        move_servo_to_direction(&servo_PE5, rx);
    }
}

void delay(volatile uint32_t count) {
    while(count--) {
        __asm__("nop");
    }
}
