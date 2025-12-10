#include "../inc/lis302dl.h"

// Persistent handles and init structs to avoid returning stack pointers
static GPIO_InitTypeDef  s_spi_pins_init;
static GPIO_InitTypeDef  s_cs_init;
static GPIO_InitTypeDef  s_leds_init;

GPIO LIS_SPI_Pins_Init() {
    s_spi_pins_init.Pin = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
    s_spi_pins_init.Mode = GPIO_MODE_AF_PP;
    s_spi_pins_init.Pull = GPIO_PULLDOWN;
    s_spi_pins_init.Speed = GPIO_SPEED_MEDIUM;
    s_spi_pins_init.Alternate = 5u;
    return GPIO(GPIO_A, &s_spi_pins_init); // construct and return by value (NRVO/move)
}

GPIO LIS_CS_Pin_Init(){
	// Configure PE3 as CS: output push-pull, very high speed, no pull
	s_cs_init.Pin = GPIO_PIN_3;
	s_cs_init.Mode = GPIO_MODE_OUTPUT_PP;
	s_cs_init.Pull = GPIO_NOPULL;
	s_cs_init.Speed = GPIO_SPEED_VERY_HIGH;
	s_cs_init.Alternate = 0u;
	return GPIO(GPIO_E, &s_cs_init);
}

GPIO LEDS_Init(){
	// Configure PD12-PD15 as outputs for LEDs
	s_leds_init.Pin = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
	s_leds_init.Mode = GPIO_MODE_OUTPUT_PP;
	s_leds_init.Pull = GPIO_NOPULL;
	s_leds_init.Speed = GPIO_SPEED_LOW;
	s_leds_init.Alternate = 0u;
	return GPIO(GPIO_D, &s_leds_init);
}

void LIS_Init(SPI spi, GPIO csPin){
	// Powering on the accelerometer and Enabling the x,y and z axis for acceleration capture
	LIS_Write(spi, csPin, CTRL_REG1, 0x47);
	// Short settle
	TIM4_ms_Delay(5);
}

void LIS_Write(SPI spi, GPIO csPin, uint8_t addr,uint8_t data){
	// Selecting the LIS accelerometer
	csPin.GPIO_ResetBit(3u); // CS low
	for(volatile int i=0;i<200;i++) { __asm volatile ("nop"); }
	// Send the Register Address then the data using driver
	spi.SPI_WriteRead(&addr, NULL, 1);
	spi.SPI_WriteRead(&data, NULL, 1);
	// De-select the accelerometer
	csPin.GPIO_SetBit(3u); // CS high
}

int16_t LIS_Read(SPI spi, GPIO csPin, int reg_addr){
	// Reading the data for x-axis
	uint16_t raw_value = spi.SPI_ReadReg(csPin, GPIO_PIN_3, (uint8_t)reg_addr, 0x80u);
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

uint8_t LIS_WhoAmI(SPI spi, GPIO csPin) {
	// LIS302DL uses bit7 as read flag (0x80)
	return spi.SPI_ReadReg(csPin, GPIO_PIN_3, WHO_AM_I, 0x80u);
}

void mode_fallback(SPI spi, GPIO csPin, uint8_t expected){
	uint8_t who = LIS_WhoAmI(spi, csPin);
	if (who != expected) {
		// try mode 0
		spi.SPI_SetMode(0);
		TIM4_ms_Delay(1);
		who = LIS_WhoAmI(spi, csPin);
		if (who != expected) {
			// restore mode 3 as default
			spi.SPI_SetMode(3);
		}
	}
}

void TIM4_ms_Delay(uint16_t delay){
	// Simple busy-wait ~1ms per iteration at ~16 MHz
	for(uint16_t ms=0; ms<delay; ++ms){
		for(volatile uint32_t i=0; i<16000; ++i){ __asm volatile ("nop"); }
	}
}