// Minimal, register-level LIS302DL SPI example adapted to STM32F407
// Pins: CS=PE3, SCK=PA5, MOSI=PA7, MISO=PA6

#include <stdint.h>
#include <stdio.h>
// Use project drivers via unified header
#include "../inc/lis302dl.h"

// Definitions moved to lis302dl.h

//User-defined Function Declarations
void LIS_GPIO_Init(void);
void LIS_SPI1_Init(void);
uint16_t SPI_Transmit(uint8_t data);
uint16_t SPI_Receive(uint8_t addr);
void LIS_Init(void);
void LIS_Write(uint8_t addr, uint8_t data);
void LIS_Read(void);
int16_t Convert_To_Val(uint16_t val);
void TIM4_ms_Delay(uint16_t delay);

//User-defined variables
uint16_t x,y,z;
int16_t x_final, y_final, z_final;
uint16_t rxd,rxdf;

// Use project GPIO struct (single 32-bit BSRR)
static inline void gpio_set_bit(GPIO_ManualTypeDef *regs, uint32_t pin) { regs->BSRR = (1u << pin); }
static inline void gpio_reset_bit(GPIO_ManualTypeDef *regs, uint32_t pin) { regs->BSRR = (1u << (pin + 16u)); }

void LIS_GPIO_Init(){
	// Enable GPIOA clock
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

	// Configuring PA5, PA6, PA7 in alternate function mode (10b)
	GPIO_A->MODER &= ~((3u<<(5*2)) | (3u<<(6*2)) | (3u<<(7*2)));
	GPIO_A->MODER |=  ((2u<<(5*2)) | (2u<<(6*2)) | (2u<<(7*2)));

	// Select AF5 for SPI on PA5, PA6, PA7
	GPIO_A->AFR[0] &= ~((0xFu<<(4*5)) | (0xFu<<(4*6)) | (0xFu<<(4*7)));
	GPIO_A->AFR[0] |=  ((5u<<(4*5)) | (5u<<(4*6)) | (5u<<(4*7)));

	// Enable GPIOE clock
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;

	// Since PE3 is CS, configure in Output Mode (01b)
	GPIO_E->MODER &= ~(3u<<(3*2));
	GPIO_E->MODER |=  (1u<<(3*2));

	// Push-pull, no pull, very high speed for CS
	GPIO_E->OTYPER &= ~(1u<<3);
	GPIO_E->PUPDR &= ~(3u<<(3*2));
	GPIO_E->OSPEEDR |= (3u<<(3*2));

	// Deassert CS initially
	gpio_set_bit(GPIO_E, 3u);

	// Medium speed on PA5/6/7
	GPIO_A->OSPEEDR |= ((1u<<(5*2)) | (1u<<(6*2)) | (1u<<(7*2)));

	// Pull-down on PA5/6/7 (10b)
	GPIO_A->PUPDR &= ~((3u<<(5*2)) | (3u<<(6*2)) | (3u<<(7*2)));
	GPIO_A->PUPDR |=  ((2u<<(5*2)) | (2u<<(6*2)) | (2u<<(7*2)));

	// Enable clock for GPIOD and Configure PD12 in output mode
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	GPIO_D->MODER &= ~((3u<<(12*2)) | (3u<<(13*2)) | (3u<<(14*2)) | (3u<<(15*2)));
	GPIO_D->MODER |=  ((1u<<(12*2)) | (1u<<(13*2)) | (1u<<(14*2)) | (1u<<(15*2)));
}

void LIS_SPI1_Init(){
	// Enable SPI clock
	RCC->APB2ENR |= RCC_APB2ENR_SPI_1EN;

	// Select the Master Configuration
	SPI_1->CR1 |= SPI_CR1_MSTR;

	SPI_1->CR1 &= ~SPI_CR1_BIDIMODE;

	SPI_1->CR1 &= ~SPI_CR1_RXONLY;

	// Set the Data Frame Format (DFF) to '0' or 8-bit.
	SPI_1->CR1 &= ~SPI_CR1_DFF;

	// SSI and SSM bits in the SP1->CR1 register need to be set
	// to '1'
	SPI_1->CR1 |= (SPI_CR1_SSI | SPI_CR1_SSM);

	// Setting Baud Rate: f_PCLK/256 (slow and safe)
	SPI_1->CR1 &= ~SPI_CR1_BR;
	SPI_1->CR1 |= (SPI_CR1_BR_0 | SPI_CR1_BR_1 | SPI_CR1_BR_2);

	// Set the transmission to MSB First Mode
	SPI_1->CR1 &= ~SPI_CR1_LSBFIRST;

	// Configure CPOL and CPHA to 1 and 1 (SPI mode 3) required by LIS302DL
	SPI_1->CR1 |= (SPI_CR1_CPOL | SPI_CR1_CPHA);

	// Disable CRC (not used)
	SPI_1->CR1 &= ~SPI_CR1_CRCEN;

	// Enable SPI
	SPI_1->CR1 |= SPI_CR1_SPE;

	// Selecting Motorola Format
	SPI_1->CR2 = 0x0000;
}

uint16_t SPI_Transmit(uint8_t data){
	//  Wait until the TX buffer is empty, i.e. data is transmitted
	while(!((SPI_1->SR) & SPI_SR_TXE)){}
	// Load the data into the data register
	SPI_1->DR = data;

	while(!(SPI_1->SR & SPI_SR_RXNE)){}
	// If reception is intended, read the value from the data register
	rxd = SPI_1->DR;

	return rxd;
}

uint16_t SPI_Receive(uint8_t addr){
	// Assert CS low
	gpio_reset_bit(GPIO_E, 3u);
	// Small delay to satisfy tCSS (a few cycles)
	for(volatile int i=0;i<200;i++) { __asm volatile ("nop"); }
	// Read transaction: set READ bit; some devices tolerate setting auto-increment bit too
	addr |= 0x80; // read
	SPI_Transmit(addr);
	rxdf = SPI_Transmit(0x00);
	// Deassert CS
	gpio_set_bit(GPIO_E, 3u);
	return rxdf;
}

void LIS_Write(uint8_t addr,uint8_t data){
	// Selecting the LIS accelerometer
	gpio_reset_bit(GPIO_E, 3u); // CS low

	// Send the Register Address
	SPI_Transmit(addr);

	// Send the data to be written
	SPI_Transmit(data);

	// De-select the accelerometer
	gpio_set_bit(GPIO_E, 3u); // CS high
}

void LIS_Init(){
	// Powering on the accelerometer and Enabling the x,y and z axis for acceleration capture
	LIS_Write(CTRL_REG1, 0x47);
	// Short settle
	TIM4_ms_Delay(5);
}

void LIS_Read(){
	// Reading the data for x-axis
	x = SPI_Receive(OUT_X);

	// Reading the data for y-axis
	y = SPI_Receive(OUT_Y);

	// Reading the data for z-axis
	z = SPI_Receive(OUT_Z);
}

static uint8_t SPI_ReadReg(uint8_t reg) { return (uint8_t)SPI_Receive(reg); }

static uint8_t LIS_WhoAmI(void) { return SPI_ReadReg(WHO_AM_I); }

static void SPI1_SetMode(uint8_t mode) {
	// mode: 0..3
	SPI_1->CR1 &= ~(SPI_CR1_CPOL | SPI_CR1_CPHA);
	if (mode & 0x02) SPI_1->CR1 |= SPI_CR1_CPOL;
	if (mode & 0x01) SPI_1->CR1 |= SPI_CR1_CPHA;
}

void TIM4_ms_Delay(uint16_t delay){
	// Simple busy-wait ~1ms per iteration at ~16 MHz
	for(uint16_t ms=0; ms<delay; ++ms){
		for(volatile uint32_t i=0; i<16000; ++i){ __asm volatile ("nop"); }
	}
}

int16_t Convert_To_Val(uint16_t val){
	if ((val & 0x80) == 0x80){
		val = ~val;
		val += 1;
		val &= 0x00FF;
		val = ( val * 2300 ) / 127;
		return (-1 * val);
	}
	else
		return (( val * 2300 ) / 127);
}

int main(void){
	LIS_GPIO_Init();
	LIS_SPI1_Init();
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
	gpio_set_bit(GPIO_E, 3u);
	while(1){
		// Call the read function
		LIS_Read();

		// Use the Convert_To_Val function to convert raw data into actual data
		x_final = Convert_To_Val(x) + X_OFFSET;
		y_final = Convert_To_Val(y);
		z_final = Convert_To_Val(z);

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
			snprintf(msg, sizeof(msg), "raw:%3u,%3u,%3u conv:%6d,%6d,%6d\r\n", x, y, z, x_final, y_final, z_final);
			USART_WriteString(&usart, msg);
		}

		// Give a finite delay
		TIM4_ms_Delay(20);
	}
}