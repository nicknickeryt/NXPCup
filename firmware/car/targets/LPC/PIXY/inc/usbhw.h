//
// begin license header
//
// This file is part of Pixy CMUcam5 or "Pixy" for short
//
// All Pixy source code is provided under the terms of the
// GNU General Public License v2 (http://www.gnu.org/licenses/gpl-2.0.html).
// Those wishing to use Pixy source code, software and/or
// technologies under different licensing terms should contact us at
// cmucam@cs.cmu.edu. Such licensing terms are available for
// all portions of the Pixy codebase presented here.
//
// end license header
//

/*----------------------------------------------------------------------------
 *      U S B  -  K e r n e l
 *----------------------------------------------------------------------------
 * Name:    usbhw.h
 * Purpose: USB Hardware Layer Definitions
 * Version: V1.20
 *----------------------------------------------------------------------------
 *      This software is supplied "AS IS" without any warranties, express,
 *      implied or statutory, including but not limited to the implied
 *      warranties of fitness for purpose, satisfactory quality and
 *      noninfringement. Keil extends you a royalty-free right to reproduce
 *      and distribute executable files created using this software for use
 *      on NXP Semiconductors LPC family microcontroller devices only. Nothing 
 *      else gives you the right to use this software.
 *
 * Copyright (c) 2009 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------
 * History:
 *          V1.20 Added USB_ClearEPBuf 
 *          V1.00 Initial Version
 *----------------------------------------------------------------------------*/

#ifndef __USBHW_H__
#define __USBHW_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* dTD Transfer Description */
typedef volatile struct
{
  volatile uint32_t next_dTD;
  volatile uint32_t total_bytes ;
  volatile uint32_t buffer0;
  volatile uint32_t buffer1;
  volatile uint32_t buffer2;
  volatile uint32_t buffer3;
  volatile uint32_t buffer4;
  volatile uint32_t reserved;
}  DTD_T;

/* dQH  Queue Head */
typedef volatile struct
{
  volatile uint32_t cap;
  volatile uint32_t curr_dTD;
  volatile uint32_t next_dTD;
  volatile uint32_t total_bytes;
  volatile uint32_t buffer0;
  volatile uint32_t buffer1;
  volatile uint32_t buffer2;
  volatile uint32_t buffer3;
  volatile uint32_t buffer4;
  volatile uint32_t reserved;
  volatile uint32_t setup[2];
  volatile uint32_t gap[4];
}  DQH_T;

/* bit defines for USBCMD register */
#define USBCMD_RS	    (1u<<0u)
#define USBCMD_RST	    (1u<<1u)
#define USBCMD_ATDTW 	(1u<<12u)
#define USBCMD_SUTW	    (1u<<13u)

/* bit defines for USBSTS register */
#define USBSTS_UI	    (1u<<0u)
#define USBSTS_UEI	    (1u<<1u)
#define USBSTS_PCI	    (1u<<2u)
#define USBSTS_URI	    (1u<<6u)
#define USBSTS_SRI	    (1u<<7u)
#define USBSTS_SLI	    (1u<<8u)
#define USBSTS_NAKI	    (1u<<16u)

/* bit defines for DEVICEADDR register */
#define USBDEV_ADDR_AD	(1u<<24u)
#define USBDEV_ADDR(n)	(((n) & 0x7Fu)<<25u)

/* bit defines for PRTSC1 register */
#define USBPRTS_CCS     (1u<<0u)
#define USBPRTS_PE      (1u<<2u)
#define USBPRTS_FPR     (1u<<6u)
#define USBPRTS_SUSP    (1u<<7u)
#define USBPRTS_PR      (1u<<8u)
#define USBPRTS_HSP     (1u<<9u)
#define USBPRTS_PLPSCD  (1u<<23u)
#define USBPRTS_PFSC    (1u<<24u)

/* bit defines for USBMODE register */
#define USBMODE_CM_IDLE	(0x0u<<0u)
#define USBMODE_CM_DEV	(0x2u<<0u)
#define USBMODE_CM_HOST	(0x3u<<0u)
#define USBMODE_SLOM    (1u<<3u)
#define USBMODE_SDIS    (1u<<4u)

/* bit defines for EP registers*/
#define USB_EP_BITPOS(n) (((n) & 0x80u)? (0x10u | ((n) & 0x7u)) : ((n) & 0x7u))

/* bit defines EPcontrol registers*/
#define EPCTRL_RXS	      (1u<<0u)
#define EPCTRL_RX_TYPE(n) (((n) & 0x3u)<<2u)
#define EPCTRL_RX_CTL	  (0u<<2u)
#define EPCTRL_RX_ISO	  (1u<<2u)
#define EPCTRL_RX_BLK	  (2u<<2u)
#define EPCTRL_RXI	      (1u<<5u)
#define EPCTRL_RXR	      (1u<<6u)
#define EPCTRL_RXE	      (1u<<7u)
#define EPCTRL_TXS	      (1u<<16u)
#define EPCTRL_TX_TYPE(n) (((n) & 0x3u)<<18u)
#define EPCTRL_TX_CTL	  (0u<<18u)
#define EPCTRL_TX_ISO	  (1u<<18u)
#define EPCTRL_TX_BLK	  (2u<<18u)
#define EPCTRL_TX_INT	  (3u<<18u)
#define EPCTRL_TXI	      (1u<<21u)
#define EPCTRL_TXR	      (1u<<22u)
#define EPCTRL_TXE	      (1u<<23u)

/* dQH field and bit defines */
/* Temp fixed on max, should be taken out of table */
#define QH_MAX_CTRL_PAYLOAD       0x03ff
#define QH_MAX_PKT_LEN_POS            16
#define QH_MAXP(n)                (((n) & 0x3FFu)<<16u)
#define QH_IOS                    (1u<<15u)
#define QH_ZLT                    (1u<<29u)

/* dTD field and bit defines */
#define TD_NEXT_TERMINATE         (1u<<0u)
#define TD_IOC                    (1u<<15u)

/* Total physical enpoints*/
#define EP_NUM_MAX	8u


/* USB Hardware Functions */
extern void  USB_Init       (LPC_USBDRV_INIT_T* cbs);
extern void  USB_Connect    (uint32_t  con);
extern void  USB_Reset      (void);
extern void  USB_Suspend    (void);
extern void  USB_Resume     (void);
extern void  USB_WakeUp     (void);
extern void  USB_WakeUpCfg  (uint32_t  cfg);
extern void  USB_SetAddress (uint32_t adr);
extern void  USB_Configure  (uint32_t  cfg);
extern void  USB_ConfigEP   (USB_ENDPOINT_DESCRIPTOR *pEPD);
extern void  USB_DirCtrlEP  (uint32_t dir);
extern void  USB_EnableEP   (uint32_t EPNum);
extern void  USB_DisableEP  (uint32_t EPNum);
extern void  USB_ResetEP    (uint32_t EPNum);
extern void  USB_SetStallEP (uint32_t EPNum);
extern void  USB_ClrStallEP (uint32_t EPNum);
extern void  USB_ClearEPBuf  (uint32_t  EPNum);
extern uint32_t USB_SetTestMode(uint8_t mode);
extern uint32_t USB_ReadEP     (uint32_t EPNum, const uint8_t *pData);
extern uint32_t USB_ReadReqEP(uint32_t EPNum, uint8_t *pData, uint32_t len);
extern uint32_t USB_ReadSetupPkt(uint32_t, uint32_t *);
extern uint32_t USB_WriteEP    (uint32_t EPNum, uint8_t *pData, uint32_t cnt);
extern uint32_t USB_GetFrame   (void);
//extern void  USB_ISR(void) __irq;

#ifdef __cplusplus
 }
#endif

#endif  /* __USBHW_H__ */
