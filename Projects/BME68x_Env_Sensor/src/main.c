#include "bme68x_env_sensor.h"


// Function prototypes
void delay(volatile uint32_t count);


//main function
int main(void) {
    // Initialize I2C1 in standard mode (100kHz)
    BME68x_Init();

    while (1) {
        int temperature = read_temp(BME68X_ADDR);
        int density = read_gas_density(BME68X_ADDR);
        int humidity = read_humidity(BME68X_ADDR);
        printf("Temp: %d C, Density: %d hPa, Humidity: %d %%\r\n", temperature, density, humidity);
        delay(1000);
    }
}


void delay(volatile uint32_t count) {
    while(count--) {
        __asm__("nop");
    }
}
