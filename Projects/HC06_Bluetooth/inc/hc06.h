#ifndef HC06_H
#define HC06_H

#include "../../Drivers/UART/inc/uart.h"
#include "../../Drivers/GPIO/inc/gpio.h"
#include <stdint.h>
#include <stddef.h>

typedef struct
{   
    USART_TypeDef usart_handle;              // high-level UART handle (embedded)
    USART_Manual_TypeDef *usart_regs;        // hardware register block
    UART_COMType hc06_com_type;
    UART_BaudRateType hc06_baud_rate;
} HC06;

void HC06_Init(HC06 *hc06,
               USART_Manual_TypeDef *usart_regs,
               UART_COMType com_type,
               UART_BaudRateType baud_rate);

// Function prototypes
// Send/receive raw bytes with explicit length
void HC06_SendData(HC06 *hc06, const uint8_t *data, size_t len);
void HC06_ReceiveData(HC06 *hc06, uint8_t *buffer, size_t len);
uint8_t HC06_ReceiveByte(HC06 *hc06);
const char* HC06_GetPortName(const HC06 *hc06); // return port name as string

#endif // HC06_H