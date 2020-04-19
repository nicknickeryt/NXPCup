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

/**********************************************************************
* $Id$		usbhw.c					2011-06-02
*//**
* @file		usbhw.c
* @brief	 USB Hardware Layer Module for NXP's lpc43xx MCU
* @version	1.0
* @date		02. June. 2011
* @author	NXP MCU SW Application Team
*
* Copyright(C) 2011, NXP Semiconductor
* All rights reserved.
*
***********************************************************************
* Software that is described herein is for illustrative purposes only
* which provides customers with programming information regarding the
* products. This software is supplied "AS IS" without any warranties.
* NXP Semiconductors assumes no responsibility or liability for the
* use of the software, conveys no license or title under any patent,
* copyright, or mask work right to the product. NXP Semiconductors
* reserves the right to make changes in the software without
* notification. NXP Semiconductors also make no representation or
* warranty that such application will be suitable for the specified
* use without further testing or modification.
* Permission to use, copy, modify, and distribute this software and its
* documentation is hereby granted, under NXP Semiconductors'
* relevant copyright in the software, without fee, provided that it
* is used in conjunction with NXP Semiconductors microcontrollers.  This
* copyright, permission, and disclaimer notice must appear in all copies of
* this code.
**********************************************************************/
#include <cstring>
#include "chip.h"
#include "lpc_types.h"
#include "usb.h"
#include "usbhw.h"
#include "usbcfg.h"
#include "usbcore.h"

#define __align(x) __attribute__((aligned(x)))
DQH_T ep_QH[EP_NUM_MAX] __attribute__((aligned(2048)));
DTD_T ep_TD[EP_NUM_MAX] __attribute__((aligned(32)));

typedef enum {
    CGU_ERROR_SUCCESS = 0,
    CGU_ERROR_CONNECT_TOGETHER,
    CGU_ERROR_INVALID_ENTITY,
    CGU_ERROR_INVALID_CLOCK_SOURCE,
    CGU_ERROR_INVALID_PARAM,
    CGU_ERROR_FREQ_OUTOF_RANGE
} CGU_ERROR;

/*********************************************************************//**
 * @brief		Get current USB PLL clock from XTAL
 * @param[in]	None
 * @return 		Returned clock value
 **********************************************************************/
uint32_t CGU_SetPLL0(){
    // Setup PLL550 to generate 480MHz from 12 MHz crystal
    LPC_CGU->PLL[CGU_USB_PLL].PLL_CTRL |= 1u;
//    LPC_CGU->PLL0USB_CTRL |= 1; 	// Power down PLL
    //	P			N
//    LPC_CGU->PLL0USB_NP_DIV = (98<<0) | (514<<12);
    LPC_CGU->PLL[CGU_USB_PLL].PLL_NP_DIV = (98u<<0u) | (514u<<12u);
    //	SELP	SELI	SELR	MDEC
//    LPC_CGU->PLL0USB_MDIV = (0xB<<17)|(0x10<<22)|(0<<28)|(0x7FFA<<0);
    LPC_CGU->PLL[CGU_USB_PLL].PLL_MDIV = (0xBu<<17u)|(0x10u<<22u)|(0u<<28u)|(0x7FFAu<<0u);
//    LPC_CGU->PLL0USB_CTRL =(CGU_CLKSRC_XTAL_OSC<<24) | (0x3<<2) | (1<<4);
// TODO CGU_CLKSRC_XTAL_OSC = 6
    LPC_CGU->PLL[CGU_USB_PLL].PLL_CTRL = (6u<<24u) | (0x3u<<2u) | (1u<<4u);
    return CGU_ERROR_SUCCESS;
}

static uint32_t ep_read_len[4];
volatile uint32_t DevStatusFS2HS = FALSE;
LPC_USBDRV_INIT_T g_drv;

/*
 *  Get Endpoint Physical Address
 *    Parameters:      EPNum: Endpoint Number
 *                       EPNum.0..3: Address
 *                       EPNum.7:    Dir
 *    Return Value:    Endpoint Physical Address
 */

uint32_t EPAdr (uint32_t EPNum) {
    uint32_t val;

    val = (EPNum & 0x0Fu) << 1u;
    if (EPNum & 0x80u) {
        val += 1;
    }
    return (val);
}

/*
 *  USB Initialize Function
 *   Called by the User to initialize USB
 *    Return Value:    None
 */

void USB_Init (LPC_USBDRV_INIT_T* cbs) {
    memcpy(&g_drv, cbs, sizeof(LPC_USBDRV_INIT_T));
    /*maxp for EP0 should be atleast 8 */
    if( g_drv.ep0_maxp == 0) {
        g_drv.ep0_maxp = 64;
    }


#ifdef USE_USB0
  // TODO
//  NXP_SCU::enable(NXP_SCU::Type::PIN, NXP_SCU::Function::_1,
//          NXP_SCU::Mode_INACT | NXP_SCU::Mode_INBUFF_EN | NXP_SCU::Mode_ZIF_DIS | NXP_SCU::Mode_HIGHSPEEDSLEW_EN,
//          0x8, 1);
//  NXP_SCU::enable(NXP_SCU::Type::PIN, NXP_SCU::Function::_1,
//          NXP_SCU::Mode_INACT | NXP_SCU::Mode_INBUFF_EN | NXP_SCU::Mode_ZIF_DIS | NXP_SCU::Mode_HIGHSPEEDSLEW_EN,
//          0x8, 2);
//	scu_pinmux(0x8,1,MD_PLN_FAST,FUNC1);    //  0: motocon pcap0_1          1: usb0 usb0_ind1           2:  nc                      3: gpio4 gpio4_1
//	scu_pinmux(0x8,2,MD_PLN_FAST,FUNC1);    //  0: motocon pcap0_0          1: usb0 usb0_ind0           2:  nc                      3: gpio4 gpio4_2
#endif
#ifdef USE_USB0

	CGU_SetPLL0();
//	CGU_EnableEntity(CGU_CLKSRC_PLL0, ENABLE);
//	CGU_EntityConnect(CGU_CLKSRC_PLL0, CGU_BASE_USB0);

    Chip_Clock_SetBaseClock(CLK_BASE_USB0, CLKIN_MAINPLL, false, false);
#else
	CGU_EntityConnect(CGU_CLKSRC_PLL1, CGU_BASE_USB1);
	/* enable USB phy */
    LPC_CREG->CREG0 &= ~(1 << 5);
    /* enable USB1_DP and USB1_DN on chip FS phy */
    LPC_SCU->SFSUSB = 0x12;
    /* enable USB1_VBUS */
    scu_pinmux(0x2, 5, MD_PLN | MD_EZI | MD_ZI, FUNC2);

#endif
	/* Turn on the phy */
#ifdef USE_USB0
	LPC_CREG->CREG0 &= ~(1<<5);
#endif
	/* reset the controller */
	LPC_USB->USBCMD_D = USBCMD_RST;
	/* wait for reset to complete */
	while (LPC_USB->USBCMD_D & USBCMD_RST);

	/* Program the controller to be the USB device controller */
	LPC_USB->USBMODE_D =   USBMODE_CM_DEV
	                  | USBMODE_SDIS
	                  | USBMODE_SLOM ;

	/* set OTG transcever in proper state, device is present
	on the port(CCS=1), port enable/disable status change(PES=1). */
#ifdef USE_USB0	
	LPC_USB->OTGSC = (1u<<3u) | (1u<<0u) /*| (1<<16)| (1<<24)| (1<<25)| (1<<26)| (1<<27)| (1<<28)| (1<<29)| (1<<30)*/;
#else
	/* force full speed */
	LPC_USB->PORTSC1_D |= (1<<24);
#endif

#ifdef USE_USB0
	NVIC_SetPriority(USB0_IRQn, 1);	// slightly lower priority interrupt
	NVIC_EnableIRQ(USB0_IRQn); //  enable USB0 interrrupts
#else
	NVIC_SetPriority(USB1_IRQn, 1);	// slightly lower priority interrupt
  	NVIC_EnableIRQ(USB1_IRQn); //  enable USB1 interrrupts
#endif

	USB_Reset();
	USB_SetAddress(0);
}

/*
 *  USB Connect Function
 *   Called by the User to Connect/Disconnect USB
 *    Parameters:      con:   Connect/Disconnect
 *    Return Value:    None
 */
void USB_Connect (uint32_t con) {
    if (con) {
        LPC_USB->USBCMD_D |= USBCMD_RS;
    } else {
        LPC_USB->USBCMD_D &= ~USBCMD_RS;
    }
}


/*
 *  USB Reset Function
 *   Called automatically on USB Reset
 *    Return Value:    None
 */

void USB_Reset () {
    uint32_t i;

    DevStatusFS2HS = FALSE;
    /* disable all EPs */
    LPC_USB->ENDPTCTRL[0] &= ~(EPCTRL_RXE | EPCTRL_TXE); // ENDPTCTRL0
    LPC_USB->ENDPTCTRL[2] &= ~(EPCTRL_RXE | EPCTRL_TXE); // ENDPTCTRL2
    LPC_USB->ENDPTCTRL[3] &= ~(EPCTRL_RXE | EPCTRL_TXE); // ENDPTCTRL3

    /* Clear all pending interrupts */
    LPC_USB->ENDPTNAK   = 0xFFFFFFFF;
    LPC_USB->ENDPTNAKEN = 0;
    LPC_USB->USBSTS_D     = 0xFFFFFFFF;
    LPC_USB->ENDPTSETUPSTAT = LPC_USB->ENDPTSETUPSTAT;
    LPC_USB->ENDPTCOMPLETE  = LPC_USB->ENDPTCOMPLETE;
    while (LPC_USB->ENDPTPRIME) { } // Wait until all bits are 0

    LPC_USB->ENDPTFLUSH = 0xFFFFFFFF;
    while (LPC_USB->ENDPTFLUSH) { }; /* Wait until all bits are 0 */

    /* Set the interrupt Threshold control interval to 0 */
    LPC_USB->USBCMD_D &= ~0x00FF0000u;

    /* Zero out the Endpoint queue heads */
    memset((void*)ep_QH, 0, EP_NUM_MAX * sizeof(DQH_T));
    /* Zero out the device transfer descriptors */
    memset((void*)ep_TD, 0, EP_NUM_MAX * sizeof(DTD_T));
    memset((void*)ep_read_len, 0, sizeof(ep_read_len));
    /* Configure the Endpoint List Address */
    /* make sure it in on 64 byte boundary !!! */
    /* init list address */
    LPC_USB->ENDPOINTLISTADDR = (uint32_t)ep_QH;
    /* Initialize device queue heads for non ISO endpoint only */
    for (i = 0; i < EP_NUM_MAX; i++) {
        ep_QH[i].next_dTD = (uint32_t)&ep_TD[i];
    }
    /* Enable interrupts */
    LPC_USB->USBINTR_D =  USBSTS_UI | USBSTS_UEI | USBSTS_PCI | USBSTS_URI | USBSTS_SLI | USBSTS_NAKI;
    //  LPC_USB->usbintr |= (0x1<<7);		/* Test SOF */
    /* enable ep0 IN and ep0 OUT */
    ep_QH[0].cap  = QH_MAXP(g_drv.ep0_maxp) | QH_IOS | QH_ZLT;
    ep_QH[1].cap  = QH_MAXP(g_drv.ep0_maxp) | QH_IOS | QH_ZLT;
    /* enable EP0 */
    LPC_USB->ENDPTCTRL[0] = EPCTRL_RXE | EPCTRL_RXR | EPCTRL_TXE | EPCTRL_TXR; // ENDPTCTRL0
}


/*
 *  USB Suspend Function
 *   Called automatically on USB Suspend
 *    Return Value:    None
 */

void USB_Suspend (void) {
  /* Performed by Hardware */
}


/*
 *  USB Resume Function
 *   Called automatically on USB Resume
 *    Return Value:    None
 */

void USB_Resume (void) {
  /* Performed by Hardware */
}


/*
 *  USB Remote Wakeup Function
 *   Called automatically on USB Remote Wakeup
 *    Return Value:    None
 */

void USB_WakeUp (void) {
    LPC_USB->PORTSC1_D |= USBPRTS_FPR ;
}


/*
 *  USB Remote Wakeup Configuration Function
 *    Parameters:      cfg:   Enable/Disable
 *    Return Value:    None
 */

void USB_WakeUpCfg (uint32_t cfg) {
    (void) cfg;
}


/*
 *  USB Set Address Function
 *    Parameters:      adr:   USB Address
 *    Return Value:    None
 */

void USB_SetAddress (uint32_t adr) {
    LPC_USB->DEVICEADDR = USBDEV_ADDR(adr);
    LPC_USB->DEVICEADDR |= USBDEV_ADDR_AD;
}

/*
*  USB set test mode Function
*    Parameters:      mode:   test mode
*    Return Value:    TRUE if supported else FALSE
*/

uint32_t USB_SetTestMode(uint8_t mode) {
    uint32_t portsc;

    if ((mode > 0) && (mode < 8)) {
        portsc = LPC_USB->PORTSC1_D & ~(0xFu << 16u);

        LPC_USB->PORTSC1_D = portsc | (mode << 16u);
        return true;
    }
    return false;
}

/*
 *  USB Configure Function
 *    Parameters:      cfg:   Configure/Deconfigure
 *    Return Value:    None
 */

void USB_Configure (uint32_t cfg) {
    (void) cfg;
}


/*
 *  Configure USB Endpoint according to Descriptor
 *    Parameters:      pEPD:  Pointer to Endpoint Descriptor
 *    Return Value:    None
 */

void USB_ConfigEP (USB_ENDPOINT_DESCRIPTOR *pEPD) {
    uint32_t num, lep;
    uint32_t ep_cfg;
    uint8_t  bmAttributes;

    lep = pEPD->bEndpointAddress & 0x7Fu;
    num = EPAdr(pEPD->bEndpointAddress);

    ep_cfg = ((uint32_t*)&(LPC_USB->ENDPTCTRL[0]))[lep];
    /* mask the attributes we are not-intersetd in */
    bmAttributes = pEPD->bmAttributes & USB_ENDPOINT_TYPE_MASK;
    /* set EP type */
    if (bmAttributes != USB_ENDPOINT_TYPE_ISOCHRONOUS) {
        /* init EP capabilities */
        ep_QH[num].cap  = QH_MAXP(pEPD->wMaxPacketSize) | QH_IOS | QH_ZLT ;
        /* The next DTD pointer is INVALID */
        ep_TD[num].next_dTD = 0x01 ;
    } else {
        /* init EP capabilities */
        ep_QH[num].cap  = QH_MAXP(0x400u) | QH_ZLT;
    }
    /* setup EP control register */
    if (pEPD->bEndpointAddress & 0x80u) {
        ep_cfg &= ~0xFFFF0000;
        ep_cfg |= EPCTRL_TX_TYPE(bmAttributes) | EPCTRL_TXR;
    } else {
        ep_cfg &= ~0xFFFFu;
        ep_cfg |= EPCTRL_RX_TYPE(bmAttributes) | EPCTRL_RXR;
    }
    ((uint32_t*)&(LPC_USB->ENDPTCTRL[0]))[lep] = ep_cfg; // ENDPTCTRL0
}

/*
 *  Set Direction for USB Control Endpoint
 *    Parameters:      dir:   Out (dir == 0), In (dir <> 0)
 *    Return Value:    None
 */

void USB_DirCtrlEP (uint32_t dir) {
    (void) dir;
}


/*
 *  Enable USB Endpoint
 *    Parameters:      EPNum: Endpoint Number
 *                       EPNum.0..3: Address
 *                       EPNum.7:    Dir
 *    Return Value:    None
 */

void USB_EnableEP (uint32_t EPNum) {
    uint32_t lep, bitpos;

    lep = EPNum & 0x0Fu;

    if (EPNum & 0x80u) {
        ((uint32_t*)&(LPC_USB->ENDPTCTRL[0]))[lep] |= EPCTRL_TXE; // ENDPTCTRL0
    } else {
        ((uint32_t*)&(LPC_USB->ENDPTCTRL[0]))[lep] |= EPCTRL_RXE; // ENDPTCTRL0
        /* enable NAK interrupt */
        bitpos = USB_EP_BITPOS(EPNum);
        LPC_USB->ENDPTNAKEN |= (1u<<bitpos);
    }
}

/*
 *  Disable USB Endpoint
 *    Parameters:      EPNum: Endpoint Number
 *                       EPNum.0..3: Address
 *                       EPNum.7:    Dir
 *    Return Value:    None
 */

void USB_DisableEP (uint32_t EPNum) {
    uint32_t lep, bitpos;

    lep = EPNum & 0x0Fu;
    if (EPNum & 0x80u) {
        ((uint32_t*)&(LPC_USB->ENDPTCTRL[0]))[lep] &= ~EPCTRL_TXE;
    } else {
        /* disable NAK interrupt */
        bitpos = USB_EP_BITPOS(EPNum);
        LPC_USB->ENDPTNAKEN &= ~(1u<<bitpos);
        ((uint32_t*)&(LPC_USB->ENDPTCTRL[0]))[lep] &= ~EPCTRL_RXE;
    }
}

/*
 *  Reset USB Endpoint
 *    Parameters:      EPNum: Endpoint Number
 *                       EPNum.0..3: Address
 *                       EPNum.7:    Dir
 *    Return Value:    None
 */

void USB_ResetEP (uint32_t EPNum) {
    uint32_t bit_pos = USB_EP_BITPOS(EPNum);
    uint32_t lep = EPNum & 0x0Fu;

    /* flush EP buffers */
    LPC_USB->ENDPTFLUSH = (1u<<bit_pos);
    while (LPC_USB->ENDPTFLUSH & (1u<<bit_pos));
    /* reset data toggles */
    if (EPNum & 0x80u) {
        ((uint32_t*)&(LPC_USB->ENDPTCTRL[0]))[lep] |= EPCTRL_TXR;
    } else {
        ((uint32_t*)&(LPC_USB->ENDPTCTRL[0]))[lep] |= EPCTRL_RXR;
    }
}

/*
 *  Set Stall for USB Endpoint
 *    Parameters:      EPNum: Endpoint Number
 *                       EPNum.0..3: Address
 *                       EPNum.7:    Dir
 *    Return Value:    None
 */

void USB_SetStallEP (uint32_t EPNum) {
    uint32_t lep;

    lep = EPNum & 0x0Fu;
    if (EPNum & 0x80u) {
        ((uint32_t*)&(LPC_USB->ENDPTCTRL[0]))[lep] |= EPCTRL_TXS;
    } else {
        ((uint32_t*)&(LPC_USB->ENDPTCTRL[0]))[lep] |= EPCTRL_RXS;
    }
}

/*
 *  Clear Stall for USB Endpoint
 *    Parameters:      EPNum: Endpoint Number
 *                       EPNum.0..3: Address
 *                       EPNum.7:    Dir
 *    Return Value:    None
 */

void USB_ClrStallEP (uint32_t EPNum) {
    uint32_t lep;

    lep = EPNum & 0x0Fu;
    if (EPNum & 0x80u) {
        ((uint32_t*)&(LPC_USB->ENDPTCTRL[0]))[lep] &= ~EPCTRL_TXS;
        /* reset data toggle */
        ((uint32_t*)&(LPC_USB->ENDPTCTRL[0]))[lep] |= EPCTRL_TXR;
    } else {
        ((uint32_t*)&(LPC_USB->ENDPTCTRL[0]))[lep] &= ~EPCTRL_RXS;
        /* reset data toggle */
        ((uint32_t*)&(LPC_USB->ENDPTCTRL[0]))[lep] |= EPCTRL_RXR;
    }
}

/*
 *  Process DTD
 *    Parameters:      EPNum: Endpoint Number
 *                       EPNum.0..3: Address
 *                       EPNum.7:    Dir
 *                     Buffer pointer
 *                     Transfer buffer size
 *    Return Value:    None
 */
void USB_ProgDTD(uint32_t Edpt, uint32_t ptrBuff, uint32_t TsfSize) {
    DTD_T*  pDTD;

    pDTD = (DTD_T*)&ep_TD[ Edpt ];

    /* Zero out the device transfer descriptors */
    memset((void*)pDTD, 0, sizeof(DTD_T));
    /* The next DTD pointer is INVALID */
    pDTD->next_dTD = 0x01 ;

    /* Length */
    pDTD->total_bytes = ((TsfSize & 0x7fffu) << 16u);
    pDTD->total_bytes |= TD_IOC ;
    pDTD->total_bytes |= 0x80u ;

    pDTD->buffer0 = ptrBuff;
    pDTD->buffer1 = (ptrBuff + 0x1000) & 0xfffff000;
    pDTD->buffer2 = (ptrBuff + 0x2000) & 0xfffff000;
    pDTD->buffer3 = (ptrBuff + 0x3000) & 0xfffff000;
    pDTD->buffer4 = (ptrBuff + 0x4000) & 0xfffff000;

    ep_QH[Edpt].next_dTD = (uint32_t)(&ep_TD[ Edpt ]);
    ep_QH[Edpt].total_bytes &= (~0xC0u) ;
}

/*
*  Read USB Endpoint Data
*    Parameters:      EPNum: Endpoint Number
*                       EPNum.0..3: Address
*                       EPNum.7:    Dir
*                     pData: Pointer to Data Buffer
*    Return Value:    Number of bytes read
*/
uint32_t USB_ReadSetupPkt(uint32_t EPNum, uint32_t *pData) {
    uint32_t setup_int, cnt = 0;
    uint32_t num = EPAdr(EPNum);

    setup_int = LPC_USB->ENDPTSETUPSTAT ;
    /* Clear the setup interrupt */
    LPC_USB->ENDPTSETUPSTAT = setup_int;

    /* ********************************** */
    /*  Check if we have received a setup */
    /* ********************************** */
    if (setup_int & (1u<<0u))  {                  /* Check only for bit 0 */
        /* No setup are admitted on other endpoints than 0 */
        do {
            /* Setup in a setup - must considere only the second setup */
            /*- Set the tripwire */
            LPC_USB->USBCMD_D |= USBCMD_SUTW ;

            /* Transfer Set-up data to the gtmudsCore_Request buffer */
            pData[0] = ep_QH[num].setup[0];
            pData[1] = ep_QH[num].setup[1];
            cnt = 8;
        } while (!(LPC_USB->USBCMD_D & USBCMD_SUTW)) ;

        /* setup in a setup - Clear the tripwire */
        LPC_USB->USBCMD_D &= (~USBCMD_SUTW);
    }
    while ((setup_int = LPC_USB->ENDPTSETUPSTAT) != 0) {
        /* Clear the setup interrupt */
        LPC_USB->ENDPTSETUPSTAT = setup_int;
    }
    return cnt;
}

/*
*  Enque read request
*    Parameters:      EPNum: Endpoint Number
*                       EPNum.0..3: Address
*                       EPNum.7:    Dir
*                     pData: Pointer to Data Buffer
*    Return Value:    Number of bytes read
*/

uint32_t USB_ReadReqEP(uint32_t EPNum, uint8_t *pData, uint32_t len) {
    uint32_t num = EPAdr(EPNum);
    uint32_t n = USB_EP_BITPOS(EPNum);

    USB_ProgDTD(num, (uint32_t)pData, len);
    ep_read_len[EPNum & 0x0Fu] = len;
    /* prime the endpoint for read */
    LPC_USB->ENDPTPRIME |= (1u<<n);
    return len;
}
/*
*  Read USB Endpoint Data
*    Parameters:      EPNum: Endpoint Number
*                       EPNum.0..3: Address
*                       EPNum.7:    Dir
*                     pData: Pointer to Data Buffer
*    Return Value:    Number of bytes read
*/

uint32_t USB_ReadEP(uint32_t EPNum, const uint8_t *pData) {
    (void) pData;
    uint32_t cnt, n;
    DTD_T*  pDTD ;

    n = EPAdr(EPNum);
    pDTD = (DTD_T*)&ep_TD[n];

    /* return the total bytes read */
    cnt  = (pDTD->total_bytes >> 16u) & 0x7FFFu;
    cnt = ep_read_len[EPNum & 0x0Fu] - cnt;
    return (cnt);
}

/*
*  Write USB Endpoint Data
*    Parameters:      EPNum: Endpoint Number
*                       EPNum.0..3: Address
*                       EPNum.7:    Dir
*                     pData: Pointer to Data Buffer
*                     cnt:   Number of bytes to write
*    Return Value:    Number of bytes written
*/
uint32_t USB_WriteEP(uint32_t EPNum, uint8_t *pData, uint32_t cnt) {
    uint32_t n = USB_EP_BITPOS(EPNum);

    USB_ProgDTD(EPAdr(EPNum), (uint32_t)pData, cnt);
    /* prime the endpoint for transmit */
    LPC_USB->ENDPTPRIME |= (1u<<n);

    /* check if priming succeeded */
    while (LPC_USB->ENDPTPRIME & (1u<<n));
    return (cnt);
}

/*
 *  USB Interrupt Service Routine
 */
extern "C" void USB0_IRQHandler (void) {
    uint32_t disr, val, n;

    disr = LPC_USB->USBSTS_D; // Device Interrupt Status
    LPC_USB->USBSTS_D = disr;

    // Device Status Interrupt (Reset, Connect change, Suspend/Resume)
    if (disr & USBSTS_URI) { // Reset
        USB_Reset();
        if (g_drv.USB_Reset_Event) {
            g_drv.USB_Reset_Event();
        }
        return;
    }

    if (disr & USBSTS_SLI) { // Suspend
        if (g_drv.USB_Suspend_Event) {
            g_drv.USB_Suspend_Event();
        }
    }

    if (disr & USBSTS_PCI) { // Resume
        if (LPC_USB->PORTSC1_D & (1u<<9u)) { //  check if device isoperating in HS mode or full speed
            DevStatusFS2HS = true;
        }

        if (g_drv.USB_Resume_Event) {
            g_drv.USB_Resume_Event();
        }
    }

    val = LPC_USB->ENDPTSETUPSTAT; // handle setup status interrupts
    /* Only EP0 will have setup packets so call EP0 handler */
    if (val) {
        /* Clear the endpoint complete CTRL OUT & IN when */
        /* a Setup is received */
        LPC_USB->ENDPTCOMPLETE = 0x00010001u;
        /* enable NAK inetrrupts */
        LPC_USB->ENDPTNAKEN |= 0x00010001u;
        if (g_drv.USB_P_EP[0]){
            g_drv.USB_P_EP[0](USB_EVT_SETUP);
        }
    }

    /* handle completion interrupts */
    val = LPC_USB->ENDPTCOMPLETE;
    if (val) {
        LPC_USB->ENDPTNAK = val;
        for (n = 0; n < EP_NUM_MAX / 2; n++) {
            if (val & (1u<<n)) {
                if (g_drv.USB_P_EP[n]) {
                    g_drv.USB_P_EP[n](USB_EVT_OUT);
                }
                LPC_USB->ENDPTCOMPLETE = (1u<<n);
            }

            if (val & (1u<<(n + 16u))) {
                ep_TD [(n << 1u) + 1u ].total_bytes &= 0xC0u;
                if (g_drv.USB_P_EP[n]) {
                    g_drv.USB_P_EP[n](USB_EVT_IN);
                }
                LPC_USB->ENDPTCOMPLETE = (1u<<(n + 16u));
            }
        }
    }

    if (disr & USBSTS_NAKI) {
        val = LPC_USB->ENDPTNAK;
        val &= LPC_USB->ENDPTNAKEN;
        /* handle NAK interrupts */
        if (val) {
            for (n = 0; n < EP_NUM_MAX / 2; n++)  {
                if (val & (1u<<n)) {
                    if (g_drv.USB_P_EP[n]) {
                        g_drv.USB_P_EP[n](USB_EVT_OUT_NAK);
                    }
                }
                if (val & (1u<<(n + 16u))) {
                    if (g_drv.USB_P_EP[n]) {
                        g_drv.USB_P_EP[n](USB_EVT_IN_NAK);
                    }
                }
            }
            LPC_USB->ENDPTNAK = val;
        }
    }

    /* Start of Frame Interrupt */
    if (disr & USBSTS_SRI) {
        if (g_drv.USB_SOF_Event)
        g_drv.USB_SOF_Event();
        }

    /* Error Interrupt */
    if (disr & USBSTS_UEI) {
        if (g_drv.USB_Error_Event) {
            g_drv.USB_Error_Event(disr);
        }
    }
}

