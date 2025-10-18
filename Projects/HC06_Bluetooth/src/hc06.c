#include "hc06.h"

void HC06_Init(HC06 *hc06,
               UART_COMType com_type,
               UART_BaudRateType baud_rate) {
    if (hc06 == NULL) {
        return;
    }
    hc06->hc06_com_type = com_type;
    hc06->hc06_baud_rate = baud_rate;

    // Initialize USART2 with specified parameters
    USART_2_Init(com_type, baud_rate);
}

void HC06_SendData(HC06 *hc06, const uint8_t *data, size_t len) {
    if (!hc06 || !data) return;
    for (size_t i = 0; i < len; i++) {
        USART_2_Write((int)data[i]);
    }
}

void HC06_ReceiveData(HC06 *hc06, uint8_t *buffer, size_t len) {
    if (!hc06 || !buffer) return;
    for (size_t i = 0; i < len; i++) {
        buffer[i] = (uint8_t)USART_2_Read();
    }
}
