#include "i2c.h"

#define i2c_addr 0x76

// Function prototypes
void delay(volatile uint32_t count);


//main function
int main(void) {
    // Initialize I2C1 in standard mode (100kHz)
    I2C_Init(I2C_STANDARD_MODE);

    while (1) {
        // Simple visible transaction: START -> address write -> dummy byte -> STOP
        I2C_Start();
        if (!I2C_SendAddress(i2c_addr, 0)) { // write
            // NACK received, stop and retry after a short delay
            I2C_Stop();
            delay(100000);
            continue;
        }
        I2C_Write(0x15);              // dummy data
        I2C_Stop();
        delay(200000);                 // brief gap between transactions for visibility
    }
}

void delay(volatile uint32_t count) {
    while(count--) {
        __asm__("nop");
    }
}
