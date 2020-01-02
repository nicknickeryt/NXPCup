#include "NXP_hal.hpp"

using namespace halina;

uint8_t g_txBuffer[NXP_Uart::echoBufferLength] = {0};
uint8_t g_rxBuffer[NXP_Uart::echoBufferLength] = {0};
volatile bool rxBufferEmpty = true;
volatile bool txBufferFull = false;
volatile bool txOnGoing = false;
volatile bool rxOnGoing = false;
uart_handle_t g_uartHandle;

/* UART user callback */
void UART_UserCallback(UART_Type *base, uart_handle_t *handle, status_t status, void *userData)
{
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

void NXP_Uart::init(){
    PORT_SetPinMux(PORTA, 14U, kPORT_MuxAlt3);
    PORT_SetPinMux(PORTA, 15U, kPORT_MuxAlt3);

    UART_GetDefaultConfig(&config);
    config.baudRate_Bps = 115200;
    config.enableTx = true;
    config.enableRx = true;

    UART_Init(UART0, &config, 120000000);
    UART_TransferCreateHandle(UART0, &g_uartHandle, UART_UserCallback, NULL);

    /* Start to echo. */
    sendXfer.data = g_txBuffer;
    sendXfer.dataSize = echoBufferLength;
    receiveXfer.data = g_rxBuffer;
    receiveXfer.dataSize = echoBufferLength;
}

void NXP_Uart::proc() {
    /* If RX is idle and g_rxBuffer is empty, start to read data to g_rxBuffer. */
    if ((!rxOnGoing) && rxBufferEmpty)
    {
        rxOnGoing = true;
        UART_TransferReceiveNonBlocking(UART0, &g_uartHandle, &receiveXfer, NULL);
    }

/* If TX is idle and g_txBuffer is full, start to send data. */
    if ((!txOnGoing) && txBufferFull)
    {
        txOnGoing = true;
        UART_TransferSendNonBlocking(UART0, &g_uartHandle, &sendXfer);
    }

/* If g_txBuffer is empty and g_rxBuffer is full, copy g_rxBuffer to g_txBuffer. */
    if ((!rxBufferEmpty) && (!txBufferFull))
    {
        memcpy(g_txBuffer, g_rxBuffer, echoBufferLength);
        rxBufferEmpty = true;
        txBufferFull = true;
    }
}


void NXP_Uart::write(char) {

}

char NXP_Uart::read() {

    return '0';
}