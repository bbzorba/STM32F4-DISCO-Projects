// LIS302DL SPI example
// Pins: CS=PE3, SCK=PA5, MOSI=PA7, MISO=PA6
#include "../inc/lis302dl.h"

int16_t x_final, y_final, z_final;

//User-defined variables

int main(void){
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_HandleTypeDef CS_Handle;
    CS_Handle.regs = GPIO_E;
    CS_Handle.init = &GPIO_InitStruct;

	LIS_GPIO_Init(&CS_Handle);
	LIS_SPI1_Init();
	LIS_Init(&CS_Handle);

	// Probe WHO_AM_I and try mode fallback if not 0x3B
	uint8_t who = LIS_WhoAmI(&CS_Handle);
	if (who != 0x3B) {
		// try mode 0
		SPI1_SetMode(0);
		TIM4_ms_Delay(1);
		who = LIS_WhoAmI(&CS_Handle);
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
	GPIO_SetBit(&CS_Handle, 3u);
	while(1){
		// Use the Convert_To_Val function to convert raw data into actual data
		x_final = Convert_To_Val(SPI_Receive(&CS_Handle, OUT_X)) + X_OFFSET;
		y_final = Convert_To_Val(SPI_Receive(&CS_Handle, OUT_Y));
		z_final = Convert_To_Val(SPI_Receive(&CS_Handle, OUT_Z));

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
			snprintf(msg, sizeof(msg), "raw:%3u,%3u,%3u conv:%6d,%6d,%6d\r\n", SPI_Receive(&CS_Handle, OUT_X), SPI_Receive(&CS_Handle, OUT_Y), SPI_Receive(&CS_Handle, OUT_Z), x_final, y_final, z_final);
			USART_WriteString(&usart, msg);
		}

		// Give a finite delay
		TIM4_ms_Delay(20);
	}
}