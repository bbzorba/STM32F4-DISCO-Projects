#ifndef HC06_H
#define HC06_H

#include "../../Drivers/UART/inc/uart.h"
#include "../../Drivers/GPIO/inc/gpio.h"
#include <stdint.h>

// Function prototypes
void HC06_SendData(uint8_t *data, uint16_t length);
void HC06_ReceiveData(uint8_t *buffer, uint16_t length);

#endif // HC06_H