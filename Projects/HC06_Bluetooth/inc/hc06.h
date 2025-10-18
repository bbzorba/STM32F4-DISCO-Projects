#ifndef HC06_H
#define HC06_H

#include "../../Drivers/UART/inc/uart.h"
#include "../../Drivers/GPIO/inc/gpio.h"
#include <stdint.h>

typedef struct
{
    UART_COMType hc06_com_type;
    UART_BaudRateType hc06_baud_rate;
} HC06;

void HC06_Init(HC06 *hc06,
               UART_COMType com_type,
               UART_BaudRateType baud_rate);

// Function prototypes
void HC06_SendData(HC06 *hc06, uint16_t *data, uint16_t length);
void HC06_ReceiveData(HC06 *hc06, uint16_t *buffer, uint16_t length);

#endif // HC06_H