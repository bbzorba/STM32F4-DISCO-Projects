#include "hc06_mlx90614_temp.h"

void hc06_scan_i2c(HC06 *hc06)
{
    char header[32];
    int h = snprintf(header, sizeof(header), "\r\nI2C Scan:\r\n");
    if (h > 0) {
        if (h > (int)sizeof(header)) h = (int)sizeof(header);
        HC06_SendData(hc06, (const uint8_t*)header, (size_t)h);
    }

    int found = 0;
    char line[32];
    for (uint8_t addr = 0x08; addr <= 0x77; ++addr) {
        I2C_Start();
        int ack = I2C_SendAddress(addr, 0);
        if (ack) {
            ++found;
            // Stop already done? I2C_SendAddress issues STOP only on NACK; do it here on ACK to finish frame
            I2C_Stop();
            int n = snprintf(line, sizeof(line), "  0x%02X ACK\r\n", addr);
            if (n > 0 && n < (int)sizeof(line)) HC06_SendData(hc06, (uint8_t*)line, (size_t)n);
        }
    }
    int t = snprintf(line, sizeof(line), "Scan done: %d device(s).\r\n", found);
    if (t > 0 && t < (int)sizeof(line)) HC06_SendData(hc06, (uint8_t*)line, (size_t)t);
}

void send_obj_temp(HC06 *hc06)
{
    char buffer[48];
    int obj_temp = read_obj_temp(MLX90614_ADDR);
    int len = snprintf(buffer, sizeof(buffer), "Object Temperature: %d\r\n", obj_temp);
    if (len > 0 && len < sizeof(buffer)) {
        HC06_SendData(hc06, (uint8_t *)buffer, (size_t)len);
    }
}

void send_amb_temp(HC06 *hc06)
{
    char buffer[48];
    int amb_temp = read_amb_temp(MLX90614_ADDR);
    int len = snprintf(buffer, sizeof(buffer), "Ambient Temperature: %d\r\n", amb_temp);
    if (len > 0 && len < sizeof(buffer)) {
        HC06_SendData(hc06, (uint8_t *)buffer, (size_t)len);
    }

}
