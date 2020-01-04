/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 * NXP HALina implementation of uart
 *
 */

#include "HALina.hpp"
#include "NXP_uart.hpp"
#include "HALina_config.hpp"
#include "fsl_uart.h"
#include "clock_config.h"

using namespace halina;

uart_handle_t uartHandle;

NXP_Uart* nxpUartHandler;

uint8_t txBuffer[1024];
uint8_t currentIndex = 0;

/* UART user callback */
void UART_UserCallback(UART_Type *base, uart_handle_t *handle, status_t status, void *userData){
    (void)base;
    (void)handle;
    (void)status;
    (void)userData;

    // todo callback to future using ex. in read method
}

NXP_Uart::NXP_Uart(PORT_Type* port, UART_Type* uart, uint8_t TXPin, uint8_t RXPin, uint32_t baudrate) : port(port), TXPin(TXPin), RXPin(RXPin), baudrate(baudrate), uart(uart) {
    nxpUartHandler = this;
}

void NXP_Uart::init(){
    PORT_SetPinMux(port, RXPin, kPORT_MuxAlt3);
    PORT_SetPinMux(port, TXPin, kPORT_MuxAlt3);

    uart_config_t config;

    UART_GetDefaultConfig(&config);
    config.baudRate_Bps = baudrate;
    config.enableTx = true;
    config.enableRx = true;

    UART_Init(uart, &config, 120000000);
//    UART_TransferStartRingBuffer(UART, &uartHandle, buffer, UART_BUFFER_SIZE);
    UART_TransferCreateHandle(uart, &uartHandle, UART_UserCallback, NULL);
}

void NXP_Uart::proc() {
    // todo implement when read method will be ready
}


void NXP_Uart::write(void const* data) {
    static uart_transfer_t dataTransfer;
    dataTransfer.data = (uint8_t*)data;
    dataTransfer.dataSize = strlen((char*)data);
    UART_TransferSendNonBlocking(uart, &uartHandle, &dataTransfer);
}

void NXP_Uart::writeChar(const char c) {
//    (void)c;
//    UART_EnableInterrupts(nxpUartHandler->uart, kUART_TxDataRegEmptyInterruptEnable);
//    while (!(nxpUartHandler->uart->S1 & UART_S1_TDRE_MASK))
//    {
//    }
    //nxpUartHandler->uart->D = c;
    static uart_transfer_t dataTransfer;
    txBuffer[currentIndex] = c;
    currentIndex++;
    dataTransfer.data = txBuffer;
    dataTransfer.dataSize = currentIndex;
    //if(0 == uartHandle.txState){
        if(kStatus_Success == UART_TransferSendNonBlocking(UART0, &uartHandle, &dataTransfer))
        {
            currentIndex = 0;
        }
    //}
}

char NXP_Uart::read() {
    // todo implement a read method
    return '0';
}

//void UART0_RX_TX_DriverIRQHandler(void){
//    if(NULL != nxpUartHandler){
//
//        uint32_t status = UART_GetStatusFlags(nxpUartHandler->uart);
//        if ((kUART_TxDataRegEmptyFlag & status) && (nxpUartHandler->uart->C2 & UART_C2_TIE_MASK)) {
////            uint8_t c;
////            if (true == RingBuffer_GetChar(txBuffer, &c)) {
////                nxpUartHandler->uart->D = c;
////            } else {
//                //UART_DisableInterrupts(nxpUartHandler->uart, kUART_TxDataRegEmptyInterruptEnable);
//            nxpUartHandler->uart->C2 = (nxpUartHandler->uart->C2 & ~UART_C2_TIE_MASK);
//           // }
//        }
//    }
//
//}
