/*
    * copyright Kolo Naukowe Elektornikow 2020.
    * Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
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
#include "fsl_gpio.h"
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
//    uart_config_t config;
    uart_transfer_t xfer;
    volatile uart_transfer_t sendXfer;
    (void)sendXfer;

    volatile uart_transfer_t receiveXfer;
    (void) receiveXfer;

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
