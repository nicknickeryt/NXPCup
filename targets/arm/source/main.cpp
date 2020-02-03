/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 * NXP Main file
 *
 */

#include <drivers/fsl_uart.h>
#include <drivers/fsl_uart_edma.h>
#include "HALina.hpp"
#include "NXP_Kitty.hpp"
#include "clock_config.h"

#define LOG_CHANNEL APP
#define APP_LOG_CHANNEL 1
#define APP_LOG_CHANNEL_LEVEL LOG_LEVEL_DEBUG

//#include "logger.h"


#include "fsl_dmamux.h"

using namespace halina;

#include "NXP_DMA.h"

//uint8_t witek [] = {'w', 'i', 't', 'e', 'k'};


int main(){
    BOARD_BootClockRUN();
    Kitty& kitty = Kitty::kitty();
    kitty.init();

//    NXP_DMA uartDMA = NXP_DMA(kDmaRequestMux0UART0Tx);
//
//    NXP_PORT uart0RXmux = {PORTA, 14U, 0x03};
//    NXP_PORT uart0TXmux = {PORTA, 15U, 0x03};
//    NXP_Uart uartCommunication = {UART0, 115200, uart0RXmux, uart0TXmux, uartDMA};
//    uartCommunication.init();
//
//    uartCommunication.initDMA();
//    uartCommunication.appendDMA(witek, sizeof(witek));
//    uartCommunication.appendDMA(witek, sizeof(witek));

//    uartCommunication.write('d');


    while (true){
        kitty.proc();
    }
}



