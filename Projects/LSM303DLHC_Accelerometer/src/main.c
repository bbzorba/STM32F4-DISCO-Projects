#include "../inc/lsm303dlhc_accel.h"

// Function prototypes
void delay(volatile uint32_t count);

//main function
int main(void) {

    USART_HandleType usart;
    USART_constructor(&usart, USART_2, TX_ONLY, __115200);

    USART_WriteString(&usart, "LSM303DLHC Accelerometer Test\r\n");

    // Initialize I2C1 in standard mode (100kHz) - onboard accelerometer uses I2C1
    I2C_HandleType lsm303;
    I2C_Init(&lsm303, I2C_1, I2C_STANDARD_MODE);
    
    // Power-up delay
    delay(500000);

    // Scan for I2C devices first
    USART_WriteString(&usart, "Scanning I2C1 bus for devices...\r\n");
    int found_devices = 0;
    char debug_msg[128];
    
    for (uint8_t address = 1; address < 128; ++address) {
        I2C_Start(&lsm303);
        int ack = I2C_SendAddress(&lsm303, address, 0);
        I2C_Stop(&lsm303);
        if (ack) {
            snprintf(debug_msg, sizeof(debug_msg), "Device found at address 0x%02X (%d)\r\n", address, address);
            USART_WriteString(&usart, debug_msg);
            found_devices++;
            
            // Test if it's LSM303DLHC accelerometer
            if (address == 0x19) {
                USART_WriteString(&usart, "  -> LSM303DLHC Accelerometer detected!\r\n");
            }
            // Check for magnetometer part at 0x1E
            if (address == 0x1E) {
                USART_WriteString(&usart, "  -> LSM303DLHC Magnetometer detected!\r\n");
            }
        }
        delay(5000);
    }
    
    if (found_devices == 0) {
        USART_WriteString(&usart, "No I2C devices found!\r\n");
        USART_WriteString(&usart, "Check if using correct I2C port (should be I2C1 for onboard accelerometer)\r\n");
        while(1) {
            delay(1000000);
        }
    }

    snprintf(debug_msg, sizeof(debug_msg), "Total devices found: %d\r\n", found_devices);
    USART_WriteString(&usart, debug_msg);

    // Check if accelerometer is found at expected address
    I2C_Start(&lsm303);
    int accel_present = I2C_SendAddress(&lsm303, LSM303DLHC_ADDR, 0);
    I2C_Stop(&lsm303);
    
    if (!accel_present) {
        snprintf(debug_msg, sizeof(debug_msg), "LSM303DLHC not found at address 0x%02X!\r\n", LSM303DLHC_ADDR);
        USART_WriteString(&usart, debug_msg);
        while(1) {
            delay(1000000);
        }
    }

    // Initialize the sensor
    LSM303DLHC_Init(&lsm303);
    snprintf(debug_msg, sizeof(debug_msg), "LSM303DLHC initialized at address 0x%02X\r\n", LSM303DLHC_ADDR);
    USART_WriteString(&usart, debug_msg);
    USART_WriteString(&usart, "Starting accelerometer measurements...\r\n\r\n");

    // Main sensor reading loop
    while (1) {
        int accel_data = read_accel(&lsm303, LSM303DLHC_ADDR);
        if (accel_data == 0xFFFF) {
            USART_WriteString(&usart, "Error reading accelerometer data!\r\n");
        } else {
            char msg[128];
            snprintf(msg, sizeof(msg), "Acceleration Magnitude: %d\r\n", accel_data);
            USART_WriteString(&usart, msg);
        }
        delay(1000000); // ~60ms delay
    }
}

void delay(volatile uint32_t count) {
    while(count--) {
        __asm__("nop");
    }
}
