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

void send_gas_density(HC06 *hc06)
{
    char buffer[30];
    int gas_density = read_gas_density(BME68x_ADDR);
    int len = snprintf(buffer, sizeof(buffer), "Gas Density: %d Ohms\r\n", gas_density);
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