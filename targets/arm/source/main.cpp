#include <stdio.h>
#include <kitty.h>
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

#include "NXP_hal.h"
#include "LEDLine.h"
/*
 * @brief   Application entry point.
 */
#define ECHO_BUFFER_LENGTH 1

/* UART user callback */
void UART_UserCallback(UART_Type *base, uart_handle_t *handle, status_t status, void *userData);

NXP_GPIO LED0(PORTA, GPIOA, 16U);
NXP_GPIO LED1(PORTA, GPIOA, 17U);
NXP_GPIO LED2(PORTA, GPIOA, 24U);
NXP_GPIO LED3(PORTA, GPIOA, 25U);
NXP_GPIO LED4(PORTA, GPIOA, 26U);
NXP_GPIO LED5(PORTA, GPIOA, 27U);
NXP_GPIO LED6(PORTA, GPIOA, 28U);
NXP_GPIO LED7(PORTA, GPIOA, 29U);

KITTY& kitty() {
    static LEDLine LED_line(LED0, LED1, LED2, LED3, LED4, LED5, LED6, LED7);

    static KITTY static_kitty(LED_line);

    return static_kitty;
}

/*******************************************************************************
 * Variables
 ******************************************************************************/
uart_handle_t g_uartHandle;

uint8_t g_tipString[] =
        "Uart interrupt example\r\nBoard receives 8 characters then sends them out\r\nNow please input:\r\n";

uint8_t g_txBuffer[ECHO_BUFFER_LENGTH] = {0};
uint8_t g_rxBuffer[ECHO_BUFFER_LENGTH] = {0};
volatile bool rxBufferEmpty = true;
volatile bool txBufferFull = false;
volatile bool txOnGoing = false;
volatile bool rxOnGoing = false;

/*******************************************************************************
 * Code
 ******************************************************************************/
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

/*!
 * @brief Main function
 */
int main(void)
{
    uart_config_t config;
    uart_transfer_t xfer;
    uart_transfer_t sendXfer;
    uart_transfer_t receiveXfer;

    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();

    kitty().LED_line.init();

    PORT_SetPinMux(PORTA, 14U, kPORT_MuxAlt3);
    PORT_SetPinMux(PORTA, 15U, kPORT_MuxAlt3);

    UART_GetDefaultConfig(&config);
    config.baudRate_Bps = 115200;
    config.enableTx = true;
    config.enableRx = true;

    UART_Init(UART0, &config, 120000000);
    UART_TransferCreateHandle(UART0, &g_uartHandle, UART_UserCallback, NULL);

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

    uint32_t licznik = 0;
    int led_index = 0;
    uint8_t direction = 0;
    uint8_t old_led = 0;
    while (true)
    {
        licznik++;
        if (licznik == 90000) {
            licznik = 0;
            if (direction == 0) {
                old_led = led_index;
                led_index++;
                if (led_index == 8) {
                    led_index = 6;
                    direction = 1;
                    old_led = 7;
                }
                kitty().LED_line.at(led_index).set();
                kitty().LED_line.at(old_led).reset();
            } else if (direction == 1) {
                old_led = led_index;
                led_index--;
                if (led_index == -1) {
                    led_index = 1;
                    direction = 0;
                    old_led = 0;
                }

                kitty().LED_line.at(led_index).set();
                kitty().LED_line.at(old_led).reset();
            }
        }
        
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
            memcpy(g_txBuffer, g_rxBuffer, ECHO_BUFFER_LENGTH);
            rxBufferEmpty = true;
            txBufferFull = true;
        }
    }
}