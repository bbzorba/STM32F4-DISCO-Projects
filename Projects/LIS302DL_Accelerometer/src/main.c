#include "../inc/lis302dl.h"
#include <stdint.h>
#include <stdio.h>

// Function prototypes
void delay(volatile uint32_t count);

GPIO_InitTypeDef GPIO_InitStruct;
GPIO_HandleTypeDef CS_Handle;
SPI_HandleType lis302dl;

//main function
int main(void) {
    // Configure GPIO_InitStruct for CS pin (PE3)
    GPIO_InitStruct.Pin = GPIO_PIN_3; // CS pin
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;  // Push-pull output mode
    GPIO_InitStruct.Pull = GPIO_NOPULL;          // No pull-up/pull-down
    GPIO_InitStruct.Speed = GPIO_SPEED_MEDIUM;   // Medium speed
    GPIO_constructor(&CS_Handle, GPIO_E, &GPIO_InitStruct);

    // Ensure CS is deasserted high before starting transactions
    // (CS was forced low very early in startup to bias SPI mode at power-up)
    SPI_CS_High(&CS_Handle, GPIO_InitStruct.Pin);

    SPI_Init(&lis302dl, SPI_1, SPI1_PORTA, SPI_MODE_MASTER, SPI_BAUDRATE_DIV256, SPI_DIRECTION_2LINES);

    USART_HandleType usart;
    USART_constructor(&usart, USART_2, TX_ONLY, __115200);

    USART_WriteString(&usart, "LIS302DL Accelerometer Test\r\n");
    // Power-up delay
    delay(500000);

    // Probe once and initialize LIS302DL
    uint8_t who = 0;
    uint8_t have_lis302 = 0;
    // Log WHO_AM_I per SPI mode for diagnostics
    for (uint8_t m = 0; m < 4; ++m) {
        uint8_t w = 0; char dbg[48];
        if (LIS302DL_ReadWhoAmI_Mode(&lis302dl, &CS_Handle, GPIO_InitStruct.Pin, m, &w)) {
            snprintf(dbg, sizeof(dbg), "WHO(m%d)=0x%02X\r\n", m, w);
        } else {
            snprintf(dbg, sizeof(dbg), "WHO(m%d)=<err>\r\n", m);
        }
        USART_WriteString(&usart, dbg);
    }
    if (LIS302DL_ReadWhoAmI(&lis302dl, &CS_Handle, GPIO_InitStruct.Pin, &who) && who == LIS302DL_WHOAMI_EXPECTED) {
        have_lis302 = 1;
        USART_WriteString(&usart, "LIS302DL detected (WHO_AM_I=0x3B). Initializing...\r\n");
    } else {
        char msg[64];
        snprintf(msg, sizeof(msg), "WHO_AM_I unexpected: 0x%02X (continuing)\r\n", who);
        USART_WriteString(&usart, msg);
    }
    // Choose the best-working SPI mode
    if (LIS302DL_SelectBestSpiMode(&lis302dl, &CS_Handle, GPIO_InitStruct.Pin)) {
        char msg[48];
        snprintf(msg, sizeof(msg), "SPI mode locked via WHO: %u\r\n", (unsigned)LIS302DL_GetSpiMode());
        USART_WriteString(&usart, msg);
    } else {
        char msg[48];
        snprintf(msg, sizeof(msg), "SPI mode heuristic pick: %u\r\n", (unsigned)LIS302DL_GetSpiMode());
        USART_WriteString(&usart, msg);
    }
    // Attempt init regardless to start streaming XYZ
    if (!LIS302DL_Init(&lis302dl, &CS_Handle, GPIO_InitStruct.Pin)) {
        USART_WriteString(&usart, "LIS302DL init failed\r\n");
    } else {
        uint8_t ctrl1 = 0;
        if (LIS302DL_ReadReg(&lis302dl, &CS_Handle, GPIO_InitStruct.Pin, LIS302DL_REG_CTRL1, &ctrl1)) {
            char msg[64];
            snprintf(msg, sizeof(msg), "CTRL1=0x%02X\r\n", ctrl1);
            USART_WriteString(&usart, msg);
        }
    }

    // Calibrate while stationary
    if (LIS302DL_Calibrate(&lis302dl, &CS_Handle, GPIO_InitStruct.Pin, 128)) {
        int16_t ox, oy, oz; uint8_t sens;
        LIS302DL_GetCalibration(&ox, &oy, &oz, &sens);
        char cmsg[96];
        snprintf(cmsg, sizeof(cmsg), "Cal offsets: %d,%d,%d mg/LSB=%u\r\n", ox, oy, oz, sens);
        USART_WriteString(&usart, cmsg);
    } else {
        USART_WriteString(&usart, "Calibration failed\r\n");
    }

    while (1) {
        int8_t xr, yr, zr;
        if (LIS302DL_ReadXYZ(&lis302dl, &CS_Handle, GPIO_InitStruct.Pin, &xr, &yr, &zr)) {
            int16_t ox, oy, oz; uint8_t sens;
            LIS302DL_GetCalibration(&ox, &oy, &oz, &sens);
            int16_t xmg = (int16_t)(((int16_t)xr - ox) * sens);
            int16_t ymg = (int16_t)(((int16_t)yr - oy) * sens);
            int16_t zmg = (int16_t)(((int16_t)zr - oz) * sens);
            char msg[128];
            snprintf(msg, sizeof(msg), "LIS302DL mg: %d, %d, %d (raw %d,%d,%d)\r\n", xmg, ymg, zmg, xr, yr, zr);
            USART_WriteString(&usart, msg);
        } else {
            USART_WriteString(&usart, "LIS302DL read XYZ failed\r\n");
        }
        delay(2000000); // Delay between reads
    }
}

void delay(volatile uint32_t count) {
    while(count--) {
        __asm__("nop");
    }
}
