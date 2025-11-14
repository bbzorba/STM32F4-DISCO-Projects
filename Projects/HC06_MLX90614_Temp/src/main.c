#include "hc06_mlx90614_temp.h"

// Function prototypes
void delay(volatile uint32_t count);

//main function
int main(void) {
    // Initialize I2C1 in standard mode (100kHz)
    HC06 hc06;
    HC06_Init(&hc06, USART_2, TX_ONLY, __9600);
    MLX90614_Init();

    while (1) {
        char msg[96];
        int m = snprintf(msg, sizeof(msg), "\r\nTemperature Measurement Mode \r\n HC06 Connected on %s\r\n", HC06_GetPortName(&hc06));
        if (m > 0) {
            if (m > (int)sizeof(msg)) m = (int)sizeof(msg);
        HC06_SendData(&hc06, (const uint8_t*)msg, (size_t)m);
        }

        send_obj_temp(&hc06);
        delay(1000);

        send_amb_temp(&hc06);
        delay(1000);
    }
}


void delay(volatile uint32_t count) {
    while(count--) {
        __asm__("nop");
    }
}
