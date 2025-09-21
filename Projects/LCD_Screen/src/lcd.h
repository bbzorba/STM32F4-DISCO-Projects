#ifndef LCD_H
#define LCD_H
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include <stdint.h>

// LCD pin structure
typedef struct {
    GPIO_TypeDef* port;
    uint16_t selectedPin;
} pinStruct_t;

// LCD handle structure
typedef struct {
    pinStruct_t rs;
    pinStruct_t en;
    pinStruct_t dataPins[4];
} LCD_HandleTypeDef;

void LCD_Init(LCD_HandleTypeDef* lcd,
              pinStruct_t rs,
              pinStruct_t en,
              pinStruct_t d4,
              pinStruct_t d5,
              pinStruct_t d6,
              pinStruct_t d7);
void LCD_SetCursor(LCD_HandleTypeDef* lcd, uint8_t row, uint8_t column);
void LCD_PrintChar(LCD_HandleTypeDef* lcd, char data);
void LCD_PrintString(LCD_HandleTypeDef* lcd, const char* pData);
void LCD_PrintInt(LCD_HandleTypeDef* lcd, uint32_t data);
void LCD_Clear(LCD_HandleTypeDef* lcd);

#endif //LCD_H