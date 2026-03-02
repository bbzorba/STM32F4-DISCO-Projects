#include "../inc/lis302dl.h"

// Persistent handles and init structs to avoid returning stack pointers
static GPIO_HandleTypeDef s_spi_pins_handle;
static GPIO_InitTypeDef  s_spi_pins_init;
static GPIO_HandleTypeDef s_cs_handle;
static GPIO_InitTypeDef  s_cs_init;
static GPIO_HandleTypeDef s_leds_handle;
static GPIO_InitTypeDef  s_leds_init;

void LIS302DL_constructor(LIS302DL_HandleType* lis302dl,
					  SPI_HandleType* spi,
					  GPIO_HandleTypeDef *csPin) {
	// Bind provided handles; initialize device separately
	lis302dl->spi = spi;
	lis302dl->csPin = csPin;
	LIS_Init(lis302dl);
}

GPIO_HandleTypeDef* LIS_SPI_Pins_Init(){
	// Configure PA5/PA6/PA7 for SPI1 (AF5), pull-down, medium speed
	s_spi_pins_init.Pin = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
	s_spi_pins_init.Mode = GPIO_MODE_AF_PP;
	s_spi_pins_init.Pull = GPIO_PULLDOWN;
	s_spi_pins_init.Speed = GPIO_SPEED_VERY_HIGH; // ensure clean SPI edges
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

void LIS_Init(LIS302DL_HandleType* lis302){
	// Powering on the accelerometer and Enabling the x,y and z axis for acceleration capture
	LIS_Write(lis302, CTRL_REG1, 0x47);
	// Short settle
	TIM4_ms_Delay(5);
}

void LIS_Write(LIS302DL_HandleType* lis302, uint8_t addr,uint8_t data){
	// Selecting the LIS accelerometer
	GPIO_ResetBit(lis302->csPin, 3u); // CS low
	for(volatile int i=0;i<200;i++) { __asm volatile ("nop"); }
	// Send the Register Address then the data using driver
	SPI_WriteRead(lis302->spi, &addr, NULL, 1);
	SPI_WriteRead(lis302->spi, &data, NULL, 1);
	// De-select the accelerometer
	GPIO_SetBit(lis302->csPin, 3u); // CS high
}

int16_t LIS_Read(LIS302DL_HandleType* lis302, int reg_addr){
	// Reading the data for x-axis
	uint16_t raw_value = SPI_ReadReg(lis302->spi, lis302->csPin, lis302->csPin->init->Pin, (uint8_t)reg_addr, 0x80u);
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

uint8_t LIS_WhoAmI(LIS302DL_HandleType* lis302dl) {
	// LIS302DL uses bit7 as read flag (0x80)
	return SPI_ReadReg(lis302dl->spi, lis302dl->csPin, lis302dl->csPin->init->Pin, WHO_AM_I, 0x80u);
}

void mode_fallback(LIS302DL_HandleType* lis302, uint8_t expected){
	uint8_t who = LIS_WhoAmI(lis302);
	if (who != expected) {
		// try mode 0
		SPI_SetMode(lis302->spi, 0);
		TIM4_ms_Delay(1);
		who = LIS_WhoAmI(lis302);
		if (who != expected) {
			// restore mode 3 as default
			SPI_SetMode(lis302->spi, 3);
		}
	}
}

void TIM4_ms_Delay(uint16_t delay){
	// Simple busy-wait ~1ms per iteration at ~16 MHz
	for(uint16_t ms=0; ms<delay; ++ms){
		for(volatile uint32_t i=0; i<16000; ++i){ __asm volatile ("nop"); }
	}
}