// Minimal, register-level LIS302DL SPI example adapted to STM32F407
// Pins: CS=PE3, SCK=PA5, MOSI=PA7, MISO=PA6

#include <stdint.h>
#include <stdio.h>
#include "../../Drivers/STM32F4xx_HAL_Driver/stm32f407xx.h"

// Minimal UART forward declarations to avoid including gpio.h via uart.h
typedef struct {
	volatile uint32_t SR;
	volatile uint32_t DR;
	volatile uint32_t BRR;
	volatile uint32_t CR1;
	volatile uint32_t CR2;
	volatile uint32_t CR3;
	volatile uint32_t GTPR;
} USART_ManualType;

typedef enum { RX_ONLY = 0, TX_ONLY, RX_AND_TX } UART_COMType;
typedef enum { __115200 = 0, __9600 } UART_BaudRateType;

typedef struct {
	UART_COMType comType;
	UART_BaudRateType baudRate;
	USART_ManualType *regs;
} USART_HandleType;

#define USART_2 ((USART_ManualType *)0x40004400U)

void USART_constructor(USART_HandleType *handle, USART_ManualType *regs, UART_COMType _comtype, UART_BaudRateType _baudrate);
void USART_Init(USART_HandleType *handle);
void USART_WriteString(USART_HandleType *handle, const char *str);

//Register Definitions for LIS302DL
#define LIS302DL_ADDR     (0x3B)
#define WHO_AM_I          (0x0F)
#define CTRL_REG1         (0x20)
#define CTRL_REG2         (0x21)
#define CTRL_REG3         (0x22)
#define HP_FILTER_RESET   (0x23)
#define STATUS_REG        (0x27)
#define OUT_X             (0x29)
#define OUT_Y             (0x2B)
#define OUT_Z             (0x2D)

// Calibration constants
#define X_OFFSET 18
#define THRESH_LOW -120
#define THRESH_HIGH 120

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

static inline void gpio_set_bit(GPIO_TypeDef *port, uint32_t pin) { port->BSRRL = (uint16_t)(1u << pin); }
static inline void gpio_reset_bit(GPIO_TypeDef *port, uint32_t pin) { port->BSRRH = (uint16_t)(1u << pin); }

void LIS_GPIO_Init(){
	// Enable GPIOA clock
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

	// Configuring PA5, PA6, PA7 in alternate function mode (10b)
	GPIOA->MODER &= ~((3u<<(5*2)) | (3u<<(6*2)) | (3u<<(7*2)));
	GPIOA->MODER |=  ((2u<<(5*2)) | (2u<<(6*2)) | (2u<<(7*2)));

	// Select AF5 for SPI on PA5, PA6, PA7
	GPIOA->AFR[0] &= ~((0xFu<<(4*5)) | (0xFu<<(4*6)) | (0xFu<<(4*7)));
	GPIOA->AFR[0] |=  ((5u<<(4*5)) | (5u<<(4*6)) | (5u<<(4*7)));

	// Enable GPIOE clock
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;

	// Since PE3 is CS, configure in Output Mode (01b)
	GPIOE->MODER &= ~(3u<<(3*2));
	GPIOE->MODER |=  (1u<<(3*2));

	// Push-pull, no pull, very high speed for CS
	GPIOE->OTYPER &= ~(1u<<3);
	GPIOE->PUPDR &= ~(3u<<(3*2));
	GPIOE->OSPEEDR |= (3u<<(3*2));

	// Deassert CS initially
	gpio_set_bit(GPIOE, 3u);

	// Medium speed on PA5/6/7
	GPIOA->OSPEEDR |= ((1u<<(5*2)) | (1u<<(6*2)) | (1u<<(7*2)));

	// Pull-down on PA5/6/7 (10b)
	GPIOA->PUPDR &= ~((3u<<(5*2)) | (3u<<(6*2)) | (3u<<(7*2)));
	GPIOA->PUPDR |=  ((2u<<(5*2)) | (2u<<(6*2)) | (2u<<(7*2)));

	// Enable clock for GPIOD and Configure PD12 in output mode
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	GPIOD->MODER &= ~((3u<<(12*2)) | (3u<<(13*2)) | (3u<<(14*2)) | (3u<<(15*2)));
	GPIOD->MODER |=  ((1u<<(12*2)) | (1u<<(13*2)) | (1u<<(14*2)) | (1u<<(15*2)));
}

void LIS_SPI1_Init(){
	// Enable SPI clock
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

	// Select the Master Configuration
	SPI1->CR1 |= SPI_CR1_MSTR;

	SPI1->CR1 &= ~SPI_CR1_BIDIMODE;

	SPI1->CR1 &= ~SPI_CR1_RXONLY;

	// Set the Data Frame Format (DFF) to '0' or 8-bit.
	SPI1->CR1 &= ~SPI_CR1_DFF;

	// SSI and SSM bits in the SP1->CR1 register need to be set
	// to '1'
	SPI1->CR1 |= (SPI_CR1_SSI | SPI_CR1_SSM);

	// Setting Baud Rate: f_PCLK/256 (slow and safe)
	SPI1->CR1 &= ~SPI_CR1_BR;
	SPI1->CR1 |= (SPI_CR1_BR_0 | SPI_CR1_BR_1 | SPI_CR1_BR_2);

	// Set the transmission to MSB First Mode
	SPI1->CR1 &= ~SPI_CR1_LSBFIRST;

	// Configure CPOL and CPHA to 1 and 1 (SPI mode 3) required by LIS302DL
	SPI1->CR1 |= (SPI_CR1_CPOL | SPI_CR1_CPHA);

	// Disable CRC (not used)
	SPI1->CR1 &= ~SPI_CR1_CRCEN;

	// Enable SPI
	SPI1->CR1 |= SPI_CR1_SPE;

	// Selecting Motorola Format
	SPI1->CR2 = 0x0000;
}

uint16_t SPI_Transmit(uint8_t data){
	//  Wait until the TX buffer is empty, i.e. data is transmitted
	while(!((SPI1->SR) & SPI_SR_TXE)){}
	// Load the data into the data register
	SPI1->DR = data;

	while(!(SPI1->SR & SPI_SR_RXNE)){}
	// If reception is intended, read the value from the data register
	rxd = SPI1->DR;

	return rxd;
}

uint16_t SPI_Receive(uint8_t addr){
	// Assert CS low
	gpio_reset_bit(GPIOE, 3u);
	// Small delay to satisfy tCSS (a few cycles)
	for(volatile int i=0;i<50;i++) __NOP();
	// Read transaction: set READ bit; some devices tolerate setting auto-increment bit too
	addr |= 0x80; // read
	SPI_Transmit(addr);
	rxdf = SPI_Transmit(0x00);
	// Deassert CS
	gpio_set_bit(GPIOE, 3u);
	return rxdf;
}

void LIS_Write(uint8_t addr,uint8_t data){
	// Selecting the LIS accelerometer
	gpio_reset_bit(GPIOE, 3u); // CS low

	// Send the Register Address
	SPI_Transmit(addr);

	// Send the data to be written
	SPI_Transmit(data);

	// De-select the accelerometer
	gpio_set_bit(GPIOE, 3u); // CS high
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
	SPI1->CR1 &= ~(SPI_CR1_CPOL | SPI_CR1_CPHA);
	if (mode & 0x02) SPI1->CR1 |= SPI_CR1_CPOL;
	if (mode & 0x01) SPI1->CR1 |= SPI_CR1_CPHA;
}

void TIM4_ms_Delay(uint16_t delay){
	RCC->APB1ENR |= RCC_APB1ENR_TIM4EN; //Enable the clock for TIM4
	TIM4->PSC = 16000-1; //Set the clock frequency to 1KHz
	TIM4->ARR = delay; // Get the required delay from user
	TIM4->CNT = 0;
	TIM4->CR1 |= 1; // Start the timer
	while(!(TIM4->SR & 1)){} // Wait for the "Update Interrupt Flag"
	TIM4->SR &= ~(0x0001); // Clear the "Update Interrupt Flag"
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
	gpio_set_bit(GPIOE, 3u);
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
				GPIOD->ODR |= (1u<<14);
				GPIOD->ODR &= ~((1u<<12) | (1u<<13) | (1u<<15));
			}
			else if (x_final < THRESH_LOW){
				GPIOD->ODR |= (1u<<12);
				GPIOD->ODR &= ~((1u<<14) | (1u<<13) | (1u<<15));
			}
			if (y_final > THRESH_HIGH){
				GPIOD->ODR |= (1u<<13);
				GPIOD->ODR &= ~((1u<<12) | (1u<<14) | (1u<<15));
			}
			else if (y_final < THRESH_LOW ){
				GPIOD->ODR |= (1u<<15);
				GPIOD->ODR &= ~((1u<<12) | (1u<<13) | (1u<<14));
			}
		}
		else
			GPIOD->ODR &= ~((1u<<12) | (1u<<13) | (1u<<14) | (1u<<15));

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