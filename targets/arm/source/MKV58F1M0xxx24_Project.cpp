/*
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
/**
 * @file    MKV58F1M0xxx24_Project.cpp
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKV58F24.h"
/* TODO: insert other include files here. */
#include "fsl_gpio.h"
/* TODO: insert other definitions and declarations here. */

#include "fsl_uart.h"
#include "fsl_port.h"

/*
 * @brief   Application entry point.
 */
#define ECHO_BUFFER_LENGTH 8

uart_handle_t g_uartHandle;
uint8_t g_tipString[] =
        "Uart interrupt example\r\nBoard receives 8 characters then sends them out\r\nNow please input:\r\n";

uint8_t g_txBuffer[ECHO_BUFFER_LENGTH] = {0};
uint8_t g_rxBuffer[ECHO_BUFFER_LENGTH] = {0};
volatile bool rxBufferEmpty = true;
volatile bool txBufferFull = false;
volatile bool txOnGoing = false;
volatile bool rxOnGoing = false;


void uart_callback(UART_Type *base, uart_handle_t *handle, status_t status, void *userData) {
    static bool state = false;
    if (state) {
        state = false;
        GPIO_PinWrite(GPIOA, 16U, 1U);
    } else {
        state = true;
        GPIO_PinWrite(GPIOA, 16U, 0U);
    }

//    userData = userData;

    (void)base;
    (void)handle;
    (void)status;
    (void)userData;
    if (kStatus_UART_TxIdle == status)
    {
        txBufferFull = false;
        txOnGoing = false;
    }

    if (kStatus_UART_RxIdle == status)
    {
        rxBufferEmpty = false;
        rxOnGoing = false;
    }


}
//void UART0_RX_TX_IRQHandler(void){
//    uart_callback();
//}

int main(void) {
    uart_config_t config;
    uart_transfer_t xfer;
    uart_transfer_t sendXfer;
    uart_transfer_t receiveXfer;

  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();


    /* Force the counter to be placed into memory. */
    volatile static long long i = 0 ;
    /* Enter an infinite loop, just incrementing a counter. */

    uart_config_t uartConfig;
    UART_GetDefaultConfig(&uartConfig);


    uartConfig.baudRate_Bps = 115200U;
//    uartConfig.parityMode = kUART_ParityDisabled;
//    uartConfig.stopBitCount = kUART_OneStopBit;
//    uartConfig.txFifoWatermark = 0;
//    uartConfig.rxFifoWatermark = 1;
    uartConfig.enableTx = true;
    uartConfig.enableRx = true;
    UART_Init(UART0, &uartConfig, 120000000);

    PORT_SetPinMux(PORTA, 14U, kPORT_MuxAlt3);
    PORT_SetPinMux(PORTA, 15U, kPORT_MuxAlt3);

    UART_WriteByte(UART0, 'e');


//    uint32_t state = 0;
//    (void)state;

    uart_handle_t uart_handle;

//    UART_TransferCreateHandle(DEMO_UART, &g_uartHandle, UART_UserCallback, NULL);

    UART_TransferCreateHandle(UART0, &uart_handle, reinterpret_cast<uart_transfer_callback_t>(uart_callback), nullptr);

    /* Send g_tipString out. */
    xfer.data = g_tipString;
    xfer.dataSize = sizeof(g_tipString) - 1;
    txOnGoing = true;
    UART_TransferSendNonBlocking(UART0, &g_uartHandle, &xfer);

    /* Wait send finished */
    while (txOnGoing)
    {
    }

    /* Start to echo. */
    sendXfer.data = g_txBuffer;
    sendXfer.dataSize = ECHO_BUFFER_LENGTH;
    receiveXfer.data = g_rxBuffer;
    receiveXfer.dataSize = ECHO_BUFFER_LENGTH;


//    UART_TransferHandleIRQ(UART0, &uart_handle);
    while(1) {

//        (void)i;
        i++ ;
        if (i > 100000) {
        	i = 0;
//        	static bool state = false;
//            if (state) {
//            	state = false;
//            	GPIO_PinWrite(GPIOA, 16U, 1U);
//            } else {
//            	state = true;
//            	GPIO_PinWrite(GPIOA, 16U, 0U);
//            }

            UART_WriteByte(UART0, 'c');
        }
        /* 'Dummy' NOP to allow source level single stepping of
            tight while() loop */
//        __asm volatile ("nop");
    }
    return 0 ;
}
