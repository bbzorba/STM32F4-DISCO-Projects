#include "../inc/lis302dl.h"

// Minimal LIS302DL SPI helpers

void LIS302DL_Select(GPIO_HandleTypeDef *cs, uint16_t pin)   { SPI_CS_Low(cs, pin); }
void LIS302DL_Deselect(GPIO_HandleTypeDef *cs, uint16_t pin) { SPI_CS_High(cs, pin); }

static uint8_t s_spi_mode = 3; // 0..3, default 3
static int16_t s_off_x = 0;
static int16_t s_off_y = 0;
static int16_t s_off_z = 0;
static uint8_t s_mg_per_lsb = 18; // typical for LIS302DL at +/-2g

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
	uint8_t tx[2] = { (uint8_t)(reg | LIS302DL_SPI_READ), 0x00 };
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
	uint8_t tx[2] = { (uint8_t)(reg & ~(LIS302DL_SPI_READ | LIS302DL_SPI_AUTO_INC)), val };
	uint8_t rx[2] = { 0 };
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
	if (!LIS302DL_WriteReg(spi, cs, pin, LIS302DL_REG_CTRL1, 0x47)) return 0;
	uint8_t chk = 0;
	if (!LIS302DL_ReadReg(spi, cs, pin, LIS302DL_REG_CTRL1, &chk)) return 0;
	return (chk == 0x47);
}

int LIS302DL_ReadXYZ(SPI_HandleType *spi, GPIO_HandleTypeDef *cs, uint16_t pin, int8_t *x, int8_t *y, int8_t *z)
{
	if (!spi || !x || !y || !z) return 0;
	spi_apply_mode(spi);
	// Burst read: OUT_X (0x29) with read+auto-inc to read X,Y,Z in one transfer
	uint8_t tx[4] = { (uint8_t)(LIS302DL_REG_OUT_X | LIS302DL_SPI_READ | LIS302DL_SPI_AUTO_INC), 0x00, 0x00, 0x00 };
	uint8_t rx[4] = { 0 };
	LIS302DL_Select(cs, pin);
	int rc = SPI_WriteRead(spi, tx, rx, 4);
	LIS302DL_Deselect(cs, pin);
	if (rc != 0) return 0;
	*x = (int8_t)rx[1];
	*y = (int8_t)rx[2];
	*z = (int8_t)rx[3];
	return 1;
}

// Removed LIS3DSH support per user request

void LIS302DL_SetSpiMode(uint8_t mode)
{
	if (mode <= 3) s_spi_mode = mode;
}

uint8_t LIS302DL_GetSpiMode(void)
{
	return s_spi_mode;
}

int LIS302DL_SelectBestSpiMode(SPI_HandleType *spi, GPIO_HandleTypeDef *cs, uint16_t pin)
{
	uint8_t who = 0;
	const uint8_t modes[4] = {3,0,2,1};
	for (unsigned i = 0; i < 4; ++i) {
		s_spi_mode = modes[i];
		if (LIS302DL_ReadReg(spi, cs, pin, LIS302DL_REG_WHO_AM_I, &who)) {
			if (who == LIS302DL_WHOAMI_EXPECTED) return 1;
		}
	}
	// Heuristic: pick mode that yields changing OUT_X values over a few reads
	uint8_t best_mode = 3;
	uint8_t best_score = 0;
	for (unsigned i = 0; i < 4; ++i) {
		s_spi_mode = modes[i];
		uint8_t prev = 0, v = 0, score = 0;
		for (int k = 0; k < 8; ++k) {
			if (!LIS302DL_ReadReg(spi, cs, pin, LIS302DL_REG_OUT_X, &v)) break;
			if (k > 0 && v != prev) ++score;
			prev = v;
		}
		if (score > best_score) { best_score = score; best_mode = modes[i]; }
	}
	s_spi_mode = best_mode;
	return 0;
}

void LIS302DL_SetSensitivityMgPerLsb(uint8_t mg_per_lsb)
{
	s_mg_per_lsb = mg_per_lsb ? mg_per_lsb : 18;
}

int LIS302DL_Calibrate(SPI_HandleType *spi, GPIO_HandleTypeDef *cs, uint16_t pin, uint16_t samples)
{
	int32_t sx = 0, sy = 0, sz = 0;
	if (samples == 0) return 0;
	for (uint16_t i = 0; i < samples; ++i) {
		int8_t x, y, z;
		if (!LIS302DL_ReadXYZ(spi, cs, pin, &x, &y, &z)) return 0;
		sx += x; sy += y; sz += z;
	}
	s_off_x = (int16_t)(sx / (int32_t)samples);
	s_off_y = (int16_t)(sy / (int32_t)samples);
	s_off_z = (int16_t)(sz / (int32_t)samples);
	return 1;
}

void LIS302DL_GetCalibration(int16_t *ox, int16_t *oy, int16_t *oz, uint8_t *mg_per_lsb)
{
	if (ox) *ox = s_off_x;
	if (oy) *oy = s_off_y;
	if (oz) *oz = s_off_z;
	if (mg_per_lsb) *mg_per_lsb = s_mg_per_lsb;
}

