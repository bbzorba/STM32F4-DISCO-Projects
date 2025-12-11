#include "../inc/tsl2591_light.h"

// Function prototypes
void delay(volatile uint32_t count);


//main function
int main(void) {
    // Initialize I2C1 in standard mode (100kHz)
    USART_HandleType usart;
    USART_constructor(&usart, USART_2, TX_ONLY, __115200);

    I2C_HandleType tsl2591;
    TSL2591_Init(&tsl2591, I2C_2, I2C_STANDARD_MODE);

    USART_WriteString(&usart, "Light Sensor Measurement Mode.\r\n");

    I2C_ScanAddresses(&tsl2591); // Scan for I2C devices
    
    // Initialize and check TSL2591 sensor
    if (!TSL2591_Begin(&tsl2591)) {
        USART_WriteString(&usart, "Error: TSL2591 sensor not detected!\r\n");
        while(1) {
            delay(1000000); // Blink or hold error state
        }
    }
    
    USART_WriteString(&usart, "TSL2591 sensor initialized successfully.\r\n");

    while (1) {
        uint32_t light_data = TSL2591_ReadLightData(&tsl2591, TSL2591_ADDR);
        
        if (light_data == 0xFFFFFFFF) {
            USART_WriteString(&usart, "Error reading sensor data!\r\n");
        } else {
            // Extract individual channels
            uint16_t ch0 = light_data & 0xFFFF;        // Full spectrum (visible + IR)
            uint16_t ch1 = (light_data >> 16) & 0xFFFF; // IR only
            
            // Output light data via USART
            char msg[128];
            snprintf(msg, sizeof(msg), "Light Data - CH0 (Full): %u, CH1 (IR): %u, Combined: %lu\r\n", 
                    ch0, ch1, light_data);
            USART_WriteString(&usart, msg);
        }

        // Process light_data as needed
        delay(800000); // ~50ms at 16 MHz
    }
    
}


void delay(volatile uint32_t count) {
    while(count--) {
        __asm__("nop");
    }
}
