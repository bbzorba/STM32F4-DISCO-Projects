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