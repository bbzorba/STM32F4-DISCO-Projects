#include "../inc/lis302dl.h"

uint16_t rxd,rxdf;

void LIS_GPIO_Init(GPIO_HandleTypeDef *CS_Handle){

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
	GPIO_SetBit(&CS_Handle, 3u);
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

uint16_t SPI_Receive(GPIO_HandleTypeDef *CS_Handle, uint8_t addr){
	// Assert CS low
	GPIO_ResetBit(CS_Handle, 3u);
	// Small delay to satisfy tCSS (a few cycles)
	for(volatile int i=0;i<50;i++) __asm__("nop");
	// Read transaction: set READ bit; some devices tolerate setting auto-increment bit too
	addr |= 0x80; // read
	SPI_Transmit(addr);
	rxdf = SPI_Transmit(0x00);
	// Deassert CS
	GPIO_SetBit(CS_Handle, 3u);
	return rxdf;
}

void LIS_Write(GPIO_HandleTypeDef *CS_Handle, uint8_t addr,uint8_t data){
	// Selecting the LIS accelerometer
	GPIO_ResetBit(CS_Handle, 3u); // CS low
	// Send the Register Address
	SPI_Transmit(addr);

	// Send the data to be written
	SPI_Transmit(data);

	// De-select the accelerometer
	GPIO_SetBit(CS_Handle, 3u); // CS high
}

void LIS_Init(GPIO_HandleTypeDef *CS_Handle){
	// Powering on the accelerometer and Enabling the x,y and z axis for acceleration capture
	LIS_Write(CS_Handle, CTRL_REG1, 0x47);
	// Short settle
	TIM4_ms_Delay(5);
}

uint8_t SPI_ReadReg(GPIO_HandleTypeDef *CS_Handle, uint8_t reg) { return (uint8_t)SPI_Receive(CS_Handle, reg); }

uint8_t LIS_WhoAmI(GPIO_HandleTypeDef *CS_Handle) { return SPI_ReadReg(CS_Handle, WHO_AM_I); }

void SPI1_SetMode(uint8_t mode) {
	// mode: 0..3
	SPI_1->CR1 &= ~(SPI_CR1_CPOL | SPI_CR1_CPHA);
	if (mode & 0x02) SPI_1->CR1 |= SPI_CR1_CPOL;
	if (mode & 0x01) SPI_1->CR1 |= SPI_CR1_CPHA;
}

void TIM4_ms_Delay(uint16_t delay){
	RCC->APB1ENR |= RCC_APB1ENR_TIM4EN; //Enable the clock for TIM4
	TIM_4->TIM_PSC = 16000-1; //Set the clock frequency to 1KHz
	TIM_4->TIM_ARR = delay; // Get the required delay from user
	TIM_4->TIM_CNT = 0;
	TIM_4->TIM_CR1 |= 1; // Start the timer
	while(!(TIM_4->TIM_SR & 1)){} // Wait for the "Update Interrupt Flag"
	TIM_4->TIM_SR &= ~(0x0001); // Clear the "Update Interrupt Flag"
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
