#include "../../Drivers/SPI_cpp/inc/spi.h"
#include "../../Drivers/UART_cpp/inc/uart.h"
#include "../../Drivers/GPIO_cpp/inc/gpio.h"
#include "../../Drivers/PWM_cpp/inc/pwm.h"

#ifndef LIS302DL_H
#define LIS302DL_H

#include <stdint.h>

// LIS302DL register addresses
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

// Pin helpers kept as free functions
GPIO LEDS_Init();
GPIO LIS_CS_Pin_Init();
GPIO LIS_SPI_Pins_Init();

// C++ LIS302DL object mirroring the C handle-based design
class LIS302DL {
public:
	// Binds to existing SPI and CS objects (owned elsewhere)
	LIS302DL(SPI* spi, GPIO* cs) : spi(spi), cs(cs) {}

	void Init();
	void Write(uint8_t addr, uint8_t data);
	int16_t Read(int reg_addr);
	uint8_t WhoAmI();
	void ModeFallback(uint8_t expected);

private:
	SPI* spi;
	GPIO* cs;
	static const uint16_t CS_MASK = GPIO_PIN_3; // PE3
};

void TIM4_ms_Delay(uint16_t delay);
#endif // LIS302DL_H