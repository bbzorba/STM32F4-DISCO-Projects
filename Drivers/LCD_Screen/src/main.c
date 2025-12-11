#include "../inc/lcd.h"

void Init(void);

static LCD_HandleTypeDef lcd;

int main(void)
{
    Init();

    while (1)
    {
        LCD_PrintString(&lcd, "Hello world!");
        delay(1000);
        LCD_Clear(&lcd);
        delay(1000);
        LCD_PrintString(&lcd, "Random Stuff:");
        LCD_SetCursor(&lcd, 1, 0);
        LCD_PrintString(&lcd, "!'^+&/()=?_;");
        delay(1000);
        LCD_Clear(&lcd);
    }
}

void Init(void) {
    // Configure GPIOA clock and pins using our GPIO driver
    GPIO_InitTypeDef init = {0};
    GPIO_HandleTypeDef gpioA_rs, gpioA_en, gpioA_d4, gpioA_d5, gpioA_d6, gpioA_d7;

    // Common config
    init.Mode = GPIO_MODE_OUTPUT_PP;
    init.Pull = GPIO_NOPULL;
    init.Speed = GPIO_SPEED_LOW;
    init.Alternate = 0;

    // RS PA0
    init.Pin = GPIO_PIN_0;
    GPIO_constructor(&gpioA_rs, GPIO_A, &init);
    __RCC_GPIO_CLK_ENABLE(&gpioA_rs);
    GPIO_Init(&gpioA_rs);

    // EN PA1
    init.Pin = GPIO_PIN_1;
    GPIO_constructor(&gpioA_en, GPIO_A, &init);
    __RCC_GPIO_CLK_ENABLE(&gpioA_en);
    GPIO_Init(&gpioA_en);

    // D4 PA4
    init.Pin = GPIO_PIN_4;
    GPIO_constructor(&gpioA_d4, GPIO_A, &init);
    __RCC_GPIO_CLK_ENABLE(&gpioA_d4);
    GPIO_Init(&gpioA_d4);

    // D5 PA5
    init.Pin = GPIO_PIN_5;
    GPIO_constructor(&gpioA_d5, GPIO_A, &init);
    __RCC_GPIO_CLK_ENABLE(&gpioA_d5);
    GPIO_Init(&gpioA_d5);

    // D6 PA6
    init.Pin = GPIO_PIN_6;
    GPIO_constructor(&gpioA_d6, GPIO_A, &init);
    __RCC_GPIO_CLK_ENABLE(&gpioA_d6);
    GPIO_Init(&gpioA_d6);

    // D7 PA7
    init.Pin = GPIO_PIN_7;
    GPIO_constructor(&gpioA_d7, GPIO_A, &init);
    __RCC_GPIO_CLK_ENABLE(&gpioA_d7);
    GPIO_Init(&gpioA_d7);

    // Initialize LCD with GPIO handles
    LCD_Init(&lcd, gpioA_rs, gpioA_en, gpioA_d4, gpioA_d5, gpioA_d6, gpioA_d7);
}
