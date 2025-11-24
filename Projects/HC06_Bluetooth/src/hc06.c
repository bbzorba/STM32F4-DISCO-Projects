#include "../inc/hc06.h"

void HC06_Init(HC06 *hc06,
               USART_ManualType *usart_regs,
               UART_COMType com_type,
               UART_BaudRateType baud_rate) {
    if (hc06 == NULL) {
        return;
    }
    hc06->usart_regs = usart_regs;
    hc06->hc06_com_type = com_type;
    hc06->hc06_baud_rate = baud_rate;

    // Bind and initialize the provided USART instance with specified parameters
    USART_Init(&hc06->usart_handle, usart_regs, com_type, baud_rate);
}

void HC06_SendData(HC06 *hc06, const uint8_t *data, size_t len) {
    if (!hc06 || !data) return;
    for (size_t i = 0; i < len; i++) {
        USART_x_Write(hc06->usart_regs, (int)data[i]);
    }
}

void HC06_ReceiveData(HC06 *hc06, uint8_t *buffer, size_t len) {
    if (!hc06 || !buffer) return;
    for (size_t i = 0; i < len; i++) {
        buffer[i] = (uint8_t)USART_x_Read(hc06->usart_regs);
    }
}

uint8_t HC06_ReceiveByte(HC06 *hc06) {
    return (uint8_t)USART_x_Read(hc06->usart_regs);
}

const char* HC06_GetPortName(const HC06 *hc06) {
    if (!hc06 || !hc06->usart_regs) return "USART?";
    if (hc06->usart_regs == USART_1) return "USART1";
    if (hc06->usart_regs == USART_2) return "USART2";
    if (hc06->usart_regs == USART_3) return "USART3";
    if (hc06->usart_regs == UART_4)  return "UART4";
    if (hc06->usart_regs == UART_5)  return "UART5";
    if (hc06->usart_regs == USART_6) return "USART6";
    return "USART?";
}

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