#include "hc06_mlx90614_temp.h"

void send_obj_temp(HC06 *hc06)
{
    char buffer[20];
    int obj_temp = read_obj_temp(MLX90614_ADDR);
    int len = snprintf(buffer, sizeof(buffer), "Object Temperature: %d\r\n", obj_temp);
    if (len > 0 && len < sizeof(buffer)) {
        HC06_SendData(hc06, (uint8_t *)buffer, (size_t)len);
    }
}

void send_amb_temp(HC06 *hc06)
{
    char buffer[20];
    int amb_temp = read_amb_temp(MLX90614_ADDR);
    int len = snprintf(buffer, sizeof(buffer), "Ambient Temperature: %d\r\n", amb_temp);
    if (len > 0 && len < sizeof(buffer)) {
        HC06_SendData(hc06, (uint8_t *)buffer, (size_t)len);
    }

}
