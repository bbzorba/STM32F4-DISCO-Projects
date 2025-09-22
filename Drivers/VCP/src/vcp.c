// Simple USB CDC ACM device (Virtual COM Port) implemented using HAL PCD only.
// Enumerates as a CDC device on Windows 10+ without extra drivers.
#include "vcp.h"
#include "stm32f4xx_hal.h"
#include <string.h>
#include <stdint.h>

static PCD_HandleTypeDef hpcd_USB_OTG_FS;

// CDC endpoints (FS): EP1 IN (interrupt), EP2 IN (bulk), EP3 OUT (bulk)
#define CDC_INT_EP        0x81
#define CDC_DATA_IN_EP    0x82
#define CDC_DATA_OUT_EP   0x03
#define CDC_MAX_PACKET    64

// Line coding structure for CDC (baud rate etc.)
typedef struct __attribute__((packed)) {
    uint32_t dwDTERate; // baud
    uint8_t  bCharFormat; // stop bits (0=1,1=1.5,2=2)
    uint8_t  bParityType; // 0=None,1=Odd,2=Even,3=Mark,4=Space
    uint8_t  bDataBits;   // data bits (5,6,7,8)
} line_coding_t;

static volatile uint8_t configured = 0;
static volatile uint8_t dtr = 0; // from SET_CONTROL_LINE_STATE
static volatile uint8_t set_address_pending = 0;
static volatile uint8_t pending_address = 0;
static line_coding_t g_line = {115200,0,0,8};

// Simple OUT buffer
static uint8_t out_buf[CDC_MAX_PACKET];

// Descriptors
static const uint8_t device_desc[] = {
    18, 0x01,             // bLength, bDescriptorType(Device)
    0x00,0x02,            // bcdUSB 2.00
    0x02,                 // bDeviceClass (CDC)
    0x00,                 // bDeviceSubClass
    0x00,                 // bDeviceProtocol
    64,                   // bMaxPacketSize0
    0xFE,0xCA,            // idVendor (0xCAFE - for local dev only)
    0x01,0x40,            // idProduct (0x4001)
    0x00,0x01,            // bcdDevice 1.00
    1, 2, 3,              // iManufacturer, iProduct, iSerialNumber
    1                     // bNumConfigurations
};

// Configuration descriptor with CDC ACM (2 interfaces)
static const uint8_t cfg_desc[] = {
    // Config
    9, 0x02,              // bLength, bDescriptorType(Configuration)
    67, 0x00,             // wTotalLength (67 bytes)
    0x02,                 // bNumInterfaces
    0x01,                 // bConfigurationValue
    0x00,                 // iConfiguration
    0x80,                 // bmAttributes (bus powered)
    50,                   // bMaxPower (100 mA)

    // Interface 0 (Communication Class)
    9, 0x04,              // bLength, bDescriptorType(Interface)
    0x00,                 // bInterfaceNumber
    0x00,                 // bAlternateSetting
    0x01,                 // bNumEndpoints
    0x02,                 // bInterfaceClass (CDC)
    0x02,                 // bInterfaceSubClass (ACM)
    0x01,                 // bInterfaceProtocol (AT cmds)
    0x00,                 // iInterface

    // Header Functional Descriptor
    5, 0x24, 0x00,        // bLength, bDescriptorType(CS_INTERFACE), bDescriptorSubType(HEADER)
    0x10, 0x01,           // bcdCDC 1.10

    // Call Management Functional Descriptor
    5, 0x24, 0x01,        // CALL_MANAGEMENT
    0x00,                 // bmCapabilities
    0x01,                 // bDataInterface

    // ACM Functional Descriptor
    4, 0x24, 0x02,        // ACM
    0x02,                 // bmCapabilities (Set_Line_Coding supported)

    // Union Functional Descriptor
    5, 0x24, 0x06,        // UNION
    0x00,                 // bMasterInterface
    0x01,                 // bSlaveInterface0

    // Endpoint (Interrupt IN)
    7, 0x05,              // bLength, bDescriptorType(Endpoint)
    CDC_INT_EP,           // bEndpointAddress
    0x03,                 // bmAttributes (Interrupt)
    16, 0x00,             // wMaxPacketSize
    0x10,                 // bInterval

    // Interface 1 (Data Class)
    9, 0x04,
    0x01,                 // bInterfaceNumber
    0x00,                 // bAlternateSetting
    0x02,                 // bNumEndpoints
    0x0A,                 // bInterfaceClass (Data)
    0x00,                 // bInterfaceSubClass
    0x00,                 // bInterfaceProtocol
    0x00,                 // iInterface

    // Endpoint (Bulk OUT)
    7, 0x05,
    CDC_DATA_OUT_EP,      // OUT ep addr
    0x02,                 // Bulk
    CDC_MAX_PACKET, 0x00, // 64 bytes
    0x00,

    // Endpoint (Bulk IN)
    7, 0x05,
    CDC_DATA_IN_EP,       // IN ep addr
    0x02,                 // Bulk
    CDC_MAX_PACKET, 0x00, // 64 bytes
    0x00,
};

static const uint8_t lang_id_desc[] = { 0x04, 0x03, 0x09, 0x04 };
static uint8_t str_desc[64];

static uint8_t make_string_desc(const char* s)
{
    uint8_t len = (uint8_t)strlen(s);
    if (len > 31) len = 31; // 31 chars max (62 bytes) to fit buffer
    str_desc[0] = (uint8_t)(2*len + 2);
    str_desc[1] = 0x03;
    for (uint8_t i=0;i<len;i++) {
        str_desc[2+2*i] = (uint8_t)s[i];
        str_desc[3+2*i] = 0;
    }
    return str_desc[0];
}

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

static void ep0_send(const uint8_t* data, uint16_t len)
{
    HAL_PCD_EP_Transmit(&hpcd_USB_OTG_FS, 0x00, (uint8_t*)data, len);
}

static void ep0_zlp_in(void)
{
    HAL_PCD_EP_Transmit(&hpcd_USB_OTG_FS, 0x00, NULL, 0);
}

void VCP_Init(void) {
    // Initialize HAL (sets SysTick timebase, NVIC priority grouping)
    HAL_Init();

    hpcd_USB_OTG_FS.Instance = USB_OTG_FS;
    hpcd_USB_OTG_FS.Init.dev_endpoints = 6;
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
        // Configure RX/TX FIFOs (units are 32-bit words)
        HAL_PCD_SetRxFiFo(&hpcd_USB_OTG_FS, 128);
        HAL_PCD_SetTxFiFo(&hpcd_USB_OTG_FS, 0, 64); // EP0 IN
        HAL_PCD_SetTxFiFo(&hpcd_USB_OTG_FS, 1, 64); // EP1 IN
        HAL_PCD_SetTxFiFo(&hpcd_USB_OTG_FS, 2, 64); // EP2 IN
        HAL_PCD_Start(&hpcd_USB_OTG_FS);
    }
}

void VCP_Task(void) {
    static uint32_t last = 0;
    if (configured && dtr) {
        uint32_t now = HAL_GetTick();
        if (now - last > 1000) {
            static const char msg[] = "Hello from F407 VCP\r\n";
            HAL_PCD_EP_Transmit(&hpcd_USB_OTG_FS, CDC_DATA_IN_EP, (uint8_t*)msg, (uint16_t)sizeof(msg)-1);
            last = now;
        }
    }
}

// PCD Callbacks: control the device behavior
void HAL_PCD_ResetCallback(PCD_HandleTypeDef *hpcd)
{
    // Open EP0 for control
    HAL_PCD_EP_Open(hpcd, 0x00, 64, EP_TYPE_CTRL);
    HAL_PCD_EP_Open(hpcd, 0x80, 64, EP_TYPE_CTRL);
    configured = 0;
    dtr = 0;
}

void HAL_PCD_SetupStageCallback(PCD_HandleTypeDef *hpcd)
{
    uint8_t* setup = (uint8_t*)hpcd->Setup; // 8 bytes
    uint8_t bmRequest = setup[0];
    uint8_t bRequest  = setup[1];
    uint16_t wValue   = (uint16_t)(setup[2] | (setup[3] << 8));
    uint16_t wIndex   = (uint16_t)(setup[4] | (setup[5] << 8));
    uint16_t wLength  = (uint16_t)(setup[6] | (setup[7] << 8));

    // Standard requests
    if ((bmRequest & 0x60) == 0x00) {
        switch (bRequest) {
            case 6: { // GET_DESCRIPTOR
                uint8_t dtype = (uint8_t)(wValue >> 8);
                uint8_t dindex = (uint8_t)(wValue & 0xFF);
                const uint8_t* data = NULL; uint16_t len = 0;
                if (dtype == 1) { data = device_desc; len = sizeof(device_desc); }
                else if (dtype == 2) { data = cfg_desc; len = sizeof(cfg_desc); }
                else if (dtype == 3) {
                    if (dindex == 0) { data = lang_id_desc; len = sizeof(lang_id_desc); }
                    else if (dindex == 1) { len = make_string_desc("Acme"); data = str_desc; }
                    else if (dindex == 2) { len = make_string_desc("F407 VCP"); data = str_desc; }
                    else if (dindex == 3) { len = make_string_desc("0001"); data = str_desc; }
                }
                if (data) {
                    if (len > wLength) len = wLength;
                    ep0_send(data, len);
                } else {
                    HAL_PCD_EP_SetStall(hpcd, 0x00);
                }
            } break;
            case 5: { // SET_ADDRESS
                pending_address = (uint8_t)(wValue & 0x7F);
                set_address_pending = 1;
                ep0_zlp_in();
            } break;
            case 9: { // SET_CONFIGURATION
                if ((uint8_t)wValue == 1) {
                    // Open CDC endpoints
                    HAL_PCD_EP_Open(hpcd, CDC_INT_EP, 16, EP_TYPE_INTR);
                    HAL_PCD_EP_Open(hpcd, CDC_DATA_IN_EP, CDC_MAX_PACKET, EP_TYPE_BULK);
                    HAL_PCD_EP_Open(hpcd, CDC_DATA_OUT_EP, CDC_MAX_PACKET, EP_TYPE_BULK);
                    // Prepare OUT reception
                    HAL_PCD_EP_Receive(hpcd, CDC_DATA_OUT_EP, out_buf, sizeof(out_buf));
                    configured = 1;
                } else {
                    configured = 0;
                }
                ep0_zlp_in();
            } break;
            case 8: { // GET_CONFIGURATION
                uint8_t cfg = configured ? 1 : 0;
                ep0_send(&cfg, 1);
            } break;
            case 0: { // GET_STATUS
                static const uint8_t st[2] = {0,0};
                ep0_send(st, 2);
            } break;
            default:
                // Unhandled standard request
                HAL_PCD_EP_SetStall(hpcd, 0x00);
                break;
        }
        return;
    }

    // Class-specific requests (CDC ACM) to interface
    if ((bmRequest & 0x7F) == 0x21) { // Host-to-device, Class, Interface
        switch (bRequest) {
            case 0x20: // SET_LINE_CODING (7 bytes OUT)
                // Prepare to receive data stage on EP0 OUT
                HAL_PCD_EP_Receive(hpcd, 0x00, (uint8_t*)&g_line, sizeof(g_line));
                return;
            case 0x22: // SET_CONTROL_LINE_STATE
                dtr = (wValue & 0x01) ? 1 : 0;
                ep0_zlp_in();
                return;
            default:
                break;
        }
    } else if ((bmRequest & 0x7F) == 0xA1) { // Device-to-host, Class, Interface
        if (bRequest == 0x21) { // GET_LINE_CODING
            uint16_t len = (wLength < sizeof(g_line)) ? wLength : sizeof(g_line);
            ep0_send((uint8_t*)&g_line, len);
            return;
        }
    }

    // Fallback
    HAL_PCD_EP_SetStall(hpcd, 0x00);
}

void HAL_PCD_DataOutStageCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum)
{
    if (epnum == 0) {
        // Data stage for control write (e.g., SET_LINE_CODING)
        ep0_zlp_in();
        return;
    }
    if ((epnum & 0x7F) == (CDC_DATA_OUT_EP & 0x7F)) {
        uint16_t count = HAL_PCD_EP_GetRxCount(hpcd, epnum);
        // For demo, echo back to host
        if (count) {
            HAL_PCD_EP_Transmit(hpcd, CDC_DATA_IN_EP, out_buf, count);
        }
        // Re-arm OUT
        HAL_PCD_EP_Receive(hpcd, CDC_DATA_OUT_EP, out_buf, sizeof(out_buf));
    }
}

void HAL_PCD_DataInStageCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum)
{
    if (epnum == 0) {
        if (set_address_pending) {
            HAL_PCD_SetAddress(hpcd, pending_address);
            set_address_pending = 0;
        }
    }
}

void HAL_PCD_SOFCallback(PCD_HandleTypeDef *hpcd) { (void)hpcd; }
void HAL_PCD_SuspendCallback(PCD_HandleTypeDef *hpcd) { (void)hpcd; }
void HAL_PCD_ResumeCallback(PCD_HandleTypeDef *hpcd) { (void)hpcd; }
void HAL_PCD_ISOOUTIncompleteCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum) { (void)hpcd; (void)epnum; }
void HAL_PCD_ISOINIncompleteCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum) { (void)hpcd; (void)epnum; }
void HAL_PCD_ConnectCallback(PCD_HandleTypeDef *hpcd) { (void)hpcd; }
void HAL_PCD_DisconnectCallback(PCD_HandleTypeDef *hpcd) { (void)hpcd; }
