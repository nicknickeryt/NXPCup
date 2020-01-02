/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 * NXP HALina implementation of uart
 *
 */

#include "NXP_hal.hpp"

using namespace halina;

uint8_t buffer[NXP_Uart::bufferLength] = {0};
uart_handle_t uartHandle;

/* UART user callback */
void UART_UserCallback(UART_Type *base, uart_handle_t *handle, status_t status, void *userData){
    (void)base;
    (void)handle;
    (void)status;
    (void)userData;

    // todo callback to future using ex. in read method
}

void NXP_Uart::init(){
    PORT_SetPinMux(PORTA, 14U, kPORT_MuxAlt3);
    PORT_SetPinMux(PORTA, 15U, kPORT_MuxAlt3);

    uart_config_t config;

    UART_GetDefaultConfig(&config);
    config.baudRate_Bps = 115200;
    config.enableTx = true;
    config.enableRx = true;

    UART_Init(UART0, &config, 120000000);
    UART_TransferStartRingBuffer(UART0, &uartHandle, buffer, NXP_Uart::bufferLength);
    UART_TransferCreateHandle(UART0, &uartHandle, UART_UserCallback, NULL);
}

void NXP_Uart::proc() {
    // todo implement when read method will be ready
}


void NXP_Uart::write(void const* data) {
    static uart_transfer_t dataTransfer;
    dataTransfer.data = (uint8_t*)data;
    dataTransfer.dataSize = strlen((char*)data);
    UART_TransferSendNonBlocking(UART0, &uartHandle, &dataTransfer);
}

char NXP_Uart::read() {
    // todo implement a read method
    return '0';
}