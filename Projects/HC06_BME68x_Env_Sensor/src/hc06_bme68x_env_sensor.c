#include "hc06_bme68x_env_sensor.h"

void send_temp(HC06 *hc06)
{
    char buffer[20];
    int temp = read_temp(BME68x_ADDR);
    int len = snprintf(buffer, sizeof(buffer), "Temperature: %d\r\n", temp);
    if (len > 0 && len < sizeof(buffer)) {
        HC06_SendData(hc06, (uint8_t *)buffer, (size_t)len);
    }
}

void send_gas_resistance(HC06 *hc06)
{
    char buffer[30];
    int gas_resistance = read_gas_resistance(BME68x_ADDR);
    int len = snprintf(buffer, sizeof(buffer), "Gas Resistance: %d Ohms\r\n", gas_resistance);
    if (len > 0 && len < sizeof(buffer)) {
        HC06_SendData(hc06, (uint8_t *)buffer, (size_t)len);
    }
}

void send_humidity(HC06 *hc06)
{
    char buffer[20];
    int humidity = read_humidity(BME68x_ADDR);
    int len = snprintf(buffer, sizeof(buffer), "Humidity: %d%%\r\n", humidity);
    if (len > 0 && len < sizeof(buffer)) {
        HC06_SendData(hc06, (uint8_t *)buffer, (size_t)len);
    }
}