#ifndef HC06_H
#define HC06_H

#include "../../Drivers/UART/inc/uart.h"
#include "../../Drivers/GPIO/inc/gpio.h"
#include "../../Drivers/I2C/inc/i2c.h"
#include <stdint.h>
#include <stddef.h>

typedef struct
{
    USART_HandleType usart;              // embedded USART handle (regs + config)
    UART_COMType hc06_com_type;
    UART_BaudRateType hc06_baud_rate;
} HC06_HandleType;

void HC06_constructor(HC06_HandleType *hc06,
                      USART_ManualType *usart_regs,
                      UART_COMType com_type,
                      UART_BaudRateType baud_rate);

// Function prototypes
// Send/receive raw bytes with explicit length
void HC06_SendData(HC06_HandleType *hc06, const uint8_t *data, size_t len);
void HC06_ReceiveData(HC06_HandleType *hc06, uint8_t *buffer, size_t len);
uint8_t HC06_ReceiveByte(HC06_HandleType *hc06);
const char* HC06_GetPortName(const HC06_HandleType *hc06); // return port name as string
void hc06_scan_i2c(HC06_HandleType *hc06, I2C_HandleType *hi2c); // Scan I2C bus and report ACKed addresses over Bluetooth

#endif // HC06_H