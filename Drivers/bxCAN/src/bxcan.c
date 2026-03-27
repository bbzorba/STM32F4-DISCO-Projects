/*
    Author: Baris Berk Zorba
    Date: June 2024
    bxCAN driver implementation for STM32F4xx series, providing
    both polling and interrupt-based APIs for CAN communication.
    This driver is designed to be simple and easy to understand,
    with clear separation between low-level register access and
    high-level "object-style" functions.
*/

#include "../inc/bxcan.h"
#include "../../../Projects/LED_Blink/inc/led.h"

#define NO_OF_CONT_TX_RX_TEST 500

// Static pointers used by IRQ handlers so the ISR can find the handle
static CAN_HandleType *s_can1_handle = NULL;
static CAN_HandleType *s_can2_handle = NULL;

// Used by interrupt-based test
static volatile uint8_t s_irq_msg_received = 0;
static CAN_MsgType      s_irq_rx_msg;

static void delay(volatile uint32_t count)
{
    while (count--) { __asm__("nop"); }
}

//////////////////////////////INTERNAL HELPERS//////////////////////////////

/*  Compute BTR value for a given baud rate.
    Assumes APB1 = 16 MHz (HSI default on STM32F4 Discovery).
    Uses TS1=13tq, TS2=2tq → nominal bit time = 1+13+2 = 16 tq.
    Sample point = (1+13)/16 = 87.5 %.
    Baud = APB1_CLK / (BRP × 16). */
static uint32_t CAN_ComputeBTR(CAN_BaudRateType baud)
{
    uint32_t brp;
    switch (baud) {
        case CAN_1MBPS:   brp = 1;  break;  // 16 MHz / (1 × 16) = 1   Mbps
        case CAN_500KBPS: brp = 2;  break;  // 16 MHz / (2 × 16) = 500 kbps
        case CAN_250KBPS: brp = 4;  break;  // 16 MHz / (4 × 16) = 250 kbps
        case CAN_125KBPS: brp = 8;  break;  // 16 MHz / (8 × 16) = 125 kbps
        default:          brp = 2;  break;
    }

    /* SJW = 1 tq (field 0), TS2 = 2 tq (field 1), TS1 = 13 tq (field 12) */
    return ((0U  << CAN_BTR_SJW_Pos) |
            (1U  << CAN_BTR_TS2_Pos) |
            (12U << CAN_BTR_TS1_Pos) |
            ((brp - 1U) << CAN_BTR_BRP_Pos));
}

/*  Read one message from a given RX FIFO (0 or 1).
    Returns 1 if a message was read, 0 if FIFO was empty.
    Releases the FIFO slot after reading. */
static int CAN_ReadFIFO(CAN_ManualTypeDef *regs, uint8_t fifo, CAN_MsgType *msg)
{
    __IO uint32_t *rfr  = (fifo == 0) ? &regs->RF0R   : &regs->RF1R;
    uint32_t fmp_msk    = (fifo == 0) ? CAN_RF0R_FMP0_Msk : CAN_RF1R_FMP1_Msk;
    uint32_t rfom       = (fifo == 0) ? CAN_RF0R_RFOM0    : CAN_RF1R_RFOM1;

    if ((*rfr & fmp_msk) == 0)
        return 0;  // No pending messages

    CAN_FIFOMailBox_ManualTypeDef *mb = &regs->sFIFOMailBox[fifo];

    // Snapshot all four registers before releasing the FIFO slot
    uint32_t rir  = mb->RIR;
    uint32_t rdtr = mb->RDTR;
    uint32_t rdlr = mb->RDLR;
    uint32_t rdhr = mb->RDHR;

    // Release the FIFO slot (hardware advances to next message)
    *rfr |= rfom;

    // Parse identifier
    msg->IDE = (rir & CAN_RIR_IDE) ? 1 : 0;
    msg->RTR = (rir & CAN_RIR_RTR) ? 1 : 0;
    if (msg->IDE) {
        msg->ExtId = (rir >> CAN_RIR_EXID_Pos) & 0x1FFFFFFFU;
        msg->StdId = 0;
    } else {
        msg->StdId = (rir >> CAN_RIR_STID_Pos) & 0x7FFU;
        msg->ExtId = 0;
    }

    // Parse data length
    msg->DLC = rdtr & CAN_RDTR_DLC_Msk;
    if (msg->DLC > 8) msg->DLC = 8;

    // Unpack data bytes (little-endian register layout)
    msg->Data[0] = (uint8_t)(rdlr >>  0);
    msg->Data[1] = (uint8_t)(rdlr >>  8);
    msg->Data[2] = (uint8_t)(rdlr >> 16);
    msg->Data[3] = (uint8_t)(rdlr >> 24);
    msg->Data[4] = (uint8_t)(rdhr >>  0);
    msg->Data[5] = (uint8_t)(rdhr >>  8);
    msg->Data[6] = (uint8_t)(rdhr >> 16);
    msg->Data[7] = (uint8_t)(rdhr >> 24);

    return 1;
}
////////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////CONSTRUCTOR & INIT////////////////////////////////////////

void CAN_constructor(CAN_HandleType *handle, CAN_ManualTypeDef *regs,
                     CAN_BaudRateType baudrate, CAN_ModeType mode)
{
    handle->regs       = regs;
    handle->baudRate   = baudrate;
    handle->mode       = mode;
    handle->rxCallback = NULL;

    CAN_Init(handle);
}

void CAN_Init(CAN_HandleType *handle)
{
    // Store handle in static pointer for IRQ dispatch
    if (handle->regs == CAN_1) s_can1_handle = handle;
    else if (handle->regs == CAN_2) s_can2_handle = handle;

    /* 1. Enable peripheral clock & configure GPIO pins */
    if (handle->regs == CAN_1) {
        RCC->APB1ENR |= RCC_APB1ENR_CAN1EN;
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;

        // PD0 = CAN1_RX, PD1 = CAN1_TX — Alternate Function 9
        GPIO_D->MODER  &= ~(MODER_PIN0_MASK | MODER_PIN1_MASK);
        GPIO_D->MODER  |=  (MODER_PIN0_SET  | MODER_PIN1_SET);   // AF mode (0b10)
        GPIO_D->AFR[0] &= ~(AFRL_PIN0_MASK  | AFRL_PIN1_MASK);
        GPIO_D->AFR[0] |=  (AFRL_PIN0_SET_AF9 | AFRL_PIN1_SET_AF9);
        // Pull-up on RX pin so floating line reads as recessive (high).
        // Without this, leaving init mode hangs waiting for 11 recessive bits.
        GPIO_D->PUPDR  &= ~PUPDR_PIN0_MASK;
        GPIO_D->PUPDR  |=  (GPIO_PULLUP << 0);   // PD0 pull-up
    }
    else if (handle->regs == CAN_2) {
        // CAN2 requires CAN1 clock to access shared filter registers
        RCC->APB1ENR |= RCC_APB1ENR_CAN1EN | RCC_APB1ENR_CAN2EN;
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

        // PB5 = CAN2_RX, PB6 = CAN2_TX — Alternate Function 9
        GPIO_B->MODER  &= ~(MODER_PIN5_MASK | MODER_PIN6_MASK);
        GPIO_B->MODER  |=  (MODER_PIN5_SET  | MODER_PIN6_SET);   // AF mode (0b10)
        GPIO_B->AFR[0] &= ~(AFRL_PIN5_MASK  | AFRL_PIN6_MASK);
        GPIO_B->AFR[0] |=  (AFRL_PIN5_SET_AF9 | AFRL_PIN6_SET_AF9);
        // Pull-up on RX pin so floating line reads as recessive (high)
        GPIO_B->PUPDR  &= ~PUPDR_PIN5_MASK;
        GPIO_B->PUPDR  |=  (GPIO_PULLUP << 10);  // PB5 pull-up
    }

    /* 2. Request initialisation mode (overrides sleep — no bus sync needed) */
    handle->regs->MCR |= CAN_MCR_INRQ;
    while (!(handle->regs->MSR & CAN_MSR_INAK)) { /* wait for INAK to set */ }

    /* 3. Now safe to clear SLEEP (we're in init mode, no bus sync required) */
    handle->regs->MCR &= ~CAN_MCR_SLEEP;

    /* 4. Configure bit timing (BTR can only be written in init mode) */
    uint32_t btr = CAN_ComputeBTR(handle->baudRate);
    if (handle->mode == CAN_MODE_LOOPBACK || handle->mode == CAN_MODE_SILENT_LOOPBACK)
        btr |= CAN_BTR_LBKM;
    if (handle->mode == CAN_MODE_SILENT || handle->mode == CAN_MODE_SILENT_LOOPBACK)
        btr |= CAN_BTR_SILM;
    handle->regs->BTR = btr;

    /* 5. MCR options: auto bus-off recovery, no automatic retransmission */
    handle->regs->MCR &= ~(CAN_MCR_SLEEP | CAN_MCR_TTCM | CAN_MCR_TXFP);
    handle->regs->MCR |= CAN_MCR_ABOM | CAN_MCR_NART;

    /* 6. Configure an accept-all filter routed to FIFO 0 */
    CAN_FilterAcceptAll(handle);

    /* 7. Leave initialisation mode → enter normal / loopback mode */
    handle->regs->MCR &= ~CAN_MCR_INRQ;
    while (handle->regs->MSR & CAN_MSR_INAK) { /* wait for INAK to clear */ }
}
////////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////FILTER CONFIGURATION//////////////////////////////////////

/*  Configure one filter bank to accept every CAN ID into FIFO 0.
    All 28 filter banks are managed through CAN1's registers, even for CAN2.
    CAN1 uses bank 0, CAN2 uses bank 14 (default CAN2SB). */
void CAN_FilterAcceptAll(CAN_HandleType *handle)
{
    CAN_ManualTypeDef *filt = CAN_1;  // Filters are always accessed via CAN1
    uint8_t bank = (handle->regs == CAN_2) ? 14 : 0;

    filt->FMR  |=  CAN_FMR_FINIT;                    // Enter filter init mode
    filt->FA1R &= ~(1U << bank);                      // Deactivate bank
    filt->FM1R &= ~(1U << bank);                      // ID/Mask mode
    filt->FS1R |=  (1U << bank);                       // 32-bit scale
    filt->sFilterRegister[bank].FR1 = 0x00000000;     // ID   = don't care
    filt->sFilterRegister[bank].FR2 = 0x00000000;     // Mask = accept all
    filt->FFA1R &= ~(1U << bank);                      // Route to FIFO 0
    filt->FA1R  |=  (1U << bank);                      // Activate bank
    filt->FMR   &= ~CAN_FMR_FINIT;                    // Leave filter init mode
}
////////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////POLLING TX API////////////////////////////////////////////

/*  Transmit one CAN message.  Returns 0 on success, -1 if no mailbox
    was available or the transmission failed. */
int CAN_Transmit(CAN_HandleType *handle, const CAN_MsgType *msg)
{
    // Find an empty TX mailbox
    uint8_t mailbox;
    if      (handle->regs->TSR & CAN_TSR_TME0) mailbox = 0;
    else if (handle->regs->TSR & CAN_TSR_TME1) mailbox = 1;
    else if (handle->regs->TSR & CAN_TSR_TME2) mailbox = 2;
    else return -1;  // All mailboxes busy

    CAN_TxMailBox_ManualTypeDef *mb = &handle->regs->sTxMailBox[mailbox];

    // Build identifier register value (TXRQ not set yet)
    uint32_t tir = 0;
    if (msg->IDE) {
        tir = (msg->ExtId << CAN_TIR_EXID_Pos) | CAN_TIR_IDE;
    } else {
        tir = (msg->StdId << CAN_TIR_STID_Pos);
    }
    if (msg->RTR) tir |= CAN_TIR_RTR;

    // Fill data length & payload
    mb->TDTR = msg->DLC & CAN_TDTR_DLC_Msk;
    mb->TDLR = ((uint32_t)msg->Data[0]      ) |
               ((uint32_t)msg->Data[1] <<  8) |
               ((uint32_t)msg->Data[2] << 16) |
               ((uint32_t)msg->Data[3] << 24);
    mb->TDHR = ((uint32_t)msg->Data[4]      ) |
               ((uint32_t)msg->Data[5] <<  8) |
               ((uint32_t)msg->Data[6] << 16) |
               ((uint32_t)msg->Data[7] << 24);

    // Request transmission
    mb->TIR = tir | CAN_TIR_TXRQ;

    // Wait for Request Completed (RQCP) flag
    uint32_t rqcp, txok;
    switch (mailbox) {
        case 0:  rqcp = CAN_TSR_RQCP0; txok = CAN_TSR_TXOK0; break;
        case 1:  rqcp = CAN_TSR_RQCP1; txok = CAN_TSR_TXOK1; break;
        default: rqcp = CAN_TSR_RQCP2; txok = CAN_TSR_TXOK2; break;
    }
    while (!(handle->regs->TSR & rqcp)) { /* wait */ }

    int result = (handle->regs->TSR & txok) ? 0 : -1;
    handle->regs->TSR = rqcp;  // Clear RQCP by writing 1
    return result;
}
////////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////POLLING RX API////////////////////////////////////////////

/*  Receive one CAN message from FIFO 0 (polling).
    Returns 0 if a message was read, -1 if FIFO was empty. */
int CAN_Receive(CAN_HandleType *handle, CAN_MsgType *msg)
{
    return CAN_ReadFIFO(handle->regs, 0, msg) ? 0 : -1;
}
////////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////INTERRUPT API//////////////////////////////////////////////

void CAN_EnableRXInterrupt(CAN_HandleType *handle, CAN_RxCallback_t callback)
{
    handle->rxCallback = callback;

    // Enable FIFO 0 Message Pending interrupt
    handle->regs->IER |= CAN_IER_FMPIE0;

    if (handle->regs == CAN_1) {
        NVIC_EnableIRQ(CAN1_RX0_IRQn);
    } else if (handle->regs == CAN_2) {
        NVIC_EnableIRQ(CAN2_RX0_IRQn);
    }
}

void CAN_DisableRXInterrupt(CAN_HandleType *handle)
{
    handle->regs->IER &= ~CAN_IER_FMPIE0;

    if (handle->regs == CAN_1) {
        NVIC_DisableIRQ(CAN1_RX0_IRQn);
    } else if (handle->regs == CAN_2) {
        NVIC_DisableIRQ(CAN2_RX0_IRQn);
    }
}

// IRQ handler for CAN1 RX FIFO 0
void CAN1_RX0_IRQHandler(void)
{
    if (!s_can1_handle) return;
    CAN_MsgType msg;
    if (CAN_ReadFIFO(CAN_1, 0, &msg) && s_can1_handle->rxCallback) {
        s_can1_handle->rxCallback(&msg);
    }
}

// IRQ handler for CAN2 RX FIFO 0
void CAN2_RX0_IRQHandler(void)
{
    if (!s_can2_handle) return;
    CAN_MsgType msg;
    if (CAN_ReadFIFO(CAN_2, 0, &msg) && s_can2_handle->rxCallback) {
        s_can2_handle->rxCallback(&msg);
    }
}
////////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////BUILT-IN TESTS////////////////////////////////////////////

/* Internal RX callback used by CAN_LoopbackTest (stores message & sets flag) */
static void loopback_irq_callback(const CAN_MsgType *msg)
{
    s_irq_rx_msg = *msg;
    s_irq_msg_received = 1;
}

/* Helper: print a CAN message in hex over UART */
static void CAN_PrintMsg(USART_HandleType *uart, const char *label, const CAN_MsgType *msg)
{
    char buf[128];
    snprintf(buf, sizeof(buf), "  %s: ID=0x%03lX DLC=%u Data=",
             label, (unsigned long)(msg->IDE ? msg->ExtId : msg->StdId), msg->DLC);
    USART_WriteString(uart, buf);
    for (uint8_t i = 0; i < msg->DLC; i++) {
        snprintf(buf, sizeof(buf), "%02X ", msg->Data[i]);
        USART_WriteString(uart, buf);
    }
    USART_WriteString(uart, "\n");
}

/*  Self-test using internal loopback.
    Phase 1: Polling TX → Polling RX      (green LED on success)
    Phase 2: Polling TX → Interrupt RX    (blue LED on success)
    Phase 3: Continuous loopback blink    (green LED toggles)
    Red LED = any failure.
    All results are printed over UART.
    Returns 0 on overall PASS, -1 on FAIL. */
int CAN_LoopbackTest(CAN_HandleType *handle, USART_HandleType *uart)
{
    LED_Type led_green, led_orange, led_red, led_blue;
    LED_constructor(&led_green,  GREEN,  LED_OFF);
    LED_constructor(&led_orange, YELLOW, LED_OFF);
    LED_constructor(&led_red,    RED,    LED_OFF);
    LED_constructor(&led_blue,   BLUE,   LED_OFF);

    char buf[128];
    int pass = 0;

    USART_WriteString(uart, "\n========================================\n");
    USART_WriteString(uart, " bxCAN Loopback Self-Test\n");
    USART_WriteString(uart, "========================================\n");

    /* Print configuration */
    snprintf(buf, sizeof(buf), "Mode: LOOPBACK | Baud: %s\n",
             (handle->baudRate == CAN_1MBPS)   ? "1 Mbps"   :
             (handle->baudRate == CAN_500KBPS)  ? "500 kbps" :
             (handle->baudRate == CAN_250KBPS)  ? "250 kbps" :
             (handle->baudRate == CAN_125KBPS)  ? "125 kbps" : "unknown");
    USART_WriteString(uart, buf);
    snprintf(buf, sizeof(buf), "BTR=0x%08lX  MCR=0x%08lX  MSR=0x%08lX\n",
             (unsigned long)handle->regs->BTR,
             (unsigned long)handle->regs->MCR,
             (unsigned long)handle->regs->MSR);
    USART_WriteString(uart, buf);

    /* ---- Phase 1: Polling TX → Polling RX ---- */
    USART_WriteString(uart, "\n--- Phase 1: Polling TX -> Polling RX ---\n");

    CAN_MsgType tx_msg = {0};
    tx_msg.StdId   = 0x123;
    tx_msg.IDE     = 0;
    tx_msg.RTR     = 0;
    tx_msg.DLC     = 4;
    tx_msg.Data[0] = 0xDE;
    tx_msg.Data[1] = 0xAD;
    tx_msg.Data[2] = 0xBE;
    tx_msg.Data[3] = 0xEF;

    CAN_PrintMsg(uart, "TX", &tx_msg);

    if (CAN_Transmit(handle, &tx_msg) != 0) {
        USART_WriteString(uart, "  TX FAILED! TSR=0x");
        snprintf(buf, sizeof(buf), "%08lX\n", (unsigned long)handle->regs->TSR);
        USART_WriteString(uart, buf);
        snprintf(buf, sizeof(buf), "  ESR=0x%08lX\n", (unsigned long)handle->regs->ESR);
        USART_WriteString(uart, buf);
        LED_setState(&led_red, LED_ON);
        USART_WriteString(uart, "  RESULT: FAIL\n");
        return -1;
    }
    USART_WriteString(uart, "  TX OK\n");
    LED_setState(&led_green, LED_ON);

    CAN_MsgType rx_msg;
    while (CAN_Receive(handle, &rx_msg) != 0) { /* spin */ }

    CAN_PrintMsg(uart, "RX", &rx_msg);

    if (rx_msg.StdId == 0x123 &&
        rx_msg.Data[0] == 0xDE && rx_msg.Data[1] == 0xAD &&
        rx_msg.Data[2] == 0xBE && rx_msg.Data[3] == 0xEF)
    {
        USART_WriteString(uart, "  RX MATCH: PASS\n");
        LED_setState(&led_orange, LED_ON);
    } else {
        USART_WriteString(uart, "  RX MISMATCH: FAIL\n");
        LED_setState(&led_red, LED_ON);
        pass = -1;
    }

    delay(3000000);
    LED_setState(&led_green, LED_OFF);
    LED_setState(&led_orange, LED_OFF);
    LED_setState(&led_red, LED_OFF);

    /* ---- Phase 2: Interrupt-based RX ---- */
    USART_WriteString(uart, "\n--- Phase 2: Polling TX -> Interrupt RX ---\n");

    s_irq_msg_received = 0;
    CAN_EnableRXInterrupt(handle, loopback_irq_callback);
    USART_WriteString(uart, "  RX interrupt enabled (FIFO 0)\n");

    tx_msg.StdId   = 0x456;
    tx_msg.DLC     = 2;
    tx_msg.Data[0] = 0xCA;
    tx_msg.Data[1] = 0xFE;

    CAN_PrintMsg(uart, "TX", &tx_msg);

    if (CAN_Transmit(handle, &tx_msg) != 0) {
        USART_WriteString(uart, "  TX FAILED!\n");
        LED_setState(&led_red, LED_ON);
        pass = -1;
    } else {
        USART_WriteString(uart, "  TX OK, waiting for IRQ callback...\n");
    }

    while (!s_irq_msg_received) { /* spin */ }
    USART_WriteString(uart, "  IRQ callback fired!\n");

    CAN_PrintMsg(uart, "IRQ RX", &s_irq_rx_msg);

    if (s_irq_rx_msg.StdId == 0x456 &&
        s_irq_rx_msg.Data[0] == 0xCA && s_irq_rx_msg.Data[1] == 0xFE)
    {
        USART_WriteString(uart, "  IRQ RX MATCH: PASS\n");
        LED_setState(&led_blue, LED_ON);
    } else {
        USART_WriteString(uart, "  IRQ RX MISMATCH: FAIL\n");
        LED_setState(&led_red, LED_ON);
        pass = -1;
    }

    CAN_DisableRXInterrupt(handle);

    delay(3000000);
    LED_setState(&led_blue, LED_OFF);
    LED_setState(&led_red, LED_OFF);

    /* ---- Overall result ---- */
    USART_WriteString(uart, "\n========================================\n");
    if (pass == 0)
        USART_WriteString(uart, " LOOPBACK TEST: ALL PASSED\n");
    else
        USART_WriteString(uart, " LOOPBACK TEST: FAILED\n");
    USART_WriteString(uart, "========================================\n");

    /* ---- Phase 3: Run a finite number of loopback iterations ---- */
    USART_WriteString(uart, "\nPhase 3: Loopback run 50 iterations (press blue button to exit early)\n");

    uint8_t counter = 0;
    const uint8_t LOOPBACK_ITERATIONS = 50;
    for (uint8_t i = 0; i < LOOPBACK_ITERATIONS; i++) {
        tx_msg.StdId   = 0x100;
        tx_msg.DLC     = 1;
        tx_msg.Data[0] = counter++;

        if (CAN_Transmit(handle, &tx_msg) == 0) {
            while (CAN_Receive(handle, &rx_msg) != 0) { /* spin */ }

            snprintf(buf, sizeof(buf), "  Loop #%u: TX=0x%02X RX=0x%02X %s\n",
                     (unsigned)(i + 1), tx_msg.Data[0], rx_msg.Data[0],
                     (tx_msg.Data[0] == rx_msg.Data[0]) ? "OK" : "MISMATCH");
            USART_WriteString(uart, buf);

            LED_setState(&led_green, LED_TOGGLE);
        } else {
            USART_WriteString(uart, "  Loop TX failed\n");
            pass = -1;
        }
        delay(1000000);
        if (GPIO_A->IDR & (1U << 0)) break;  // Blue user button → exit early
    }

    USART_WriteString(uart, "Phase 3 completed cont. test \n");
    LED_setState(&led_green, LED_OFF);
    return pass;
}

/*  Transceiver test for normal bus operation.
    Sends a test message and waits for an incoming message.
    Requires a CAN transceiver (e.g. MCP2551/SN65HVD230) and
    another CAN node on the bus.
    All results are printed over UART.
    Returns 0 on overall PASS, -1 on FAIL. */
int CAN_TransceiverTest(CAN_HandleType *handle, USART_HandleType *uart)
{
    LED_Type led_green, led_orange, led_red, led_blue;
    LED_constructor(&led_green,  GREEN,  LED_OFF);
    LED_constructor(&led_orange, YELLOW, LED_OFF);
    LED_constructor(&led_red,    RED,    LED_OFF);
    LED_constructor(&led_blue,   BLUE,   LED_OFF);

    char buf[128];
    int pass = 0;

    USART_WriteString(uart, "\n========================================\n");
    USART_WriteString(uart, " bxCAN Transceiver Test (Normal Mode)\n");
    USART_WriteString(uart, "========================================\n");

    snprintf(buf, sizeof(buf), "Mode: NORMAL | Baud: %s\n",
             (handle->baudRate == CAN_1MBPS)   ? "1 Mbps"   :
             (handle->baudRate == CAN_500KBPS)  ? "500 kbps" :
             (handle->baudRate == CAN_250KBPS)  ? "250 kbps" :
             (handle->baudRate == CAN_125KBPS)  ? "125 kbps" : "unknown");
    USART_WriteString(uart, buf);
    snprintf(buf, sizeof(buf), "BTR=0x%08lX  MCR=0x%08lX  MSR=0x%08lX\n",
             (unsigned long)handle->regs->BTR,
             (unsigned long)handle->regs->MCR,
             (unsigned long)handle->regs->MSR);
    USART_WriteString(uart, buf);

    /* ---- Phase 1: Polling TX ---- */
    USART_WriteString(uart, "\n--- Phase 1: Polling TX ---\n");

    CAN_MsgType tx_msg = {0};
    tx_msg.StdId   = 0x200;
    tx_msg.IDE     = 0;
    tx_msg.RTR     = 0;
    tx_msg.DLC     = 3;
    tx_msg.Data[0] = 'C';
    tx_msg.Data[1] = 'A';
    tx_msg.Data[2] = 'N';

    CAN_PrintMsg(uart, "TX", &tx_msg);

    if (CAN_Transmit(handle, &tx_msg) == 0) {
        USART_WriteString(uart, "  TX OK\n");
        LED_setState(&led_green, LED_ON);
    } else {
        USART_WriteString(uart, "  TX FAILED!\n");
        snprintf(buf, sizeof(buf), "  ESR=0x%08lX  TSR=0x%08lX\n",
                 (unsigned long)handle->regs->ESR,
                 (unsigned long)handle->regs->TSR);
        USART_WriteString(uart, buf);
        LED_setState(&led_red, LED_ON);
        pass = -1;
    }

    /* ---- Phase 2: Polling RX (wait for any message from another node) ---- */
    USART_WriteString(uart, "\n--- Phase 2: Polling RX (waiting for remote node) ---\n");

    CAN_MsgType rx_msg;
    while (CAN_Receive(handle, &rx_msg) != 0) { /* spin until a message arrives */ }

    USART_WriteString(uart, "  Message received!\n");
    CAN_PrintMsg(uart, "RX", &rx_msg);
    LED_setState(&led_orange, LED_ON);

    delay(3000000);
    LED_setState(&led_green, LED_OFF);
    LED_setState(&led_orange, LED_OFF);
    LED_setState(&led_red, LED_OFF);

    /* ---- Phase 3: Interrupt-based RX ---- */
    USART_WriteString(uart, "\n--- Phase 3: Interrupt RX (waiting for remote echo) ---\n");

    s_irq_msg_received = 0;
    CAN_EnableRXInterrupt(handle, loopback_irq_callback);
    USART_WriteString(uart, "  RX interrupt enabled (FIFO 0)\n");

    // Send another message so the remote node may echo back
    tx_msg.StdId   = 0x201;
    tx_msg.DLC     = 1;
    tx_msg.Data[0] = 0x42;

    CAN_PrintMsg(uart, "TX", &tx_msg);
    CAN_Transmit(handle, &tx_msg);
    USART_WriteString(uart, "  Waiting for IRQ callback...\n");

    while (!s_irq_msg_received) { /* spin until IRQ callback fires */ }
    USART_WriteString(uart, "  IRQ callback fired!\n");
    CAN_PrintMsg(uart, "IRQ RX", &s_irq_rx_msg);
    LED_setState(&led_blue, LED_ON);

    CAN_DisableRXInterrupt(handle);

    delay(3000000);
    LED_setState(&led_blue, LED_OFF);

    /* ---- Overall result ---- */
    USART_WriteString(uart, "\n========================================\n");
    if (pass == 0)
        USART_WriteString(uart, " TRANSCEIVER TEST: ALL PASSED\n");
    else
        USART_WriteString(uart, " TRANSCEIVER TEST: FAILED\n");
    USART_WriteString(uart, "========================================\n");

    /* ---- Phase 4: Continuous TX+RX (30 rounds) ---- */
    USART_WriteString(uart, "\nPhase 4: TX+RX cont. test (green LED toggles)\n");

    uint8_t counter = 0;
    for (uint8_t i = 0; i < NO_OF_CONT_TX_RX_TEST; i++) {
        tx_msg.StdId   = 0x100;
        tx_msg.DLC     = 1;
        tx_msg.Data[0] = counter++;

        CAN_Transmit(handle, &tx_msg);

        if (CAN_Receive(handle, &rx_msg) == 0) {
            snprintf(buf, sizeof(buf), "  Loop #%u: TX=0x%02X RX=0x%02X\n",
                     (unsigned)(counter - 1), tx_msg.Data[0], rx_msg.Data[0]);
            USART_WriteString(uart, buf);
            LED_setState(&led_green, LED_TOGGLE);
        }
        delay(1000000);
    }

    USART_WriteString(uart, "Phase 4 cont. test complete\n");
    LED_setState(&led_green, LED_OFF);
    return pass;
}
////////////////////////////////////////////////////////////////////////////////////////////
