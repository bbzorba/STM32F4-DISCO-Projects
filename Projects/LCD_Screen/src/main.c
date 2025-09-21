#include "lcd.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx.h"
#include "stm32f4xx_hal_conf.h"
#include "main.h"

#ifndef __HAL_RCC_GPIOA_CLK_ENABLE
#define __HAL_RCC_GPIOA_CLK_ENABLE() (RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN)
#endif

void Init(void);

static LCD_HandleTypeDef lcd;

int main(void)
{
    Init();

    while (1)
    {
        LCD_PrintString(&lcd, "Hello world!");
        HAL_Delay(1000);
        LCD_Clear(&lcd);
        HAL_Delay(1000);
        LCD_PrintString(&lcd, "Random Stuff:");
        LCD_SetCursor(&lcd, 1, 0);
        LCD_PrintString(&lcd, "!'^+%&/()=?_;");
        HAL_Delay(1000);
        LCD_Clear(&lcd);
    }
}

void Init(void) {
    HAL_Init();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    pinStruct_t rs = {GPIOA, GPIO_PIN_0};
    pinStruct_t en = {GPIOA, GPIO_PIN_1};
    pinStruct_t d4 = {GPIOA, GPIO_PIN_4};
    pinStruct_t d5 = {GPIOA, GPIO_PIN_5};
    pinStruct_t d6 = {GPIOA, GPIO_PIN_6};
    pinStruct_t d7 = {GPIOA, GPIO_PIN_7};

    LCD_Init(&lcd, rs, en, d4, d5, d6, d7);
}