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
        if (!LIS302DL_Init(&lis302dl, &CS_Handle, GPIO_InitStruct.Pin)) {
            USART_WriteString(&usart, "LIS302DL init failed\r\n");
        } else {
            // Read back CTRL1 for verification
            uint8_t ctrl1 = 0;
            if (LIS302DL_ReadReg(&lis302dl, &CS_Handle, GPIO_InitStruct.Pin, LIS302DL_REG_CTRL1, &ctrl1)) {
                char msg[64];
                snprintf(msg, sizeof(msg), "CTRL1=0x%02X\r\n", ctrl1);
                USART_WriteString(&usart, msg);
            }
        }
    } else {
        char msg[64];
        snprintf(msg, sizeof(msg), "WHO_AM_I read failed or unexpected: 0x%02X\r\n", who);
        USART_WriteString(&usart, msg);
    }

    while (1) {
        if (have_lis302) {
            int8_t x, y, z;
            if (LIS302DL_ReadXYZ(&lis302dl, &CS_Handle, GPIO_InitStruct.Pin, &x, &y, &z)) {
                char msg[96];
                snprintf(msg, sizeof(msg), "LIS302DL XYZ: %d, %d, %d\r\n", x, y, z);
                USART_WriteString(&usart, msg);
            } else {
                USART_WriteString(&usart, "LIS302DL read XYZ failed\r\n");
            }
        } else {
            // Periodically dump WHO_AM_I in all modes to diagnose interface
            for (uint8_t m = 0; m < 4; ++m) {
                uint8_t w = 0; char dbg[48];
                if (LIS302DL_ReadWhoAmI_Mode(&lis302dl, &CS_Handle, GPIO_InitStruct.Pin, m, &w)) {
                    snprintf(dbg, sizeof(dbg), "WHO(m%d)=0x%02X\r\n", m, w);
                } else {
                    snprintf(dbg, sizeof(dbg), "WHO(m%d)=<err>\r\n", m);
                }
                USART_WriteString(&usart, dbg);
            }
        }
        delay(2000000); // Delay between reads
    }
}

void delay(volatile uint32_t count) {
    while(count--) {
        __asm__("nop");
    }
}
