#include "mlx90614_temp.h"


// Function prototypes
void delay(volatile uint32_t count);


//main function
int main(void) {
    // Initialize I2C1 in standard mode (100kHz)
    MLX90614_Init();

    while (1) {
        int obj_temp = read_obj_temp(MLX90614_ADDR);
        UART_SendString("Object Temperature: ");
        UART_SendInt(obj_temp);
        UART_SendString("\r\n");
        delay(1000);
        int amb_temp = read_amb_temp(MLX90614_ADDR);
        UART_SendString("Ambient Temperature: ");
        UART_SendInt(amb_temp);
        UART_SendString("\r\n");
        delay(1000);
    }
}


void delay(volatile uint32_t count) {
    while(count--) {
        __asm__("nop");
    }
}
