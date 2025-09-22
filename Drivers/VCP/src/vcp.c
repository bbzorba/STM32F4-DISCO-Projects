// Simple USB OTG FS device bring-up using HAL PCD.
// NOTE: This does not yet implement the CDC class; it will not enumerate until descriptors are provided.
#include "vcp.h"
#include "stm32f4xx_hal.h"

static PCD_HandleTypeDef hpcd_USB_OTG_FS;

// OTG FS global IRQ handler: forward to HAL
void OTG_FS_IRQHandler(void) {
    HAL_PCD_IRQHandler(&hpcd_USB_OTG_FS);
}

// Minimal MSP init to enable clocks and NVIC; HAL_PCD_Init will call this.
void HAL_PCD_MspInit(PCD_HandleTypeDef *hpcd)
{
    if (hpcd->Instance == USB_OTG_FS) {
    __GPIOA_CLK_ENABLE();
    __USB_OTG_FS_CLK_ENABLE();

        // PA11 USB_DM, PA12 USB_DP (AF10)
        GPIO_InitTypeDef GPIO_InitStruct = {0};
        GPIO_InitStruct.Pin = GPIO_PIN_11 | GPIO_PIN_12;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF10_OTG_FS;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        // Enable OTG FS IRQ
        NVIC_SetPriority(OTG_FS_IRQn, 5);
        NVIC_EnableIRQ(OTG_FS_IRQn);
    }
}

void HAL_PCD_MspDeInit(PCD_HandleTypeDef *hpcd)
{
    if (hpcd->Instance == USB_OTG_FS) {
    __USB_OTG_FS_CLK_DISABLE();
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_11 | GPIO_PIN_12);
        NVIC_DisableIRQ(OTG_FS_IRQn);
    }
}

void VCP_Init(void) {
    // Initialize HAL (sets SysTick timebase, NVIC priority grouping)
    HAL_Init();

    hpcd_USB_OTG_FS.Instance = USB_OTG_FS;
    hpcd_USB_OTG_FS.Init.dev_endpoints = 4;
    hpcd_USB_OTG_FS.Init.speed = PCD_SPEED_FULL;
    hpcd_USB_OTG_FS.Init.phy_itface = PCD_PHY_EMBEDDED;
    hpcd_USB_OTG_FS.Init.Sof_enable = ENABLE;
    hpcd_USB_OTG_FS.Init.low_power_enable = DISABLE;
    hpcd_USB_OTG_FS.Init.vbus_sensing_enable = DISABLE;
    hpcd_USB_OTG_FS.Init.use_dedicated_ep1 = DISABLE;
    hpcd_USB_OTG_FS.Init.use_external_vbus = DISABLE;
    hpcd_USB_OTG_FS.Init.dma_enable = DISABLE;
    hpcd_USB_OTG_FS.Init.ep0_mps = 64;
    if (HAL_PCD_Init(&hpcd_USB_OTG_FS) == HAL_OK) {
        // Configure basic FIFOs (sizes in 32-bit words); minimal values to avoid HAL errors
        HAL_PCD_SetRxFiFo(&hpcd_USB_OTG_FS, 128);
        HAL_PCD_SetTxFiFo(&hpcd_USB_OTG_FS, 0, 64);
        HAL_PCD_SetTxFiFo(&hpcd_USB_OTG_FS, 1, 64);
        HAL_PCD_Start(&hpcd_USB_OTG_FS);
    }
}

void VCP_Task(void) {
    // Placeholder; a full CDC requires descriptors and class driver.
}

// Weak callbacks to satisfy HAL if invoked before class stack is in place
void HAL_PCD_SetupStageCallback(PCD_HandleTypeDef *hpcd) {}
void HAL_PCD_DataOutStageCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum) {}
void HAL_PCD_DataInStageCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum) {}
void HAL_PCD_SOFCallback(PCD_HandleTypeDef *hpcd) {}
void HAL_PCD_ResetCallback(PCD_HandleTypeDef *hpcd) {}
void HAL_PCD_SuspendCallback(PCD_HandleTypeDef *hpcd) {}
void HAL_PCD_ResumeCallback(PCD_HandleTypeDef *hpcd) {}
void HAL_PCD_ISOOUTIncompleteCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum) {}
void HAL_PCD_ISOINIncompleteCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum) {}
void HAL_PCD_ConnectCallback(PCD_HandleTypeDef *hpcd) {}
void HAL_PCD_DisconnectCallback(PCD_HandleTypeDef *hpcd) {}
