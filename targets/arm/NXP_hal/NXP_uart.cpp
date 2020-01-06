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

NXP_Uart* nxpUart0Handler;
NXP_Uart* nxpUart2Handler;

NXP_Uart::NXP_Uart(UART_Type* uart, uint32_t baudrate) : baudrate(baudrate), uart(uart){
    if(UART0 == uart){
        nxpUart0Handler = this;
    } else if(UART2 == uart){
        nxpUart2Handler = this;
    }

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

    // initialize buffer
    RingBuffer_Init(&txRingBuffer, txBuffer, txBufferSize);

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

    uart->C4 |= 9;
    uart->PFIFO = 0xAA;

    if(UART0 == uart){
        NVIC_ClearPendingIRQ(UART0_RX_TX_IRQn);
        NVIC_EnableIRQ(UART0_RX_TX_IRQn);
    } else if(UART1 == uart){
        NVIC_ClearPendingIRQ(UART1_RX_TX_IRQn);
        NVIC_EnableIRQ(UART1_RX_TX_IRQn);
    } else if(UART2 == uart){
        NVIC_ClearPendingIRQ(UART2_RX_TX_IRQn);
        NVIC_EnableIRQ(UART2_RX_TX_IRQn);
    } else if(UART3 == uart){
        NVIC_ClearPendingIRQ(UART3_RX_TX_IRQn);
        NVIC_EnableIRQ(UART3_RX_TX_IRQn);
    } else if(UART4 == uart){
        NVIC_ClearPendingIRQ(UART4_RX_TX_IRQn);
        NVIC_EnableIRQ(UART4_RX_TX_IRQn);
    }
}

void NXP_Uart::proc() {
    // todo implement when read method will be ready
}

void NXP_Uart::write(void const* data, uint16_t length){
    // enter critical section
    __disable_irq();
    // put data to ring buffer
    for(uint16_t i=0; i<length; i++){
        RingBuffer_PutChar(&txRingBuffer, *(reinterpret_cast<char*>(const_cast<void*>(data)) + i));
    }
    // exit critical section
    __enable_irq();
    // enable TX empty interrupt
    enableInterrupt(InterruptType::TX_EMPTY);
}

char NXP_Uart::read() {
    // todo implement a read method
    return '0';
}

void NXP_Uart::enableInterrupt(InterruptType interrupt){
    switch(interrupt){
        case InterruptType::TX_EMPTY:
            uart->C2 |= UART_C2_TIE_MASK;
            break;
        case InterruptType::TX_COMPLETE:
            uart->C2 |= UART_C2_TCIE_MASK;
            break;
        case InterruptType::RX_FULL:
            uart->C2 |= UART_C2_RIE_MASK;
            break;
        default:
            break;
    }

}

void NXP_Uart::disableInterrupt(InterruptType interrupt){
    switch(interrupt){
        case InterruptType::TX_EMPTY:
            uart->C2 &= ~(UART_C2_TIE_MASK);
            break;
        case InterruptType::TX_COMPLETE:
            uart->C2 &= ~(UART_C2_TCIE_MASK);
            break;
        case InterruptType::RX_FULL:
            uart->C2 &= ~(UART_C2_RIE_MASK);
            break;
        default:
            break;
    }
}

void loggerWriteChar(const char c) {
    // enter critical section
    __disable_irq();
    // put data to ring buffer
    RingBuffer_PutChar(&(nxpUart0Handler->txRingBuffer), (uint8_t)c);
    // exit critical section
    __enable_irq();
    // enable TX empty interrupt
    nxpUart0Handler->uart->C2 |= UART_C2_TIE_MASK;
}

extern "C" {
void UART0_RX_TX_IRQHandler() {
    if(UART0->S1 & UART_S1_TDRE_MASK){
        UART0->S1 |= UART_S1_TDRE_MASK;
        uint8_t c;
        if (RingBuffer_GetChar(&(nxpUart0Handler->txRingBuffer), &c)) {
            nxpUart0Handler->uart->D = c;
        } else {
            nxpUart0Handler->uart->C2 &= ~(UART_C2_TIE_MASK);
        }
    }
}

void UART2_RX_TX_IRQHandler() {
    if(UART2->S1 & UART_S1_TDRE_MASK){
        UART2->S1 |= UART_S1_TDRE_MASK;
        uint8_t c;
        if (RingBuffer_GetChar(&(nxpUart2Handler->txRingBuffer), &c)) {
            nxpUart2Handler->uart->D = c;
        } else {
            nxpUart2Handler->uart->C2 &= ~(UART_C2_TIE_MASK);
        }
    }
}
}