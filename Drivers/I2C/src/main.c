#include "i2c.h"

#define i2c_addr 0x76

// Function prototypes
void delay(volatile uint32_t count);


//main function
int main(void) {
    // Initialize I2C1 in standard mode (100kHz)
    I2C_Init(I2C_STANDARD_MODE);

    while (1) {
        // Issue a read-address phase to the target address
        (void)read_I2C_address(i2c_addr);
        delay(1000);
    }
}


void delay(volatile uint32_t count) {
    while(count--) {
        __asm__("nop");
    }
}
