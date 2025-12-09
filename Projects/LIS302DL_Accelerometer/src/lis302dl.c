#include "../inc/lis302dl.h"

// Use project GPIO struct (single 32-bit BSRR)
static inline void gpio_set_bit(GPIO_ManualTypeDef *regs, uint32_t pin) { regs->BSRR = (1u << pin); }
static inline void gpio_reset_bit(GPIO_ManualTypeDef *regs, uint32_t pin) { regs->BSRR = (1u << (pin + 16u)); }

static uint8_t SPI_ReadReg(uint8_t reg) { return (uint8_t)SPI_Receive(reg); }

void SPI1_SetMode(uint8_t mode) {
	// mode: 0..3
	SPI_1->CR1 &= ~(SPI_CR1_CPOL | SPI_CR1_CPHA);
	if (mode & 0x02) SPI_1->CR1 |= SPI_CR1_CPOL;
	if (mode & 0x01) SPI_1->CR1 |= SPI_CR1_CPHA;
}

void LIS_GPIO_Init(){
	// Configure PA5/PA6/PA7 for SPI1 (AF5), pull-down, medium speed
	GPIO_InitTypeDef gpioa_init;
	gpioa_init.Pin = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
	gpioa_init.Mode = GPIO_MODE_AF_PP;
	gpioa_init.Pull = GPIO_PULLDOWN;
	gpioa_init.Speed = GPIO_SPEED_MEDIUM;
	gpioa_init.Alternate = 5u; // AF5 for SPI1
	GPIO_HandleTypeDef GPIOA_handle;
	GPIO_constructor(&GPIOA_handle, GPIO_A, &gpioa_init);

	// Configure PE3 as CS: output push-pull, very high speed, no pull
	GPIO_InitTypeDef gpioe_init;
	gpioe_init.Pin = GPIO_PIN_3;
	gpioe_init.Mode = GPIO_MODE_OUTPUT_PP;
	gpioe_init.Pull = GPIO_NOPULL;
	gpioe_init.Speed = GPIO_SPEED_VERY_HIGH;
	gpioe_init.Alternate = 0u;
	GPIO_HandleTypeDef GPIOE_handle;
	GPIO_constructor(&GPIOE_handle, GPIO_E, &gpioe_init);
	// Deassert CS initially
	GPIO_WritePin(&GPIOE_handle, GPIO_PIN_3, GPIO_PIN_SET);

	// Configure PD12-PD15 as outputs for LEDs
	GPIO_InitTypeDef gpiod_init;
	gpiod_init.Pin = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
	gpiod_init.Mode = GPIO_MODE_OUTPUT_PP;
	gpiod_init.Pull = GPIO_NOPULL;
	gpiod_init.Speed = GPIO_SPEED_LOW;
	gpiod_init.Alternate = 0u;
	GPIO_HandleTypeDef GPIOD_handle;
	GPIO_constructor(&GPIOD_handle, GPIO_D, &gpiod_init);
}

uint16_t SPI_Transmit(uint8_t data){
	//  Wait until the TX buffer is empty, i.e. data is transmitted
	while(!((SPI_1->SR) & SPI_SR_TXE)){}
	// Load the data into the data register
	SPI_1->DR = data;

	while(!(SPI_1->SR & SPI_SR_RXNE)){}
	// If reception is intended, read the value from the data register
	uint16_t rxd = SPI_1->DR;

	return rxd;
}

uint16_t SPI_Receive(uint8_t reg_addr){
	// Assert CS low
	gpio_reset_bit(GPIO_E, 3u);
	// Small delay to satisfy tCSS (a few cycles)
	for(volatile int i=0;i<200;i++) { __asm volatile ("nop"); }
	// Read transaction: set READ bit; some devices tolerate setting auto-increment bit too
	reg_addr |= 0x80; // read
	SPI_Transmit(reg_addr);
	uint16_t rxdf = SPI_Transmit(0x00);
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

int16_t LIS_Read(int reg_addr){
	// Reading the data for x-axis
	uint16_t raw_value = SPI_Receive(reg_addr);
	if ((raw_value & 0x80) == 0x80){
		raw_value = ~raw_value;
		raw_value += 1;
		raw_value &= 0x00FF;
		raw_value = ( raw_value * 2300 ) / 127;
		return (-1 * raw_value);
	}
	else
		return (( raw_value * 2300 ) / 127);
}

void TIM4_ms_Delay(uint16_t delay){
	// Simple busy-wait ~1ms per iteration at ~16 MHz
	for(uint16_t ms=0; ms<delay; ++ms){
		for(volatile uint32_t i=0; i<16000; ++i){ __asm volatile ("nop"); }
	}
}

uint8_t LIS_WhoAmI(void) { return SPI_ReadReg(WHO_AM_I); }