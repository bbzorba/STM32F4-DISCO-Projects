#ifndef __COMPAT_STM32F4XX_GPIO_H__
#define __COMPAT_STM32F4XX_GPIO_H__

// The project references stm32f4xx_gpio.h, but the repo has stm32f4xx_hal_gpio.h
// Provide a thin compatibility wrapper that includes the HAL header so IntelliSense resolves.
#include "../STM32F4xx_HAL_Driver/stm32f4xx_hal_gpio.h"

#endif // __COMPAT_STM32F4XX_GPIO_H__
