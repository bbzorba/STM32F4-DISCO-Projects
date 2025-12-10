// Minimal, register-level LIS302DL SPI example adapted to STM32F407
// Pins: CS=PE3, SCK=PA5, MOSI=PA7, MISO=PA6

#include <stdint.h>
#include <stdio.h>
#include "../inc/lis302dl.h"

//User-defined variables
int16_t x_final, y_final, z_final;

int main(void){

	GPIO spiPins = LIS_SPI_Pins_Init();
	GPIO csPin = LIS_CS_Pin_Init();
	GPIO leds = LEDS_Init();

	// Initialize SPI device using SPI driver
	SPI spiDev(SPI_1, SPI1_PORTA, SPI_MODE_MASTER, SPI_BAUDRATE_DIV256, SPI_DIRECTION_2LINES, SPI_CLOCK_POL_HIGH_PHASE_2EDGE);
    
    // Bind LIS302DL object to SPI and CS
    LIS302DL lis(&spiDev, &csPin);
    lis.Init();

	// Probe WHO_AM_I and try mode fallback if not 0x3B
	lis.ModeFallback(0x3B);

	// Init USART2 (TX only) to stream accelerometer values
	USART usart(USART_2, TX_ONLY, __115200);
	usart.USART_WriteString("LIS302DL SPI stream on USART2 @115200\r\n");
	// Ensure CS high when idle
	csPin.GPIO_SetBit(3u);
	while(1){

		// Use the Convert_To_Val function to convert raw data into actual data
		x_final = lis.Read(OUT_X) + X_OFFSET;
		y_final = lis.Read(OUT_Y);
		z_final = lis.Read(OUT_Z);

		// Switch on LEDs based on the acceleration value obtained
		if ((x_final != 0) && (y_final != 0)){
			if (x_final > THRESH_HIGH){
				leds.GPIO_SetPin(GPIO_PIN_14);
				leds.GPIO_ResetPin(GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_15);
			}
			else if (x_final < THRESH_LOW){
				leds.GPIO_SetPin(GPIO_PIN_12);
				leds.GPIO_ResetPin(GPIO_PIN_14 | GPIO_PIN_13 | GPIO_PIN_15);
			}
			if (y_final > THRESH_HIGH){
				leds.GPIO_SetPin(GPIO_PIN_13);
				leds.GPIO_ResetPin(GPIO_PIN_12 | GPIO_PIN_14 | GPIO_PIN_15);
			}
			else if (y_final < THRESH_LOW ){
				leds.GPIO_SetPin(GPIO_PIN_15);
				leds.GPIO_ResetPin(GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14);
			}
		}
		else
            leds.GPIO_ResetPin(GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);
		// Also stream over UART2
		{
			char msg[96];
			snprintf(msg, sizeof(msg), "values:%6d,%6d,%6d\r\n", x_final, y_final, z_final);
			usart.USART_WriteString(msg);
		}

		// Give a finite delay
		TIM4_ms_Delay(20);
	}
}