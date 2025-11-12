#include "mlx90614_temp.h"


// Function prototypes
void delay(volatile uint32_t count);


//main function
int main(void) {
    // Initialize I2C1 in standard mode (100kHz)
    MLX90614_Init();

    while (1) {
        int temperature = read_temp(MLX90614_ADDR);
        delay(1000);
    }
}


void delay(volatile uint32_t count) {
    while(count--) {
        __asm__("nop");
    }
}
