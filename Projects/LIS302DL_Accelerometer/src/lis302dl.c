#include "../inc/lis302dl.h"

// Use project GPIO struct (single 32-bit BSRR)
void set_cs_pin(GPIO_ManualTypeDef *regs, uint32_t pin) {
	 regs->BSRR = (1u << pin); 
}
void reset_cs_pin(GPIO_ManualTypeDef *regs, uint32_t pin) { 
	regs->BSRR = (1u << (pin + 16u)); 
}

static uint8_t SPI_ReadReg(uint8_t reg) { return (uint8_t)SPI_Receive(reg); }

// Persistent handles and init structs to avoid returning stack pointers
static GPIO_HandleTypeDef s_spi_pins_handle;
static GPIO_InitTypeDef  s_spi_pins_init;
static GPIO_HandleTypeDef s_cs_handle;
static GPIO_InitTypeDef  s_cs_init;
static GPIO_HandleTypeDef s_leds_handle;
static GPIO_InitTypeDef  s_leds_init;

GPIO_HandleTypeDef* LIS_SPI_Pins_Init(){
	// Configure PA5/PA6/PA7 for SPI1 (AF5), pull-down, medium speed
	s_spi_pins_init.Pin = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
	s_spi_pins_init.Mode = GPIO_MODE_AF_PP;
	s_spi_pins_init.Pull = GPIO_PULLDOWN;
	s_spi_pins_init.Speed = GPIO_SPEED_MEDIUM;
	s_spi_pins_init.Alternate = 5u; // AF5 for SPI1
	GPIO_constructor(&s_spi_pins_handle, GPIO_A, &s_spi_pins_init);
	return &s_spi_pins_handle;
}

GPIO_HandleTypeDef* LIS_CS_Pin_Init(){
	// Configure PE3 as CS: output push-pull, very high speed, no pull
	s_cs_init.Pin = GPIO_PIN_3;
	s_cs_init.Mode = GPIO_MODE_OUTPUT_PP;
	s_cs_init.Pull = GPIO_NOPULL;
	s_cs_init.Speed = GPIO_SPEED_VERY_HIGH;
	s_cs_init.Alternate = 0u;
	GPIO_constructor(&s_cs_handle, GPIO_E, &s_cs_init);
	// Deassert CS initially
	GPIO_WritePin(&s_cs_handle, GPIO_PIN_3, GPIO_PIN_SET);
	return &s_cs_handle;
}

GPIO_HandleTypeDef* LEDS_Init(){
	// Configure PD12-PD15 as outputs for LEDs
	s_leds_init.Pin = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
	s_leds_init.Mode = GPIO_MODE_OUTPUT_PP;
	s_leds_init.Pull = GPIO_NOPULL;
	s_leds_init.Speed = GPIO_SPEED_LOW;
	s_leds_init.Alternate = 0u;
	GPIO_constructor(&s_leds_handle, GPIO_D, &s_leds_init);
	return &s_leds_handle;
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
	reset_cs_pin(GPIO_E, 3u);
	// Small delay to satisfy tCSS (a few cycles)
	for(volatile int i=0;i<200;i++) { __asm volatile ("nop"); }
	// Read transaction: set READ bit; some devices tolerate setting auto-increment bit too
	reg_addr |= 0x80; // read
	SPI_Transmit(reg_addr);
	uint16_t rxdf = SPI_Transmit(0x00);
	// Deassert CS
	set_cs_pin(GPIO_E, 3u);
	return rxdf;
}

void LIS_Init(){
	// Powering on the accelerometer and Enabling the x,y and z axis for acceleration capture
	LIS_Write(CTRL_REG1, 0x47);
	// Short settle
	TIM4_ms_Delay(5);
}

void LIS_Write(uint8_t addr,uint8_t data){
	// Selecting the LIS accelerometer
	reset_cs_pin(GPIO_E, 3u); // CS low

	// Send the Register Address
	SPI_Transmit(addr);

	// Send the data to be written
	SPI_Transmit(data);

	// De-select the accelerometer
	set_cs_pin(GPIO_E, 3u); // CS high
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