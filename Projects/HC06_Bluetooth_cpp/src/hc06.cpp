#include "../inc/hc06.h"

HC06::HC06(USART_ManualType *usart_regs,
        UART_COMType com_type,
        UART_BaudRateType baud_rate)
    : usart(usart_regs, com_type, baud_rate),
      hc06_com_type(com_type),
      hc06_baud_rate(baud_rate)
{
}

void HC06::HC06_SendData(const uint8_t *data, size_t len) {
    if (!data) return;
    for (size_t i = 0; i < len; i++) {
        usart.USART_x_Write((int)data[i]);
    }
}

void HC06::HC06_ReceiveData(uint8_t *buffer, size_t len) {
    if (!buffer) return;
    for (size_t i = 0; i < len; i++) {
        buffer[i] = (uint8_t)usart.USART_x_Read();
    }
}

uint8_t HC06::HC06_ReceiveByte() {
    return (uint8_t)usart.USART_x_Read();
}

const char* HC06::HC06_GetPortName() {
    return usart.GetPortName();
}

void hc06_scan_i2c(HC06 *hc06, I2C *hi2c)
{
    char header[32];
    int h = snprintf(header, sizeof(header), "\r\nI2C Scan:\r\n");
    if (h > 0) {
        if (h > (int)sizeof(header)) h = (int)sizeof(header);
        hc06->HC06_SendData((const uint8_t*)header, (size_t)h);
    }

    int found = 0;
    char line[32];
    for (uint8_t addr = 0x08; addr <= 0x77; ++addr) {
        hi2c->I2C_Start();
        int ack = hi2c->I2C_SendAddress(addr, 0);
        if (ack) {
            ++found;
            // Stop already done? I2C_SendAddress issues STOP only on NACK; do it here on ACK to finish frame
            hi2c->I2C_Stop();
            int n = snprintf(line, sizeof(line), "  0x%02X ACK\r\n", addr);
            if (n > 0 && n < (int)sizeof(line)) hc06->HC06_SendData((uint8_t*)line, (size_t)n);
        }
    }
    int t = snprintf(line, sizeof(line), "Scan done: %d device(s).\r\n", found);
    if (t > 0 && t < (int)sizeof(line)) hc06->HC06_SendData((uint8_t*)line, (size_t)t);
}