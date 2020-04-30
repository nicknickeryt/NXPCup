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
 * Name:    usbuser.c
 * Purpose: USB Custom User Module
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
 *---------------------------------------------------------------------------*/
#include <cstring>
#include "chip.h"
#include "lpc_types.h"
#include "usb.h"
#include "usbcfg.h"
#include "usbhw.h"
#include "usbcore.h"
#include "usbuser.h"

void sendChunk();
void recvChunk();
void bulkOutNak();
/*
 *  USB Power Event Callback
 *   Called automatically on USB Power Event
 *    Parameter:       power: On(TRUE)/Off(FALSE)
 */
#if USB_POWER_EVENT
void USB_Power_Event (uint32_t  power) {
}
#endif
/*
 *  USB Reset Event Callback
 *   Called automatically on USB Reset Event
 */
#if USB_RESET_EVENT
void USB_Reset_Event (void) {
    USB_ResetCore();
}
#endif


/*
 *  USB Suspend Event Callback
 *   Called automatically on USB Suspend Event
 */

#if USB_SUSPEND_EVENT
void USB_Suspend_Event (void) { }
#endif


/*
 *  USB Resume Event Callback
 *   Called automatically on USB Resume Event
 */

#if USB_RESUME_EVENT
void USB_Resume_Event (void) { }
#endif


/*
 *  USB Remote Wakeup Event Callback
 *   Called automatically on USB Remote Wakeup Event
 */

#if USB_WAKEUP_EVENT
void USB_WakeUp_Event (void) {
}
#endif


/*
 *  USB Start of Frame Event Callback
 *   Called automatically on USB Start of Frame Event
 */

#if USB_SOF_EVENT
void USB_SOF_Event (void) {
}
#endif

/*
 *  USB Error Event Callback
 *   Called automatically on USB Error Event
 *    Parameter:       error: Error Code
 */
#if USB_ERROR_EVENT
void USB_Error_Event (uint32_t error) {
}
#endif

/*
 *  USB Set Configuration Event Callback
 *   Called automatically on USB Set Configuration Request
 */
#if USB_CONFIGURE_EVENT
void USB_Configure_Event (void) {

  if (USB_Configuration) {                  /* Check if USB is configured */
    /* add your code here */
  }
}
#endif

/*
 *  USB Set Interface Event Callback
 *   Called automatically on USB Set Interface Request
 */
#if USB_INTERFACE_EVENT
void USB_Interface_Event (void) {
}
#endif

/*
 *  USB Set/Clear Feature Event Callback
 *   Called automatically on USB Set/Clear Feature Request
 */
#if USB_FEATURE_EVENT
void USB_Feature_Event (void) {
}
#endif


#define P_EP(n) ((USB_EP_EVENT & (1 << (n))) ? USB_EndPoint##n : NULL)

/* USB Endpoint Events Callback Pointers */
void (* const USB_P_EP[USB_EP_NUM]) (uint32_t event) = {
    P_EP(0),
    P_EP(1),
    P_EP(2),
    P_EP(3),
};


/*
 *  USB Endpoint 1 Event Callback
 *   Called automatically on USB Endpoint 1 Event
 *    Parameter:       event
 */

void USB_EndPoint1 (uint32_t event) {
    if (event == USB_EVT_IN) {
        return;
    }
}


/*
 *  USB Endpoint 2 Event Callback
 *   Called automatically on USB Endpoint 2 Event
 *    Parameter:       event
 */

//void testWrite()
//{
//	static int flag = 0;
//   	auto *memory = (unsigned int *)0x10000000;
//	int i;
//
//	if (!flag)
//	{
//		for (i=0; i<0x2000; i++)
//			memory[i] = (unsigned int)&memory[i];
//		flag = 1;
//	}
//
//	USB_WriteEP(USB_BULK_IN_EP, (uint8_t *)memory, 0x4000);
//}

void USB_EndPoint2 (uint32_t event) {

    switch (event) {
        case USB_EVT_OUT:
            recvChunk();
            break;
        case USB_EVT_IN:  // this gets called upon completion
            sendChunk();
            break;
        default:
            return;
    }
}

/*
 *  USB Endpoint 3 Event Callback
 *   Called automatically on USB Endpoint 3 Event
 *    Parameter:       event
 */

void USB_EndPoint3 (uint32_t event) { }


/*
 *  USB Endpoint 4 Event Callback
 *   Called automatically on USB Endpoint 4 Event
 *    Parameter:       event
 */

void USB_EndPoint4 (uint32_t event) { }


/*
 *  USB Endpoint 5 Event Callback
 *   Called automatically on USB Endpoint 5 Event
 *    Parameter:       event
 */

void USB_EndPoint5 (uint32_t event) {
  event = event;
}

void USB_UserInit() {
    LPC_USBDRV_INIT_T usb_cb;

    /* initilize call back structures */
    memset((void*)&usb_cb, 0, sizeof(LPC_USBDRV_INIT_T));
    usb_cb.USB_Reset_Event = USB_Reset_Event;
    usb_cb.USB_P_EP[0] = USB_EndPoint0;
    usb_cb.USB_P_EP[1] = USB_EndPoint1;
    usb_cb.USB_P_EP[2] = USB_EndPoint2;
    usb_cb.ep0_maxp = USB_MAX_PACKET0;

    USB_Init(&usb_cb);                        // USB Initialization
    USB_Connect(true);                        // USB Connect
}

// monitor usb connector plug/unplugs
uint32_t USB_handleState() {
	uint32_t susp;

	if (USB_Configuration) {
		susp = LPC_USB->PORTSC1_D & (1u<<7u);
		if (susp) {
			USB_ResetEP(USB_BULK_OUT_EP);
			USB_ResetEP(USB_BULK_IN_EP);
			USB_Configuration = 0;
			// TOOD printf
			// printf("reset configuration\n");
		}
	}

	return USB_Configuration;
}

void bulkOutNak(){
    //USB_ReadReqEP(USB_BULK_OUT_EP, (uint8_t *)buf, len);
}

#define MAX_CHUNK		     0x4000

const uint8_t *g_recvData = nullptr;
uint32_t g_recvLen = 0;
uint32_t g_recvOffset = 0;
volatile uint8_t g_recvComplete = 0;
volatile uint32_t g_recvTimerStart = 0;

const uint8_t *g_sendData = nullptr;
uint32_t g_sendLen = 0;
uint32_t g_sendOffset = 0;
volatile uint8_t g_sendComplete = 0;
volatile uint32_t g_sendTimerStart = 0;



void sendChunk() {
	uint32_t chunk;
	uint8_t *data;

	// reset timer
	g_sendTimerStart = LPC_TIMER1->TC;

	if (g_sendOffset>=g_sendLen) {
		g_sendComplete = 1;
		return;
	}

	if (g_sendLen-g_sendOffset>=USB_MAX_CHUNK) {
        chunk = USB_MAX_CHUNK;
	} else {
        chunk = g_sendLen-g_sendOffset;
	}

	data = (uint8_t *)g_sendData + g_sendOffset;
	// advance g_sendOffset before we call USB_WriteEP, 
	// otherwise we create a race condition which can cause multiple 
	// of the same packet to be sent
	g_sendOffset += chunk;
	USB_WriteEP(USB_BULK_IN_EP, data, chunk);
}

void recvChunk() {
	uint32_t chunk;
	uint8_t *data;

	// reset timer
	g_recvTimerStart = LPC_TIMER1->TC;

	if (g_recvOffset>=g_recvLen) {
		g_recvComplete = 1;
		return;
	}

	if (g_recvLen-g_recvOffset>=USB_MAX_CHUNK) {
        chunk = USB_MAX_CHUNK;
	} else {
        chunk = g_recvLen-g_recvOffset;
	}

	data = (uint8_t *)g_recvData + g_recvOffset;
	// advance g_recvOffset before we call USB_ReadReqEP, 
	// otherwise we create a race condition which can cause us 
	// to miss received packets
	g_recvOffset += chunk;
	USB_ReadReqEP(USB_BULK_OUT_EP, data, chunk);
}								 

void USB_Send(const uint8_t *data, uint32_t len) {
	if ((uint32_t)data & 0x3u) {
        return;
	}

	g_sendData = data;
	g_sendLen = len;
	g_sendOffset = 0;
	g_sendComplete = 0;

	sendChunk();
}


void USB_Recv(const uint8_t *data, uint32_t len) {
	if ((uint32_t)data & 0x03u) {
        return;
	}

	g_recvData = data;
	g_recvLen = len;
	g_recvOffset = 0;
	g_recvComplete = 0;

	recvChunk();
}  

void USB_RecvReset() {
	USB_ResetEP(USB_BULK_OUT_EP);
}

void USB_SendReset() {
	USB_ResetEP(USB_BULK_IN_EP);
}