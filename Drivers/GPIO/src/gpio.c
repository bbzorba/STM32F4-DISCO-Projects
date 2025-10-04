#include "gpio.h"

#define GPIO_NUM 16
#define GPIO_MODER_MODE0 0x3U
#define GPIO_MODE 0x3U
#define GPIO_PUPDR_PUPD0 0x3U

void LIB_GPIO_Init(GPIO_TypeDef *GPIOx, GPIO_InitTypeDef *GPIO_Init) {
    uint32_t position;
    uint32_t temp = 0x00U;
    uint32_t ioposition = 0x00U;
    uint32_t iocurrent = 0x00U;

    for(position = 0; position < GPIO_NUM; position++) {
        ioposition = (1U << position);
        iocurrent = (GPIO_Init->Pin & ioposition);

        if (iocurrent == ioposition)
        {
            temp = GPIOx->MODER;
            temp &= ~(GPIO_MODER_MODE0 << (position * 2U));
            temp |= ((GPIO_Init->Mode & GPIO_MODE) << (position * 2U));
            GPIOx->MODER = temp;

            temp = GPIOx->PUPDR;
            temp &= ~(GPIO_PUPDR_PUPD0 << (position * 2U));
            temp |= ((GPIO_Init->Pull & GPIO_PUPDR_PUPD0) << (position * 2U));
            GPIOx->PUPDR = temp;
        }
    }
}

GPIO_PinState LIB_GPIO_ReadPin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
    GPIO_PinState bitstatus;

    if ((GPIOx->IDR & GPIO_Pin) != (uint32_t)GPIO_PIN_RESET) {
        bitstatus = GPIO_PIN_SET;
    } else {
        bitstatus = GPIO_PIN_RESET;
    }
    return bitstatus;
}

void LIB_GPIO_WritePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState) {
    if (PinState != GPIO_PIN_RESET) {
        GPIOx->BSRR = (uint32_t)GPIO_Pin; // Set the pin
    } else {
        GPIOx->BSRR = (uint32_t)(GPIO_Pin << 16U); // Reset the pin (upper half of BSRR)
    }
}

void LIB_GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
    GPIOx->ODR ^= GPIO_Pin; // Toggle the specified pin
}

void __LIB_RCC_GPIOA_CLK_ENABLE(void) {
    RCC->RCC_AHB1ENR |= GPIOA_EN;
}

void __LIB_RCC_GPIOB_CLK_ENABLE(void) {
    RCC->RCC_AHB1ENR |= GPIOB_EN;
}

void __LIB_RCC_GPIOC_CLK_ENABLE(void) {
    RCC->RCC_AHB1ENR |= GPIOC_EN;
}

void __LIB_RCC_GPIOD_CLK_ENABLE(void) {
    RCC->RCC_AHB1ENR |= GPIOD_EN;
}

void __LIB_RCC_GPIOE_CLK_ENABLE(void) {
    RCC->RCC_AHB1ENR |= GPIOE_EN;
}

void __LIB_RCC_GPIOF_CLK_ENABLE(void) {
    RCC->RCC_AHB1ENR |= GPIOF_EN;
}

void __LIB_RCC_GPIOG_CLK_ENABLE(void) {
    RCC->RCC_AHB1ENR |= GPIOG_EN;
}

void __LIB_RCC_GPIOH_CLK_ENABLE(void) {
    RCC->RCC_AHB1ENR |= GPIOH_EN;
}
