#include "hc06_bme68x_env_sensor.h"

// Function prototypes
void delay(volatile uint32_t count);

//main function
int main(void) {
    // Initialize I2C1 in standard mode (100kHz)
    HC06 hc06;
    HC06_Init(&hc06, USART_2, RX_AND_TX, __9600);
    BME68x_Init();

    // Replace stale scan_i2c_bus() call with hc06_scan_i2c via HC06 helper
    hc06_scan_i2c(&hc06); // Scan I2C bus and report devices over Bluetooth

    while (1) {
        char msg[96];
        int m = snprintf(msg, sizeof(msg), "\r\nEnvironmental Sensor Measurement \r\n HC06 Connected on %s\r\n", HC06_GetPortName(&hc06));
        if (m > 0) {
            if (m > (int)sizeof(msg)) m = (int)sizeof(msg);
        HC06_SendData(&hc06, (const uint8_t*)msg, (size_t)m);
        }

        // Debug once per loop (you can comment out after confirming values)
        //send_bme68x_debug(&hc06);

        send_temp(&hc06);
        delay(1000);
    }
}


void delay(volatile uint32_t count) {
    while(count--) {
        __asm__("nop");
    }
}
