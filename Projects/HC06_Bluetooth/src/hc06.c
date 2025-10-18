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

void HC06_SendData(HC06 *hc06, uint16_t *data, uint16_t length) {
    if (hc06 == NULL) {
        return;
    }
    for (uint16_t i = 0; i < length; i++) {
        USART_2_Write(data[i]);
    }
}

void HC06_ReceiveData(HC06 *hc06, uint16_t *buffer, uint16_t length) {
    if (hc06 == NULL) {
        return;
    }
    for (uint16_t i = 0; i < length; i++) {
        buffer[i] = USART_2_Read();
    }
}
