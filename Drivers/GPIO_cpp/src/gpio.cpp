#include "gpio.h"

#define GPIO_NUM 16
#define GPIO_MODER_MODE0 0x3U
#define GPIO_MODE 0x3U
#define GPIO_PUPDR_PUPD0 0x3U
#define GPIO_OSPEEDR_OSPEED0 0x3U
#define GPIO_OTYPER_OT0 0x1U
#define GPIO_OUTPUT_TYPE_MASK 0x10U

GPIO::GPIO(GPIO_ManualTypeDef *regs, GPIO_InitTypeDef *GPIO_Init)
: regs(regs), init(GPIO_Init) {

    __RCC_GPIO_CLK_ENABLE(regs); // Enable GPIO clock
    
    uint32_t position;
    uint32_t temp = 0x00U;
    uint32_t ioposition = 0x00U;
    uint32_t iocurrent = 0x00U;

    for(position = 0; position < GPIO_NUM; position++) {
        ioposition = (1U << position);
        iocurrent = (GPIO_Init->Pin & ioposition);

        if (iocurrent == ioposition) {
            
            // Alternate function selection 
            if (GPIO_Init->Mode == GPIO_MODE_AF_PP || GPIO_Init->Mode == GPIO_MODE_AF_OD) {
                temp = regs->AFR[position >> 3U];
                temp &= ~(0xFU << (((uint32_t)position & 0x07U) * 4U));
                temp |= ((GPIO_Init->Alternate & 0x0FU) << (((uint32_t)position & 0x07U) * 4U));
                regs->AFR[position >> 3U] = temp;
            }

            // Direction mode configuration
            temp = regs->MODER;
            temp &= ~(GPIO_MODER_MODE0 << (position * 2U));
            temp |= ((GPIO_Init->Mode & GPIO_MODE) << (position * 2U));
            regs->MODER = temp;

            // Alternate function selection 
            if ((GPIO_Init->Mode == GPIO_MODE_OUTPUT_PP) || (GPIO_Init->Mode == GPIO_MODE_OUTPUT_OD) ||
                (GPIO_Init->Mode == GPIO_MODE_AF_PP) || (GPIO_Init->Mode == GPIO_MODE_AF_OD)) {
                temp = regs->OSPEEDR;
                temp &= ~(GPIO_OSPEEDR_OSPEED0 << (position * 2U));
                temp |= (GPIO_Init->Speed  << (position * 2U));
                regs->OSPEEDR = temp;

                // Output type configuration
                temp = regs->OTYPER;
                temp &= ~(GPIO_OTYPER_OT0 << position);
                temp |= (((GPIO_Init->Mode & GPIO_OUTPUT_TYPE_MASK) >> 4U) << position);
                regs->OTYPER = temp;
            }

            // Pull-up Pull down resistor configuration
            temp = regs->PUPDR;
            temp &= ~(GPIO_PUPDR_PUPD0 << (position * 2U));
            temp |= ((GPIO_Init->Pull & GPIO_PUPDR_PUPD0) << (position * 2U));
            regs->PUPDR = temp;
        }
    }
}

// Read the specified input port pin state
GPIO_PinState GPIO::GPIO_ReadPin(uint16_t GPIO_Pin) {
    GPIO_PinState bitstatus;

    if ((this->regs->IDR & GPIO_Pin) != (uint32_t)GPIO_PIN_RESET) {
        bitstatus = GPIO_PIN_SET;
    } else {
        bitstatus = GPIO_PIN_RESET;
    }
    return bitstatus;
}

// Write to the specified output port pin
void GPIO::GPIO_WritePin(uint16_t GPIO_Pin, GPIO_PinState PinState) {
    if (PinState != GPIO_PIN_RESET) {
        this->regs->BSRR = (uint32_t)GPIO_Pin; // Set the pin
    } else {
        this->regs->BSRR = (uint32_t)(GPIO_Pin << 16U); // Reset the pin (upper half of BSRR)
    }
}

void GPIO::GPIO_TogglePin(uint16_t GPIO_Pin) {
    this->regs->ODR ^= GPIO_Pin; // Toggle the specified pin
}

void __RCC_GPIO_CLK_ENABLE(GPIO_ManualTypeDef *regs) {
    if (regs == GPIO_A) {
        RCC->AHB1ENR |= GPIOA_EN;
    } else if (regs == GPIO_B) {
        RCC->AHB1ENR |= GPIOB_EN;
    } else if (regs == GPIO_C) {
        RCC->AHB1ENR |= GPIOC_EN;
    } else if (regs == GPIO_D) {
        RCC->AHB1ENR |= GPIOD_EN;
    } else if (regs == GPIO_E) {
        RCC->AHB1ENR |= GPIOE_EN;
    } else if (regs == GPIO_F) {
        RCC->AHB1ENR |= GPIOF_EN;
    } else if (regs == GPIO_G) {
        RCC->AHB1ENR |= GPIOG_EN;
    } else if (regs == GPIO_H) {
        RCC->AHB1ENR |= GPIOH_EN;
    } else if (regs == GPIO_I) {
        RCC->AHB1ENR |= GPIOI_EN;
    }
}