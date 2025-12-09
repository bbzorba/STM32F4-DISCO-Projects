// Minimal, register-level LIS302DL SPI example adapted to STM32F407
// Pins: CS=PE3, SCK=PA5, MOSI=PA7, MISO=PA6

#include <stdint.h>
#include <stdio.h>
// Use project drivers via unified header
#include "../inc/lis302dl.h"

//User-defined variables
int16_t x_final, y_final, z_final;

int main(void){

	LIS_GPIO_Init();
	// Initialize SPI using project SPI driver, then apply LIS-specific tweaks
	SPI_HandleType spi;
	SPI_Init(&spi, SPI_1, SPI1_PORTA, SPI_MODE_MASTER, SPI_BAUDRATE_DIV256, SPI_DIRECTION_2LINES);
	// Ensure 8-bit, MSB-first, mode 3 (CPOL=1, CPHA=1), and disable CRC
	SPI_1->CR1 &= ~SPI_CR1_LSBFIRST;
	SPI_1->CR1 |= (SPI_CR1_CPOL | SPI_CR1_CPHA);
	SPI_1->CR1 &= ~SPI_CR1_CRCEN;
	LIS_Init();

	// Probe WHO_AM_I and try mode fallback if not 0x3B
	uint8_t who = LIS_WhoAmI();
	if (who != 0x3B) {
		// try mode 0
		SPI1_SetMode(0);
		TIM4_ms_Delay(1);
		who = LIS_WhoAmI();
		if (who != 0x3B) {
			// restore mode 3 as default
			SPI1_SetMode(3);
		}
	}

	// Init USART2 (TX only) to stream accelerometer values
	USART_HandleType usart;
	USART_constructor(&usart, USART_2, TX_ONLY, __115200);
	USART_WriteString(&usart, "LIS302DL SPI stream on USART2 @115200\r\n");
	// Ensure CS high when idle
	GPIO_E->BSRR = (1u << 3u);
	while(1){

		// Use the Convert_To_Val function to convert raw data into actual data
		x_final = LIS_Read(OUT_X) + X_OFFSET;
		y_final = LIS_Read(OUT_Y);
		z_final = LIS_Read(OUT_Z);

		// Switch on LEDs based on the acceleration value obtained
		if ((x_final != 0) && (y_final != 0)){
			if (x_final > THRESH_HIGH){
				GPIO_D->ODR |= (1u<<14);
				GPIO_D->ODR &= ~((1u<<12) | (1u<<13) | (1u<<15));
			}
			else if (x_final < THRESH_LOW){
				GPIO_D->ODR |= (1u<<12);
				GPIO_D->ODR &= ~((1u<<14) | (1u<<13) | (1u<<15));
			}
			if (y_final > THRESH_HIGH){
				GPIO_D->ODR |= (1u<<13);
				GPIO_D->ODR &= ~((1u<<12) | (1u<<14) | (1u<<15));
			}
			else if (y_final < THRESH_LOW ){
				GPIO_D->ODR |= (1u<<15);
				GPIO_D->ODR &= ~((1u<<12) | (1u<<13) | (1u<<14));
			}
		}
		else
			GPIO_D->ODR &= ~((1u<<12) | (1u<<13) | (1u<<14) | (1u<<15));

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