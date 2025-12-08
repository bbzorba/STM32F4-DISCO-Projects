#include "../inc/lis302dl.h"

// Minimal LIS302DL SPI helpers

void LIS302DL_Select(GPIO_HandleTypeDef *cs, uint16_t pin)   { SPI_CS_Low(cs, pin); }
// Keep CS low (asserted) to emulate prior behavior that produced non-00/FF reads
void LIS302DL_Deselect(GPIO_HandleTypeDef *cs, uint16_t pin) { (void)cs; (void)pin; /* keep asserted */ }

static uint8_t s_spi_mode = 3; // 0..3, default 3

static void spi_apply_mode(SPI_HandleType *spi)
{
	if (!spi || !spi->regs) return;
	// Clear CPOL/CPHA then set according to selected mode
	spi->regs->CR1 &= ~(SPI_CR1_CPOL | SPI_CR1_CPHA);
	if (s_spi_mode == 0) {
		// CPOL=0, CPHA=0
	} else if (s_spi_mode == 1) {
		spi->regs->CR1 |= SPI_CR1_CPHA; // CPOL=0, CPHA=1
	} else if (s_spi_mode == 2) {
		spi->regs->CR1 |= SPI_CR1_CPOL; // CPOL=1, CPHA=0
	} else { // 3
		spi->regs->CR1 |= (SPI_CR1_CPOL | SPI_CR1_CPHA); // CPOL=1, CPHA=1
	}
}

int LIS302DL_ReadReg(SPI_HandleType *spi, GPIO_HandleTypeDef *cs, uint16_t pin, uint8_t reg, uint8_t *val)
{
	if (!spi || !val) return 0;
	spi_apply_mode(spi);
	uint8_t tx[2] = { (uint8_t)(reg | LIS302DL_SPI_READ), 0xFF };
	uint8_t rx[2] = { 0 };
	LIS302DL_Select(cs, pin);
	int rc = SPI_WriteRead(spi, tx, rx, 2);
	LIS302DL_Deselect(cs, pin);
	if (rc != 0) return 0;
	*val = rx[1];
	return 1;
}

int LIS302DL_WriteReg(SPI_HandleType *spi, GPIO_HandleTypeDef *cs, uint16_t pin, uint8_t reg, uint8_t val)
{
	if (!spi) return 0;
	spi_apply_mode(spi);
	uint8_t tx[2] = { reg & ~(LIS302DL_SPI_READ | LIS302DL_SPI_AUTO_INC), val };
	uint8_t rx[2];
	LIS302DL_Select(cs, pin);
	int rc = SPI_WriteRead(spi, tx, rx, 2);
	LIS302DL_Deselect(cs, pin);
	return (rc == 0);
}

int LIS302DL_ReadWhoAmI(SPI_HandleType *spi, GPIO_HandleTypeDef *cs, uint16_t pin, uint8_t *who)
{
	// Try modes 3,0,2,1
	static const uint8_t modes[4] = {3,0,2,1};
	for (unsigned i = 0; i < 4; ++i) {
		s_spi_mode = modes[i];
		if (LIS302DL_ReadReg(spi, cs, pin, LIS302DL_REG_WHO_AM_I, who)) {
			if (*who == LIS302DL_WHOAMI_EXPECTED) return 1;
		}
	}
	// default back to 3 if not found
	s_spi_mode = 3;
	return 0;
}

int LIS302DL_ReadWhoAmI_Mode(SPI_HandleType *spi, GPIO_HandleTypeDef *cs, uint16_t pin, uint8_t mode, uint8_t *who)
{
	if (mode > 3) return 0;
	s_spi_mode = mode;
	return LIS302DL_ReadReg(spi, cs, pin, LIS302DL_REG_WHO_AM_I, who);
}

int LIS302DL_Init(SPI_HandleType *spi, GPIO_HandleTypeDef *cs, uint16_t pin)
{
	// CTRL1: Power on, enable all axes, 100Hz data rate
	// CTRL1 bits: PD=1 (power on), Zen/Yen/Xen=1, DR bits for ODR
	// Typical enable value: 0x47 (0b0100_0111) or 0x47/0x67 depending on ODR
	// Use 0x47: 100Hz, all axes enabled, normal power
	return LIS302DL_WriteReg(spi, cs, pin, LIS302DL_REG_CTRL1, 0x47);
}

int LIS302DL_ReadXYZ(SPI_HandleType *spi, GPIO_HandleTypeDef *cs, uint16_t pin, int8_t *x, int8_t *y, int8_t *z)
{
	if (!spi) return 0;
	spi_apply_mode(spi);
	// Read OUT_X, OUT_Y, OUT_Z (1-byte each, already 8-bit signed)
	uint8_t val;
	if (x) { if (!LIS302DL_ReadReg(spi, cs, pin, LIS302DL_REG_OUT_X, &val)) return 0; *x = (int8_t)val; }
	if (y) { if (!LIS302DL_ReadReg(spi, cs, pin, LIS302DL_REG_OUT_Y, &val)) return 0; *y = (int8_t)val; }
	if (z) { if (!LIS302DL_ReadReg(spi, cs, pin, LIS302DL_REG_OUT_Z, &val)) return 0; *z = (int8_t)val; }
	return 1;
}

// Removed LIS3DSH support per user request

