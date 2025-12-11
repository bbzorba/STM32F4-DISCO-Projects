#include "../inc/lcd.h"

enum LCDCommands {
    FUNCTION_SET_8BIT = 0x03,
    FUNCTION_SET_4BIT = 0x02,
    FUNCTION_SET_2LINE_5x8DOT = 0x28,
    DISPLAY_OFF = 0x08,
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

// Timing helpers calibrated for ~16 MHz HSI
static void delay_us(volatile uint32_t us)
{
    // Approximate: ~10 cycles per loop -> at 16 MHz, ~0.625us per iteration
    // Scale to achieve ~1us per unit
    while (us--) {
        volatile uint32_t c = 20; // tune factor
        while (c--) { __asm__("nop"); }
    }
}

static void delay_ms(volatile uint32_t ms)
{
    while (ms--) { delay_us(1000); }
}

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
    //Send nibble: set data lines before toggling EN
    for (uint8_t i = 0; i < 4; i++) {
        GPIO_WritePin(&lcd->dataPins[i], (uint16_t)lcd->dataPins[i].init->Pin, pinState[nibbleArr[i]]);
    }
    //High to low pulse on EN pin (to transfer nibble)
    GPIO_WritePin(&lcd->en, (uint16_t)lcd->en.init->Pin, GPIO_PIN_SET);
    delay_us(5); // >450ns
    GPIO_WritePin(&lcd->en, (uint16_t)lcd->en.init->Pin, GPIO_PIN_RESET);
    delay_us(5);
}

static void LCD_WriteByte(LCD_HandleTypeDef* lcd, GPIO_PinState lcdMode, char byte) {
    GPIO_WritePin(&lcd->rs, (uint16_t)lcd->rs.init->Pin, lcdMode); // Register select
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
              GPIO_HandleTypeDef rs,
              GPIO_HandleTypeDef en,
              GPIO_HandleTypeDef d4,
              GPIO_HandleTypeDef d5,
              GPIO_HandleTypeDef d6,
              GPIO_HandleTypeDef d7) {
    // Assign handles (assumed pre-configured as outputs)
    lcd->rs = rs;
    lcd->en = en;
    lcd->dataPins[0] = d4;
    lcd->dataPins[1] = d5;
    lcd->dataPins[2] = d6;
    lcd->dataPins[3] = d7;
    // Ensure initial pin states are low
    GPIO_WritePin(&lcd->rs, (uint16_t)lcd->rs.init->Pin, GPIO_PIN_RESET);
    GPIO_WritePin(&lcd->en, (uint16_t)lcd->en.init->Pin, GPIO_PIN_RESET);
    for (uint8_t i = 0; i < 4; i++) {
        GPIO_WritePin(&lcd->dataPins[i], (uint16_t)lcd->dataPins[i].init->Pin, GPIO_PIN_RESET);
    }
    
    // LCD Initialization sequence according to the datasheet
    delay_ms(40); // Power-on delay (>30ms)
    // Send 0x03 three times in 8-bit mode using nibbles
    LCD_WriteNibble(lcd, FUNCTION_SET_8BIT, LOW_NIBBLE);
    delay_ms(5); // >4.1ms
    LCD_WriteNibble(lcd, FUNCTION_SET_8BIT, LOW_NIBBLE);
    delay_us(150); // >100us
    LCD_WriteNibble(lcd, FUNCTION_SET_8BIT, LOW_NIBBLE);
    delay_us(150);
    
    // Switch to 4-bit mode
    LCD_WriteNibble(lcd, FUNCTION_SET_4BIT, LOW_NIBBLE);
    delay_us(150);

    // Function set: 4-bit, 2 line, 5x8 dots
    LCD_WriteByte(lcd, GPIO_PIN_RESET, FUNCTION_SET_2LINE_5x8DOT);
    // Display OFF, then clear, then entry mode, then display ON
    LCD_WriteByte(lcd, GPIO_PIN_RESET, DISPLAY_OFF);
    delay_ms(2);
    LCD_WriteByte(lcd, GPIO_PIN_RESET, CLEAR_DISPLAY);
    delay_ms(2);
    LCD_WriteByte(lcd, GPIO_PIN_RESET, ENTRY_MODE_INCREMENT_CURSOR);
    LCD_WriteByte(lcd, GPIO_PIN_RESET, DISPLAY_ON_CURSOR_OFF);
    
    // Ready for data
    delay_us(50);
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

void delay(volatile uint32_t count) {
    while(count--) {
        __asm__("nop");
    }
}