#ifndef LCD_H
#define LCD_H
#include "../../GPIO/inc/gpio.h"
#include <stdint.h>
#include <string.h>

// LCD handle structure uses GPIO driver handle directly
typedef struct {
    GPIO_HandleTypeDef rs;        // RS pin GPIO handle (init->Pin selects pin)
    GPIO_HandleTypeDef en;        // EN pin GPIO handle
    GPIO_HandleTypeDef dataPins[4]; // D4-D7 GPIO handles
} LCD_HandleTypeDef;

void LCD_Init(LCD_HandleTypeDef* lcd,
              GPIO_HandleTypeDef rs,
              GPIO_HandleTypeDef en,
              GPIO_HandleTypeDef d4,
              GPIO_HandleTypeDef d5,
              GPIO_HandleTypeDef d6,
              GPIO_HandleTypeDef d7);
void LCD_SetCursor(LCD_HandleTypeDef* lcd, uint8_t row, uint8_t column);
void LCD_PrintChar(LCD_HandleTypeDef* lcd, char data);
void LCD_PrintString(LCD_HandleTypeDef* lcd, const char* pData);
void LCD_PrintInt(LCD_HandleTypeDef* lcd, uint32_t data);
void LCD_Clear(LCD_HandleTypeDef* lcd);
void delay(volatile uint32_t count);

#endif //LCD_H