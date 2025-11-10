#include "hc06.h"

HC06::HC06(USART_Manual_TypeDef *USARTx,
           UART_COMType com_type,
           UART_BaudRateType baud_rate)
           : hc06_com_type(com_type),
           hc06_baud_rate(baud_rate),
           usart(USARTx, com_type, baud_rate)
{
}

void HC06::SendData(const uint8_t *data, size_t len) {
    if (!data) return;
    for (size_t i = 0; i < len; i++) {
        usart.USART_x_Write((int)data[i]);
    }
}

void HC06::ReceiveData(uint8_t *buffer, size_t len) {
    if (!buffer) return;
    for (size_t i = 0; i < len; i++) {
        buffer[i] = (uint8_t)usart.USART_x_Read();
    }
}

uint8_t HC06::ReceiveData() {
    return (uint8_t)usart.USART_x_Read();
}

const char* HC06::GetPortName() const {
    return usart.GetPortName();
}