#include "hc06.h"

void HC06_SendData(uint8_t *data, uint16_t length) {
    for (uint16_t i = 0; i < length; i++) {
        USART_2_Write(data[i]);
    }
}

void HC06_ReceiveData(uint8_t *buffer, uint16_t length) {   
    for (uint16_t i = 0; i < length; i++) {
        buffer[i] = USART_2_Read();
    }
}
