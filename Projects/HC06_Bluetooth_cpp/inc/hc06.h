#ifndef HC06_H
#define HC06_H

#include "../../Drivers/UART_cpp/inc/uart.h"
#include "../../Drivers/GPIO_cpp/inc/gpio.h"
#include <stdint.h>
#include <stddef.h>

class HC06
{
private:
    UART_COMType hc06_com_type;
    UART_BaudRateType hc06_baud_rate;
    UART uart;

public:
    HC06(UART_COMType com_type, UART_BaudRateType baud_rate);

    // Function prototypes
    void SendData(const uint8_t *data, size_t len);
    void ReceiveData(uint8_t *buffer, size_t len);
    // Convenience: read a single byte
    uint8_t ReceiveData();
};

#endif // HC06_H