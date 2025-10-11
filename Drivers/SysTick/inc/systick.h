#ifndef __SYSTICK_H
#define __SYSTICK_H

#include <stdint.h>
#include "../../GPIO/inc/gpio.h"

#define __IOM volatile /* Defines read/write permissions */
#define __IM  volatile const /* Defines read only permissions */
#define __OM  volatile       /* Defines write only permissions */

#define SYSTEM_CTR_BASE (0xE000E000UL)                            /* System Control Base Address */
#define SYSTICK_OFFSET (0x0010UL)                            /* SysTick Offset Address */
#define SYSTICK_BASE (SYSTEM_CTR_BASE + SYSTICK_OFFSET)         /* SysTick Base Address */

#define SysTick_CTRL_COUNTFLAG_Msk (1UL << 16)                   /*!< SysTick CTRL: COUNTFLAG Mask */

typedef struct
{
  __IOM uint32_t CTRL;                   /*!< Offset: 0x000 (R/W)  SysTick Control and Status Register */
  __IOM uint32_t LOAD;                   /*!< Offset: 0x004 (R/W)  SysTick Reload Value Register       */
  __IOM uint32_t VAL;                    /*!< Offset: 0x008 (R/W)  SysTick Current Value Register      */
  __IM  uint32_t CALIB;                  /*!< Offset: 0x00C (R/ )  SysTick Calibration Register        */
} SysTick_Type;

#define SysTick ((SysTick_Type *)SYSTICK_BASE)   /*!< SysTick configuration struct */

void SysTick_delay(volatile uint32_t sec);
void SysTick_delay_ms(volatile uint32_t ms);

#endif /* __SYSTICK_H */