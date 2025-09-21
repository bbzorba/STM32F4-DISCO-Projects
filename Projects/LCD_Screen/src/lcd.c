#include "lcd.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include <string.h>

enum LCDCommands {
    FUNCTION_SET_8BIT = 0x03,
    FUNCTION_SET_4BIT = 0x02,
    FUNCTION_SET_2LINE_5x8DOT = 0x28,
    CLEAR_DISPLAY = 0x01,
    DISPLAY_ON_CURSOR_ON = 0x0E,
    DISPLAY_ON_CURSOR_OFF = 0x0C,
    ENTRY_MODE_INCREMENT_CURSOR = 0x06
};

enum NibbleBitPosition {
    LOW_NIBBLE = 0,
    HIGH_NIBBLE = 4
};

const uint8_t ddramAddr[4][20] = {
    {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10,0x11,0x12,0x13},
    {0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F,0x50,0x51,0x52,0x53},
    {0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27},
    {0x54,0x55,0x56,0x57,0x58,0x59,0x5A,0x5B,0x5C,0x5D,0x5E,0x5F,0x60,0x61,0x62,0x63,0x64,0x65,0x66,0x67}
};

static void IntegerToString(uint32_t integer, char* pBuffer) {
    if (integer == 0) {
        pBuffer[0] = '0';
        pBuffer[1] = '\0';
        return;
    }
    uint32_t copyOfInt = integer;
    uint8_t noOfDigits = 0;
    //get number of digits
    while (copyOfInt > 0) {
        copyOfInt /= 10;
        noOfDigits++;
    }
    pBuffer[noOfDigits] = '\0';
    //get each digit, convert to char, store in buffer
    while (integer > 0) {
        pBuffer[noOfDigits - 1] = '0' + (integer % 10);
        integer /= 10;
        noOfDigits--;
    }
}

static void LCD_WriteNibble(LCD_HandleTypeDef* lcd, char byte, uint8_t nibbleBitPos) {
    const GPIO_PinState pinState[2] = {GPIO_PIN_RESET, GPIO_PIN_SET};
    uint8_t nibbleArr[4] = {0};
    uint8_t j = 0;
    
    for (uint8_t i = nibbleBitPos; i < nibbleBitPos + 4; i++) {
        nibbleArr[j] = (byte & (1 << i)) >> i;
        j++;
    }
    //Send nibble
    for (uint8_t i = 0; i < 4; i++) {
        HAL_GPIO_WritePin(lcd->dataPins[i].port, lcd->dataPins[i].selectedPin, pinState[nibbleArr[i]]);
    }
    //High to low pulse on EN pin (to transfer nibble)
    HAL_GPIO_WritePin(lcd->en.port, lcd->en.selectedPin, GPIO_PIN_SET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(lcd->en.port, lcd->en.selectedPin, GPIO_PIN_RESET);
    HAL_Delay(1);
}

static void LCD_WriteByte(LCD_HandleTypeDef* lcd, GPIO_PinState lcdMode, char byte) {
    HAL_GPIO_WritePin(lcd->rs.port, lcd->rs.selectedPin, lcdMode); //Register select
    LCD_WriteNibble(lcd, byte, HIGH_NIBBLE);
    LCD_WriteNibble(lcd, byte, LOW_NIBBLE);
}

static void LCD_WriteBytes(LCD_HandleTypeDef* lcd, const char* pData) {
    while (*pData != '\0') {
        LCD_WriteByte(lcd, GPIO_PIN_SET, *pData);
        pData++;
    }
}

static void LCD_WriteInteger(LCD_HandleTypeDef* lcd, uint32_t data) {
    const uint8_t maxNumberOfDigits = 10;
    char integerToStringBuffer[maxNumberOfDigits + 1];
    memset(integerToStringBuffer, 0, sizeof(integerToStringBuffer));
    if (data < 10) {
        LCD_WriteByte(lcd, GPIO_PIN_SET, '0');
    }
    IntegerToString(data, integerToStringBuffer);
    LCD_WriteBytes(lcd, integerToStringBuffer);
}

void LCD_Init(LCD_HandleTypeDef* lcd,
              pinStruct_t rs,
              pinStruct_t en,
              pinStruct_t d4,
              pinStruct_t d5,
              pinStruct_t d6,
              pinStruct_t d7) {
    //LCD pin initialization
    GPIO_InitTypeDef lcdPinInitStruct[6] = {0};
    pinStruct_t lcdPinArray[6] = {rs, en, d4, d5, d6, d7};
    for (uint8_t i = 0; i < 6; i++) {
        lcdPinInitStruct[i].Pin = lcdPinArray[i].selectedPin;
        lcdPinInitStruct[i].Mode = GPIO_MODE_OUTPUT_PP;
        HAL_GPIO_Init(lcdPinArray[i].port, &lcdPinInitStruct[i]);
    }
    //Private variable initialization
    lcd->rs = rs;
    lcd->en = en;
    lcd->dataPins[0] = d4;
    lcd->dataPins[1] = d5;
    lcd->dataPins[2] = d6;
    lcd->dataPins[3] = d7;
    
    //LCD Initialization sequence according to the datasheet
    HAL_Delay(16); //Power-on delay
    LCD_WriteByte(lcd, GPIO_PIN_RESET, FUNCTION_SET_8BIT);//Function set mode (8-bit mode)
    HAL_Delay(5); //wait for more than 4.1ms
    LCD_WriteByte(lcd, GPIO_PIN_RESET, FUNCTION_SET_8BIT);//Function set mode (8-bit mode)
    HAL_Delay(1); //wait for more than 100us
    
    //4-bit operation commands
    uint8_t fourBitCommandSetting[5] = 
    {
        FUNCTION_SET_4BIT,
        FUNCTION_SET_2LINE_5x8DOT,
        CLEAR_DISPLAY,
        DISPLAY_ON_CURSOR_OFF,
        ENTRY_MODE_INCREMENT_CURSOR
    };
    for (uint8_t i = 0; i < 5; i++) {
        LCD_WriteByte(lcd, GPIO_PIN_RESET, fourBitCommandSetting[i]);
    }
}

void LCD_SetCursor(LCD_HandleTypeDef* lcd, uint8_t row, uint8_t column) {
    if ((row > 3) || (column > 19)) {
        //Out of range
        return;
    }
    LCD_WriteByte(lcd, GPIO_PIN_RESET, ((1 << 7) | ddramAddr[row][column]));
}

void LCD_PrintChar(LCD_HandleTypeDef* lcd, char data) {
    LCD_WriteByte(lcd, GPIO_PIN_SET, data);
}

void LCD_PrintString(LCD_HandleTypeDef* lcd, const char* pData) {
    LCD_WriteBytes(lcd, pData);
}

void LCD_PrintInt(LCD_HandleTypeDef* lcd, uint32_t data) {
    LCD_WriteInteger(lcd, data);
}

void LCD_Clear(LCD_HandleTypeDef* lcd) {
    LCD_WriteByte(lcd, GPIO_PIN_RESET, CLEAR_DISPLAY);
}
