#include "i2c.h"

#define i2c_addr 0x76

// Function prototypes
void delay(volatile uint32_t count);


//main function
int main(void) {
    I2C1_Init();
    
    while (1) {
        I2C1_Read(i2c_addr);
        delay(1000);
    }
}


void delay(volatile uint32_t count) {
    while(count--) {
        __asm__("nop");
    }
}
