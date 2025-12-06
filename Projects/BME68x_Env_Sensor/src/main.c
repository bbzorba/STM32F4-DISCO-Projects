#include "../inc/bme68x_env_sensor.h"

// Function prototypes
void delay(volatile uint32_t count);


//main function
int main(void) {

    USART_HandleType usart;
    USART_constructor(&usart, USART_2, TX_ONLY, __115200);

    USART_WriteString(&usart, "BME68x Environmental Sensor Test\r\n");

    // Initialize I2C2 in standard mode (100kHz)
    I2C_HandleType bme68x;
    I2C_Init(&bme68x, I2C_1, I2C_STANDARD_MODE);
    
    // Power-up delay for sensor
    delay(500000);

    // Scan for I2C devices
    USART_WriteString(&usart, "Scanning I2C2 bus for devices...\r\n");
    int found_devices = 0;
    char debug_msg[128];
    
    for (uint8_t address = 1; address < 128; ++address) {
        I2C_Start(&bme68x);
        int ack = I2C_SendAddress(&bme68x, address, 0);
        I2C_Stop(&bme68x);
        if (ack) {
            snprintf(debug_msg, sizeof(debug_msg), "Device found at address 0x%02X (%d)\r\n", address, address);
            USART_WriteString(&usart, debug_msg);
            found_devices++;
            
            // Test if it's BME68x
            if (address == 0x76 || address == 0x77) {
                uint8_t chip_id = bme68x_read_u8_safe(&bme68x, address, 0xD0);
                snprintf(debug_msg, sizeof(debug_msg), "  -> Chip ID: 0x%02X ", chip_id);
                USART_WriteString(&usart, debug_msg);
                if (chip_id == 0x61) {
                    USART_WriteString(&usart, "(BME680/BME688 detected!)\r\n");
                } else {
                    USART_WriteString(&usart, "(Unknown device)\r\n");
                }
            }
        }
        delay(5000);
    }
    
    if (found_devices == 0) {
        USART_WriteString(&usart, "No I2C devices found!\r\n");
        USART_WriteString(&usart, "Check wiring: VIN->3.3V, GND->GND, SDA->PB11, SCL->PB10\r\n");
        while(1) {
            delay(1000000); // Stop execution - no point continuing without sensor
        }
    }

    snprintf(debug_msg, sizeof(debug_msg), "Total devices found: %d\r\n", found_devices);
    USART_WriteString(&usart, debug_msg);

    // Try to initialize BME68x at detected addresses
    uint8_t bme_detected = 0;
    uint8_t working_address = 0;
    uint8_t bme_addresses[] = {0x76, 0x77};
    
    for (int i = 0; i < 2; i++) {
        I2C_Start(&bme68x);
        int ack = I2C_SendAddress(&bme68x, bme_addresses[i], 0);
        I2C_Stop(&bme68x);
        
        if (ack) {
            uint8_t chip_id = bme68x_read_u8_safe(&bme68x, bme_addresses[i], 0xD0);
            if (chip_id == 0x61) {
                working_address = bme_addresses[i];
                bme_detected = 1;
                snprintf(debug_msg, sizeof(debug_msg), "BME68x initialized at address 0x%02X\r\n", working_address);
                USART_WriteString(&usart, debug_msg);
                break;
            }
        }
    }
    
    if (!bme_detected) {
        USART_WriteString(&usart, "No BME68x sensor found at expected addresses!\r\n");
        while(1) {
            delay(1000000); // Stop execution
        }
    }

    // Initialize the sensor
    BME68x_Init(&bme68x);
    USART_WriteString(&usart, "Starting temperature measurements...\r\n\r\n");

    // Main sensor reading loop
    while (1) {
        int temperature = read_temp(&bme68x, working_address);
        int raw_temp = read_raw_temp(&bme68x, working_address);
        
        snprintf(debug_msg, sizeof(debug_msg), "Temperature: %d C, Raw ADC: %d\r\n", temperature, raw_temp);
        USART_WriteString(&usart, debug_msg);
        
        delay(1000000); // ~60ms delay
    }
}


void delay(volatile uint32_t count) {
    while(count--) {
        __asm__("nop");
    }
}
