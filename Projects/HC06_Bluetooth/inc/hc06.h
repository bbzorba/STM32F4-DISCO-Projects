#ifndef HC06_H
#define HC06_H

#include "../../Drivers/UART/inc/uart.h"
#include "../../Drivers/GPIO/inc/gpio.h"
#include <stdint.h>
#include <stddef.h>

typedef struct
{
    UART_COMType hc06_com_type;
    UART_BaudRateType hc06_baud_rate;
} HC06;

void HC06_Init(HC06 *hc06,
               UART_COMType com_type,
               UART_BaudRateType baud_rate);

// Function prototypes
// Send/receive raw bytes with explicit length
void HC06_SendData(HC06 *hc06, const uint8_t *data, size_t len);
void HC06_ReceiveData(HC06 *hc06, uint8_t *buffer, size_t len);

#endif // HC06_H