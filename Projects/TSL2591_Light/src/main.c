#include "../inc/tsl2591_light.h"

// Function prototypes
void delay(volatile uint32_t count);


//main function
int main(void) {
    // Initialize I2C1 in standard mode (100kHz)
    USART_HandleType usart;
    USART_constructor(&usart, USART_2, TX_ONLY, __115200);

    I2C_HandleType tsl2591;
    TSL2591_Init(&tsl2591, I2C_1, I2C_STANDARD_MODE);

    USART_WriteString(&usart, "Light Sensor Measurement Mode.\r\n");

    while (1) {
        uint32_t light_data = TSL2591_ReadLightData(&tsl2591, TSL2591_ADDR);
        // Output light data via USART
        char msg[64];
        snprintf(msg, sizeof(msg), "Light Data: %lu\r\n", light_data);
        USART_WriteString(&usart, msg);

        // Process light_data as needed
        delay(800000); // ~50ms at 16 MHz
    }
    
}


void delay(volatile uint32_t count) {
    while(count--) {
        __asm__("nop");
    }
}
