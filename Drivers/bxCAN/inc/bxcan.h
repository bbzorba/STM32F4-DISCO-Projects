/*
    Author: Baris Berk Zorba
    Date: June 2024
    bxCAN (basic extended CAN) header for STM32F4xx series.
    Provides register definitions, message types, and APIs for
    both polling and interrupt-based CAN communication.
*/

#ifndef __BXCAN_H
#define __BXCAN_H

#include <stdint.h>
#include <stddef.h>
#include "../../GPIO/inc/gpio.h"

#ifndef __IO
#define __IO volatile
#endif
#ifndef __NVIC_PRIO_BITS
#define __NVIC_PRIO_BITS 4
#endif

/* Minimal IRQn_Type covering CAN IRQs (used when stm32f4xx.h is not included).
   Also includes USART/EXTI entries so uart.h (pulled in by led.h) doesn't
   redeclare the enum. */
#ifndef __STM32F4xx_H
typedef enum IRQn
{
    NonMaskableInt_IRQn         = -14,
    MemoryManagement_IRQn       = -12,
    BusFault_IRQn               = -11,
    UsageFault_IRQn             = -10,
    SVCall_IRQn                 = -5,
    DebugMonitor_IRQn           = -4,
    PendSV_IRQn                 = -2,
    SysTick_IRQn                = -1,

    /* EXTI */
    EXTI0_IRQn                  = 6,
    EXTI1_IRQn                  = 7,
    EXTI2_IRQn                  = 8,
    EXTI3_IRQn                  = 9,
    EXTI4_IRQn                  = 10,
    EXTI9_5_IRQn                = 23,
    EXTI15_10_IRQn              = 40,

    /* STM32F407 CAN peripheral interrupts */
    CAN1_TX_IRQn                = 19,
    CAN1_RX0_IRQn               = 20,
    CAN1_RX1_IRQn               = 21,
    CAN1_SCE_IRQn               = 22,
    CAN2_TX_IRQn                = 63,
    CAN2_RX0_IRQn               = 64,
    CAN2_RX1_IRQn               = 65,
    CAN2_SCE_IRQn               = 66,

    /* USART/UART */
    USART1_IRQn                 = 37,
    USART2_IRQn                 = 38,
    USART3_IRQn                 = 39,
    UART4_IRQn                  = 52,
    UART5_IRQn                  = 53,
    USART6_IRQn                 = 71
} IRQn_Type;
#define __STM32F4xx_H   /* prevent uart.h from re-declaring IRQn_Type */
#endif

#include "../../UART/inc/uart.h"
#include "core_cm4.h"

////////////////////////////// PERIPHERAL BASE ADDRESSES //////////////////////////////

#define CAN1_BASE (APB1PERIPH_BASE + 0x00006400U)
#define CAN2_BASE (APB1PERIPH_BASE + 0x00006800U)

////////////////////////////// RCC CLOCK ENABLE BITS /////////////////////////////////

// APB1ENR: CAN1 = bit 25, CAN2 = bit 26
#define RCC_APB1ENR_CAN1EN  ((uint32_t)0x02000000)
#define RCC_APB1ENR_CAN2EN  ((uint32_t)0x04000000)

////////////////////////////// GPIO AF9 (CAN) PIN DEFINITIONS /////////////////////////

// AF9 set values for AFRL register (CAN alternate function)
#define AFRL_PIN0_SET_AF9  ((uint32_t)0x00000009)  // PD0 = CAN1_RX
#define AFRL_PIN1_SET_AF9  ((uint32_t)0x00000090)  // PD1 = CAN1_TX
#define AFRL_PIN5_SET_AF9  ((uint32_t)0x00900000)  // PB5 = CAN2_RX
#define AFRL_PIN6_SET_AF9  ((uint32_t)0x09000000)  // PB6 = CAN2_TX

////////////////////////////// APB1 CLOCK (HSI DEFAULT) ///////////////////////////////

#ifndef APB1_CLK_HZ
#define APB1_CLK_HZ 16000000U
#endif

////////////////////////////// CAN MCR REGISTER BITS //////////////////////////////////

#define CAN_MCR_INRQ   ((uint32_t)0x00000001)
#define CAN_MCR_SLEEP  ((uint32_t)0x00000002)
#define CAN_MCR_TXFP   ((uint32_t)0x00000004)
#define CAN_MCR_RFLM   ((uint32_t)0x00000008)
#define CAN_MCR_NART   ((uint32_t)0x00000010)
#define CAN_MCR_AWUM   ((uint32_t)0x00000020)
#define CAN_MCR_ABOM   ((uint32_t)0x00000040)
#define CAN_MCR_TTCM   ((uint32_t)0x00000080)
#define CAN_MCR_RESET  ((uint32_t)0x00008000)
#define CAN_MCR_DBF    ((uint32_t)0x00010000)

////////////////////////////// CAN MSR REGISTER BITS //////////////////////////////////

#define CAN_MSR_INAK   ((uint32_t)0x00000001)
#define CAN_MSR_SLAK   ((uint32_t)0x00000002)
#define CAN_MSR_ERRI   ((uint32_t)0x00000004)
#define CAN_MSR_WKUI   ((uint32_t)0x00000008)
#define CAN_MSR_SLAKI  ((uint32_t)0x00000010)

////////////////////////////// CAN TSR REGISTER BITS //////////////////////////////////

#define CAN_TSR_RQCP0  ((uint32_t)0x00000001)
#define CAN_TSR_TXOK0  ((uint32_t)0x00000002)
#define CAN_TSR_ALST0  ((uint32_t)0x00000004)
#define CAN_TSR_TERR0  ((uint32_t)0x00000008)
#define CAN_TSR_ABRQ0  ((uint32_t)0x00000080)
#define CAN_TSR_RQCP1  ((uint32_t)0x00000100)
#define CAN_TSR_TXOK1  ((uint32_t)0x00000200)
#define CAN_TSR_ALST1  ((uint32_t)0x00000400)
#define CAN_TSR_TERR1  ((uint32_t)0x00000800)
#define CAN_TSR_ABRQ1  ((uint32_t)0x00008000)
#define CAN_TSR_RQCP2  ((uint32_t)0x00010000)
#define CAN_TSR_TXOK2  ((uint32_t)0x00020000)
#define CAN_TSR_ALST2  ((uint32_t)0x00040000)
#define CAN_TSR_TERR2  ((uint32_t)0x00080000)
#define CAN_TSR_ABRQ2  ((uint32_t)0x00800000)
#define CAN_TSR_TME0   ((uint32_t)0x04000000)
#define CAN_TSR_TME1   ((uint32_t)0x08000000)
#define CAN_TSR_TME2   ((uint32_t)0x10000000)

////////////////////////////// CAN RF0R / RF1R REGISTER BITS //////////////////////////

#define CAN_RF0R_FMP0_Msk  ((uint32_t)0x00000003)  // FIFO 0 message pending (2 bits)
#define CAN_RF0R_FULL0     ((uint32_t)0x00000008)
#define CAN_RF0R_FOVR0     ((uint32_t)0x00000010)
#define CAN_RF0R_RFOM0     ((uint32_t)0x00000020)  // Release FIFO 0 output mailbox

#define CAN_RF1R_FMP1_Msk  ((uint32_t)0x00000003)
#define CAN_RF1R_FULL1     ((uint32_t)0x00000008)
#define CAN_RF1R_FOVR1     ((uint32_t)0x00000010)
#define CAN_RF1R_RFOM1     ((uint32_t)0x00000020)

////////////////////////////// CAN IER REGISTER BITS //////////////////////////////////

#define CAN_IER_TMEIE  ((uint32_t)0x00000001)
#define CAN_IER_FMPIE0 ((uint32_t)0x00000002)  // FIFO 0 message pending interrupt
#define CAN_IER_FFIE0  ((uint32_t)0x00000004)
#define CAN_IER_FOVIE0 ((uint32_t)0x00000008)
#define CAN_IER_FMPIE1 ((uint32_t)0x00000010)
#define CAN_IER_FFIE1  ((uint32_t)0x00000020)
#define CAN_IER_FOVIE1 ((uint32_t)0x00000040)
#define CAN_IER_EWGIE  ((uint32_t)0x00000100)
#define CAN_IER_EPVIE  ((uint32_t)0x00000200)
#define CAN_IER_BOFIE  ((uint32_t)0x00000400)
#define CAN_IER_LECIE  ((uint32_t)0x00000800)
#define CAN_IER_ERRIE  ((uint32_t)0x00008000)
#define CAN_IER_WKUIE  ((uint32_t)0x00010000)
#define CAN_IER_SLKIE  ((uint32_t)0x00020000)

////////////////////////////// CAN ESR REGISTER BITS //////////////////////////////////

#define CAN_ESR_EWGF    ((uint32_t)0x00000001)
#define CAN_ESR_EPVF    ((uint32_t)0x00000002)
#define CAN_ESR_BOFF    ((uint32_t)0x00000004)
#define CAN_ESR_LEC_Msk ((uint32_t)0x00000070)   // bits [6:4]
#define CAN_ESR_TEC_Pos 8
#define CAN_ESR_TEC_Msk ((uint32_t)0x0000FF00)   // bits [15:8]
#define CAN_ESR_REC_Pos 16
#define CAN_ESR_REC_Msk ((uint32_t)0x00FF0000)   // bits [23:16]

////////////////////////////// CAN BTR REGISTER BITS //////////////////////////////////

#define CAN_BTR_BRP_Pos  0
#define CAN_BTR_BRP_Msk  ((uint32_t)0x000003FF)   // bits [9:0]  prescaler
#define CAN_BTR_TS1_Pos  16
#define CAN_BTR_TS1_Msk  ((uint32_t)0x000F0000)   // bits [19:16] time segment 1
#define CAN_BTR_TS2_Pos  20
#define CAN_BTR_TS2_Msk  ((uint32_t)0x00700000)   // bits [22:20] time segment 2
#define CAN_BTR_SJW_Pos  24
#define CAN_BTR_SJW_Msk  ((uint32_t)0x03000000)   // bits [25:24] resync jump width
#define CAN_BTR_LBKM     ((uint32_t)0x40000000)   // Loop Back Mode
#define CAN_BTR_SILM     ((uint32_t)0x80000000)   // Silent Mode

////////////////////////////// CAN TIR / RIR (MAILBOX ID) BITS ////////////////////////

#define CAN_TIR_TXRQ      ((uint32_t)0x00000001)  // Transmit mailbox request
#define CAN_TIR_RTR       ((uint32_t)0x00000002)   // Remote transmission request
#define CAN_TIR_IDE       ((uint32_t)0x00000004)   // Identifier extension
#define CAN_TIR_EXID_Pos  3                         // Extended ID starts at bit 3
#define CAN_TIR_STID_Pos  21                        // Standard ID starts at bit 21

#define CAN_RIR_RTR       ((uint32_t)0x00000002)
#define CAN_RIR_IDE       ((uint32_t)0x00000004)
#define CAN_RIR_EXID_Pos  3
#define CAN_RIR_STID_Pos  21

////////////////////////////// CAN TDTR / RDTR (DLC) BITS /////////////////////////////

#define CAN_TDTR_DLC_Msk  ((uint32_t)0x0000000F)   // bits [3:0]
#define CAN_RDTR_DLC_Msk  ((uint32_t)0x0000000F)

////////////////////////////// CAN FMR (FILTER MASTER) BITS ///////////////////////////

#define CAN_FMR_FINIT     ((uint32_t)0x00000001)   // Filter init mode

////////////////////////////// CAN REGISTER STRUCTURES ////////////////////////////////

/* TX mailbox (3 per CAN peripheral, at offset 0x180) */
typedef struct {
    __IO uint32_t TIR;    // TX mailbox identifier
    __IO uint32_t TDTR;   // TX mailbox data length control & time stamp
    __IO uint32_t TDLR;   // TX mailbox data low  (bytes 0-3)
    __IO uint32_t TDHR;   // TX mailbox data high (bytes 4-7)
} CAN_TxMailBox_ManualTypeDef;

/* RX FIFO mailbox (2 per CAN peripheral, at offset 0x1B0) */
typedef struct {
    __IO uint32_t RIR;    // RX FIFO mailbox identifier
    __IO uint32_t RDTR;   // RX FIFO mailbox data length control & time stamp
    __IO uint32_t RDLR;   // RX FIFO mailbox data low  (bytes 0-3)
    __IO uint32_t RDHR;   // RX FIFO mailbox data high (bytes 4-7)
} CAN_FIFOMailBox_ManualTypeDef;

/* Filter bank register pair (28 banks, at offset 0x240) */
typedef struct {
    __IO uint32_t FR1;    // Filter bank register 1
    __IO uint32_t FR2;    // Filter bank register 2
} CAN_FilterRegister_ManualTypeDef;

/* Complete CAN peripheral register map */
typedef struct {
    __IO uint32_t MCR;                                  // 0x000  Master Control
    __IO uint32_t MSR;                                  // 0x004  Master Status
    __IO uint32_t TSR;                                  // 0x008  Transmit Status
    __IO uint32_t RF0R;                                 // 0x00C  Receive FIFO 0
    __IO uint32_t RF1R;                                 // 0x010  Receive FIFO 1
    __IO uint32_t IER;                                  // 0x014  Interrupt Enable
    __IO uint32_t ESR;                                  // 0x018  Error Status
    __IO uint32_t BTR;                                  // 0x01C  Bit Timing
         uint32_t RESERVED0[88];                        // 0x020 – 0x17F
    CAN_TxMailBox_ManualTypeDef    sTxMailBox[3];        // 0x180 – 0x1AC  (3 TX mailboxes)
    CAN_FIFOMailBox_ManualTypeDef  sFIFOMailBox[2];      // 0x1B0 – 0x1CC  (2 RX FIFOs)
         uint32_t RESERVED1[12];                        // 0x1D0 – 0x1FF
    __IO uint32_t FMR;                                  // 0x200  Filter Master
    __IO uint32_t FM1R;                                 // 0x204  Filter Mode
         uint32_t RESERVED2;                            // 0x208
    __IO uint32_t FS1R;                                 // 0x20C  Filter Scale
         uint32_t RESERVED3;                            // 0x210
    __IO uint32_t FFA1R;                                // 0x214  Filter FIFO Assignment
         uint32_t RESERVED4;                            // 0x218
    __IO uint32_t FA1R;                                 // 0x21C  Filter Activation
         uint32_t RESERVED5[8];                         // 0x220 – 0x23F
    CAN_FilterRegister_ManualTypeDef sFilterRegister[28]; // 0x240 – 0x31F
} CAN_ManualTypeDef;

////////////////////////////// CAN PERIPHERAL INSTANCES ////////////////////////////////

#define CAN_1 ((CAN_ManualTypeDef *)CAN1_BASE)
#define CAN_2 ((CAN_ManualTypeDef *)CAN2_BASE)

////////////////////////////// CONFIGURATION ENUMS /////////////////////////////////////

/* Standard CAN baud rates (assumes APB1 = 16 MHz) */
typedef enum {
    CAN_500KBPS = 0,
    CAN_250KBPS,
    CAN_125KBPS,
    CAN_1MBPS
} CAN_BaudRateType;

/* CAN operating modes */
typedef enum {
    CAN_MODE_NORMAL          = 0,   // Normal bus operation (needs transceiver)
    CAN_MODE_LOOPBACK        = 1,   // Internal loopback (TX→RX, no pins needed)
    CAN_MODE_SILENT          = 2,   // Listen-only (no ACK, no TX)
    CAN_MODE_SILENT_LOOPBACK = 3    // Silent + loopback (self-test without bus)
} CAN_ModeType;

////////////////////////////// CAN MESSAGE STRUCTURE ///////////////////////////////////

typedef struct {
    uint32_t StdId;       // Standard identifier (11-bit, 0..0x7FF)
    uint32_t ExtId;       // Extended identifier (29-bit, 0..0x1FFFFFFF)
    uint8_t  IDE;         // 0 = standard frame, 1 = extended frame
    uint8_t  RTR;         // 0 = data frame, 1 = remote frame
    uint8_t  DLC;         // Data length code (0..8)
    uint8_t  Data[8];     // Payload bytes
} CAN_MsgType;

////////////////////////////// CALLBACK & HANDLE ///////////////////////////////////////

typedef void (*CAN_RxCallback_t)(const CAN_MsgType *msg);

typedef struct {
    CAN_BaudRateType    baudRate;
    CAN_ModeType        mode;
    CAN_ManualTypeDef  *regs;
    CAN_RxCallback_t    rxCallback;
} CAN_HandleType;

////////////////////////////// API DECLARATIONS ////////////////////////////////////////

// Constructor / initialisation
void CAN_constructor(CAN_HandleType *handle, CAN_ManualTypeDef *regs,
                     CAN_BaudRateType baudrate, CAN_ModeType mode);
void CAN_Init(CAN_HandleType *handle);

// Filter configuration (accept-all into FIFO 0)
void CAN_FilterAcceptAll(CAN_HandleType *handle);

// Polling TX: returns 0 on success, -1 on failure
int  CAN_Transmit(CAN_HandleType *handle, const CAN_MsgType *msg);

// Polling RX (FIFO 0): returns 0 if a message was read, -1 if FIFO empty
int  CAN_Receive(CAN_HandleType *handle, CAN_MsgType *msg);

// Interrupt-based RX (FIFO 0)
void CAN_EnableRXInterrupt(CAN_HandleType *handle, CAN_RxCallback_t callback);
void CAN_DisableRXInterrupt(CAN_HandleType *handle);

// Built-in test: loopback self-test (returns 0 on PASS, -1 on FAIL)
int  CAN_LoopbackTest(CAN_HandleType *handle, USART_HandleType *uart);

// Transceiver test: send and receive on real bus (returns 0 on PASS, -1 on FAIL)
int  CAN_TransceiverTest(CAN_HandleType *handle, USART_HandleType *uart);

#endif /* __BXCAN_H */
