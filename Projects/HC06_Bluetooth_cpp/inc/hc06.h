#ifndef HC06_H
#define HC06_H

#include "../../Drivers/UART_cpp/inc/uart.h"
#include "../../Drivers/GPIO_cpp/inc/gpio.h"
#include "../../Drivers/I2C_cpp/inc/i2c.h"
#include <stdint.h>
#include <stddef.h>

class HC06
{
private:
    USART usart;              // embedded USART handle (regs + config)
    UART_COMType hc06_com_type;
    UART_BaudRateType hc06_baud_rate;
public:
    HC06(USART_ManualType *usart_regs,
         UART_COMType com_type,
         UART_BaudRateType baud_rate);
    void HC06_SendData(const uint8_t *data, size_t len);
    void HC06_ReceiveData(uint8_t *buffer, size_t len);
    uint8_t HC06_ReceiveByte();
    const char* HC06_GetPortName(); // return port name as string

};

// Function prototypes
// Send/receive raw bytes with explicit length
void hc06_scan_i2c(HC06 *hc06, I2C *hi2c); // Scan I2C bus and report ACKed addresses over Bluetooth

#endif // HC06_H