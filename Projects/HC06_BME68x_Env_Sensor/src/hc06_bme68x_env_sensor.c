#include "hc06_bme68x_env_sensor.h"

void send_temp(HC06 *hc06)
{
    char buffer[20];
    int temp = read_temp(BME68X_ADDR);
    int len = snprintf(buffer, sizeof(buffer), "Temperature: %d\r\n", temp);
    if (len > 0 && len < sizeof(buffer)) {
        HC06_SendData(hc06, (uint8_t *)buffer, (size_t)len);
    }

    char buffer2[30];
    int raw_temp = read_raw_temp(BME68X_ADDR);
    int len2 = snprintf(buffer2, sizeof(buffer2), "Temp ADC: %d\r\n", raw_temp);
    if (len2 > 0 && len2 < sizeof(buffer2)) {
        HC06_SendData(hc06, (uint8_t *)buffer2, (size_t)len2);
    }
}

void send_bme68x_debug(HC06 *hc06)
{
    uint16_t t1; int16_t t2; int8_t t3; uint32_t adc_raw; int rc;
    rc = bme68x_get_cal_and_raw(BME68X_ADDR, &t1, &t2, &t3, &adc_raw);
    char buffer[96];
    if (rc == 0) {
        int len = snprintf(buffer, sizeof(buffer),
                           "DBG t1=%u t2=%d t3=%d adc_temp=0x%05lX (%lu)\r\n",
                           t1, t2, (int)t3, (unsigned long)adc_raw, (unsigned long)adc_raw);
        if (len > 0 && len < (int)sizeof(buffer)) {
            HC06_SendData(hc06, (uint8_t*)buffer, (size_t)len);
        }
    } else {
        const char *err = "DBG read failed\r\n";
        HC06_SendData(hc06, (const uint8_t*)err, strlen(err));
    }
}