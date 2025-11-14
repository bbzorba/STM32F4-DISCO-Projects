#include "mlx90614_temp.h"


// Function prototypes
void delay(volatile uint32_t count);


//main function
int main(void) {
    // Initialize I2C1 in standard mode (100kHz)
    MLX90614_Init();

    // Initialize UART
    USART_TypeDef usart;
    USART_Init(&usart, USART_1, RX_AND_TX, __115200);

    while (1) {
        char obj_temp = (char)(read_obj_temp(MLX90614_ADDR));
        USART_WriteString(&usart, "Object Temperature: ");
        USART_WriteChar(&usart, obj_temp);
        USART_WriteString(&usart, "\r\n");
        delay(1000);
        char amb_temp = (char)(read_amb_temp(MLX90614_ADDR));
        USART_WriteString(&usart, "Ambient Temperature: ");
        USART_WriteChar(&usart, amb_temp);
        USART_WriteString(&usart, "\r\n");
        delay(1000);
    }
}


void delay(volatile uint32_t count) {
    while(count--) {
        __asm__("nop");
    }
}
