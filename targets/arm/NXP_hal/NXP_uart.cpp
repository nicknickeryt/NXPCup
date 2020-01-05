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
#include "clock_config.h"

using namespace halina;

NXP_Uart* nxpUartHandler;

NXP_Uart::NXP_Uart(UART_Type* uart, uint32_t baudrate) : baudrate(baudrate), uart(uart) {
    nxpUartHandler = this;
}

void NXP_Uart::init(){
    uint16_t sbr = 0;
    uint32_t baudDiff = 0;
    uint32_t clock = 120000000;

    if(uart == UART0){
        SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;
    } else if(uart == UART1){
        SIM->SCGC4 |= SIM_SCGC4_UART1_MASK;
    } else if(uart == UART2){
        SIM->SCGC4 |= SIM_SCGC4_UART2_MASK;
    } else if(uart == UART3){
        SIM->SCGC4 |= SIM_SCGC4_UART3_MASK;
    }

    sbr = clock / (baudrate * 16);
    if (sbr == 0){
        sbr = 1;
    }
    baudDiff = (clock / (sbr * 16)) - baudrate;
    uint16_t brfa = (2 * clock / (baudrate)) - 32 * sbr;
    if (baudDiff > (baudrate - (clock / (16 * (sbr + 1))))) {
        sbr++;
    }

    uart->C2 &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK);
    uart->BDH = (uart->BDH & ~UART_BDH_SBR_MASK) | (uint8_t)(sbr >> 8);
    uart->BDL = (uint8_t)sbr;
    uart->C4 = (uart->C4 & ~UART_C4_BRFA_MASK) | (brfa & UART_C4_BRFA_MASK);
    uart->C2 |= UART_C2_TE_MASK | UART_C2_RE_MASK;
    UART0->C4 |= 9;
    UART0->PFIFO = 0xAA;

    //enableInterupts();
}

void NXP_Uart::proc() {
    // todo implement when read method will be ready
}

void NXP_Uart::writeChar(const char c) {
    (void)c;
    while (!(nxpUartHandler->uart->S1 & UART_S1_TDRE_MASK))
    {
    }
    nxpUartHandler->uart->D = c;
}

char NXP_Uart::read() {
    // todo implement a read method
    return '0';
}

void NXP_Uart::enableInterupts(){
    uart->C2 |= UART_C2_RIE_MASK | UART_C2_TIE_MASK;
    NVIC_ClearPendingIRQ(UART0_RX_TX_IRQn);
    NVIC_EnableIRQ(UART0_RX_TX_IRQn);
}

void NXP_Uart::disableInterupts(){
    uart->C2 &= ~(UART_C2_RIE_MASK | UART_C2_TIE_MASK);
}

extern "C" {
void UART0_RX_TX_DriverIRQHandler() {
    if(UART0->S1 & UART_S1_RDRF_MASK){
        UART0->S1 |= UART_S1_RDRF_MASK;
    }
    if(UART0->S1 & UART_S1_TC_MASK){
        UART0->S1 |= UART_S1_TC_MASK;
    }
}
}