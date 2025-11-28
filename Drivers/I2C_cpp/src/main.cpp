#include "../inc/i2c.h"

#define i2c_addr 0x76

// Function prototypes
void delay(volatile uint32_t count);

//main function
int main(void) {

    // Initialize I2C3 in standard mode (100kHz)
    I2C i2c3(I2C_3, I2C_STANDARD_MODE);

    while (1) {
        // Simple visible transaction: START -> address write -> dummy byte -> STOP
        i2c3.I2C_Start();
        if (!i2c3.I2C_SendAddress(i2c_addr, 0)) { // write
            // NACK received, stop and retry after a short delay
            i2c3.I2C_Stop();
            delay(100000);
            continue;
        }
        i2c3.I2C_Write(0x15);              // dummy data
        i2c3.I2C_Stop();
        delay(200000);                 // brief gap between transactions for visibility
    }
}

void delay(volatile uint32_t count) {
    while(count--) {
        __asm__("nop");
    }
}