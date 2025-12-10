// Minimal, register-level LIS302DL SPI example adapted to STM32F407
// Pins: CS=PE3, SCK=PA5, MOSI=PA7, MISO=PA6

#include <stdint.h>
#include <stdio.h>
#include "../inc/lis302dl.h"

//User-defined variables
int16_t x_final, y_final, z_final;

int main(void){

    // Configure pins (SPI, CS, LEDs)
    GPIO_HandleTypeDef* spi_pin_handle = LIS_SPI_Pins_Init();
    GPIO_HandleTypeDef* cs_pin_handle = LIS_CS_Pin_Init();
    GPIO_HandleTypeDef* leds_handle = LEDS_Init();

	// Initialize SPI device using SPI driver
	SPI_HandleType spi1;
	SPI_Init(&spi1, SPI_1, SPI1_PORTA, SPI_MODE_MASTER, SPI_BAUDRATE_DIV256, SPI_DIRECTION_2LINES, SPI_CLOCK_POL_HIGH_PHASE_2EDGE);
    
    // Initialize the LIS302DL accelerometer (bind SPI and CS)
	LIS302DL_HandleType lis302;
	LIS302DL_constructor(&lis302, &spi1, cs_pin_handle);

	// Probe WHO_AM_I and try mode fallback if not 0x3B
	mode_fallback(&lis302, 0x3B);

	// Init USART2 (TX only) to stream accelerometer values
	USART_HandleType usart;
	USART_constructor(&usart, USART_2, TX_ONLY, __115200);
	USART_WriteString(&usart, "LIS302DL SPI stream on USART2 @115200\r\n");
	// Ensure CS high when idle
	GPIO_SetBit(cs_pin_handle, 3u);
	while(1){

		// Use the Convert_To_Val function to convert raw data into actual data
		x_final = LIS_Read(&lis302, OUT_X) + X_OFFSET;
		y_final = LIS_Read(&lis302, OUT_Y);
		z_final = LIS_Read(&lis302, OUT_Z);

		// Switch on LEDs based on the acceleration value obtained
		if ((x_final != 0) && (y_final != 0)){
			if (x_final > THRESH_HIGH){
				GPIO_SetPin(leds_handle, GPIO_PIN_14);
				GPIO_ResetPin(leds_handle, GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_15);
			}
			else if (x_final < THRESH_LOW){
				GPIO_SetPin(leds_handle, GPIO_PIN_12);
				GPIO_ResetPin(leds_handle, GPIO_PIN_14 | GPIO_PIN_13 | GPIO_PIN_15);
			}
			if (y_final > THRESH_HIGH){
				GPIO_SetPin(leds_handle, GPIO_PIN_13);
				GPIO_ResetPin(leds_handle, GPIO_PIN_12 | GPIO_PIN_14 | GPIO_PIN_15);
			}
			else if (y_final < THRESH_LOW ){
				GPIO_SetPin(leds_handle, GPIO_PIN_15);
				GPIO_ResetPin(leds_handle, GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14);
			}
		}
		else
            GPIO_ResetPin(leds_handle, GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);

		// Also stream over UART2
		{
			char msg[96];
			snprintf(msg, sizeof(msg), "values:%6d,%6d,%6d\r\n", x_final, y_final, z_final);
			USART_WriteString(&usart, msg);
		}

		// Give a finite delay
		TIM4_ms_Delay(20);
	}
}