#include "bme68x_env_sensor.h"

// Very basic helpers: single-byte read/write and a tiny delay
static void delay_cycles(volatile uint32_t c)
{
    while (c--) __asm__("nop");
}

static void bme68x_write_u8(uint8_t address, uint8_t reg, uint8_t val)
{
    I2C_Start();
    I2C_SendAddress(address, 0); // write
    I2C_Write(reg);
    I2C_Write(val);
    I2C_Stop();
}

static uint8_t bme68x_read_u8(uint8_t address, uint8_t reg)
{
    uint8_t v;
    // Write register address
    I2C_Start();
    I2C_SendAddress(address, 0); // write
    I2C_Write(reg);

    // Read one byte
    I2C_Restart();
    I2C_SendAddress(address, 1); // read
    I2C_DisableAck();
    while (!(I2C_1->SR1 & I2C_SR1_RXNE));
    I2C_Stop();
    v = (uint8_t)I2C_1->DR;
    I2C_EnableAck();
    return v;
}

void BME68x_Init()
{
    // Bring up I2C then perform a soft reset and verify chip ID.
    I2C_Init(I2C_STANDARD_MODE);

    // Soft reset (datasheet: write 0xB6 to 0xE0)
    bme68x_write_u8(BME68X_ADDR, 0xE0, 0xB6);
    delay_cycles(100000); // ~6ms at 168MHz (coarse)

    // Read chip ID (0xD0 should be 0x61). We ignore mismatch silently here.
    (void)bme68x_read_u8(BME68X_ADDR, 0xD0);
}

// Trigger a simple forced measurement with 1x temp/hum oversampling
static void bme68x_trigger_forced(uint8_t address)
{
    // Humidity oversampling x1
    bme68x_write_u8(address, 0x72, 0x01);
    // Temperature oversampling x1, pressure skipped, mode = forced (01)
    bme68x_write_u8(address, 0x74, 0x21);
    // Conversion time (forced mode) ~3-5ms; use generous fixed delay for reliability.
    delay_cycles(300000); // ~18ms at 168MHz
}

// --- Minimal temperature calibration (BME68x) ---
static uint8_t s_cal_loaded = 0;
static uint16_t par_t1;
static int16_t  par_t2;
static int8_t   par_t3;

static void bme68x_load_temp_calib(uint8_t address)
{
    if (s_cal_loaded) return;
    uint8_t t1_l = bme68x_read_u8(address, 0xE9);
    uint8_t t1_h = bme68x_read_u8(address, 0xEA);
    par_t1 = (uint16_t)((t1_h << 8) | t1_l);

    uint8_t t2_l = bme68x_read_u8(address, 0x8A);
    uint8_t t2_h = bme68x_read_u8(address, 0x8B);
    par_t2 = (int16_t)((t2_h << 8) | t2_l);

    par_t3 = (int8_t)bme68x_read_u8(address, 0x8C);
    s_cal_loaded = 1;
}

int read_temp(uint8_t address)
{
    // Trigger a forced measurement
    bme68x_trigger_forced(address);
    bme68x_load_temp_calib(address);
    // Optional status read (clears previous NEW_DATA bit); ignore value.
    (void)bme68x_read_u8(address, 0x1D);

    // Read raw temperature ADC (msb@0x22, lsb@0x23, xlsb@0x24 upper nibble)
    uint32_t msb  = bme68x_read_u8(address, 0x22);
    uint32_t lsb  = bme68x_read_u8(address, 0x23);
    uint32_t xlsb = bme68x_read_u8(address, 0x24);
    uint32_t adc_t = (msb << 12) | (lsb << 4) | (xlsb >> 4);

    // Compensate temperature
    int32_t var1 = ((int32_t)adc_t >> 3) - ((int32_t)par_t1 << 1);
    int32_t var2 = (var1 * (int32_t)par_t2) >> 11;
    int32_t var3 = (((var1 >> 1) * (var1 >> 1)) >> 12);
    var3 = (var3 * ((int32_t)par_t3 << 4)) >> 14;
    int32_t t_fine = var2 + var3;
    int32_t T_01C = ((t_fine * 5) + 128) >> 8; // 0.01 C units
    int temp_c = (int)(T_01C / 100);
    return temp_c;
}

// Debug helper: fetch calibration and raw temperature ADC without compensation
int bme68x_get_cal_and_raw(uint8_t address, uint16_t *out_t1, int16_t *out_t2, int8_t *out_t3, uint32_t *out_adc_temp)
{
    if (!out_t1 || !out_t2 || !out_t3 || !out_adc_temp) return -1;

    // Always (re)read calibration to ensure values are valid
    uint8_t t1_l = bme68x_read_u8(address, 0xE9);
    uint8_t t1_h = bme68x_read_u8(address, 0xEA);
    *out_t1 = (uint16_t)((t1_h << 8) | t1_l);

    uint8_t t2_l = bme68x_read_u8(address, 0x8A);
    uint8_t t2_h = bme68x_read_u8(address, 0x8B);
    *out_t2 = (int16_t)((t2_h << 8) | t2_l);

    *out_t3 = (int8_t)bme68x_read_u8(address, 0x8C);

    // Trigger measurement and rely on fixed delay inside trigger
    bme68x_trigger_forced(address);
    (void)bme68x_read_u8(address, 0x1D); // clear status

    uint32_t msb  = bme68x_read_u8(address, 0x22);
    uint32_t lsb  = bme68x_read_u8(address, 0x23);
    uint32_t xlsb = bme68x_read_u8(address, 0x24);
    *out_adc_temp = (msb << 12) | (lsb << 4) | (xlsb >> 4);
    return 0;
}

int read_raw_temp(uint8_t address)
{
    // Trigger measurement and rely on fixed delay inside trigger
    bme68x_trigger_forced(address);
    (void)bme68x_read_u8(address, 0x1D); // clear status

    uint32_t msb  = bme68x_read_u8(address, 0x22);
    uint32_t lsb  = bme68x_read_u8(address, 0x23);
    uint32_t xlsb = bme68x_read_u8(address, 0x24);
    int out_adc_temp = (msb << 12) | (lsb << 4) | (xlsb >> 4);
    return out_adc_temp;
}