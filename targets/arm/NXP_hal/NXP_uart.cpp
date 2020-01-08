/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 * NXP HALina implementation of uart
 *
 */

#include "HALina.hpp"
#include "NXP_uart.hpp"
#include <drivers/fsl_clock.h>
#include "NXP_Kitty.hpp"

using namespace halina;

RingBuffer rxRingBuffer[6];
RingBuffer txRingBuffer[6];

NXP_Uart::NXP_Uart(UART_Type* uart, uint32_t baudrate, NXP_PORT& rxPin, NXP_PORT& txPin) : uart(uart), baudrate(baudrate), rxPin(rxPin), txPin(txPin) {
    if(UART0 == uart){
        setMyBuffers(&rxRingBuffer[0], &txRingBuffer[0]);
    } else if(UART1 == uart){
        setMyBuffers(&rxRingBuffer[1], &txRingBuffer[1]);
    } else if(UART2 == uart){
        setMyBuffers(&rxRingBuffer[2], &txRingBuffer[2]);
    } else if(UART3 == uart){
        setMyBuffers(&rxRingBuffer[3], &txRingBuffer[3]);
    } else if(UART4 == uart){
        setMyBuffers(&rxRingBuffer[4], &txRingBuffer[4]);
    } else if(UART5 == uart){
        setMyBuffers(&rxRingBuffer[5], &txRingBuffer[5]);
    }
}

void NXP_Uart::init(){
    uint16_t sbr = 0;
    uint32_t baudDiff = 0;

    uint32_t clock =  CLOCK_GetFreq(kCLOCK_FastPeriphClk);

    if(uart == UART0){
        SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;
    } else if(uart == UART1){
        SIM->SCGC4 |= SIM_SCGC4_UART1_MASK;
    } else if(uart == UART2){
        SIM->SCGC4 |= SIM_SCGC4_UART2_MASK;
    } else if(uart == UART3){
        SIM->SCGC4 |= SIM_SCGC4_UART3_MASK;
    } else if(uart == UART4){
        SIM->SCGC1 |= SIM_SCGC1_UART4_MASK;
    } else if(uart == UART5){
        SIM->SCGC1 |= SIM_SCGC1_UART5_MASK;
    } else {
        return;
    }

    // set mux as UART
    rxPin.setMux();
    txPin.setMux();

    sbr = clock / (baudrate * 16);
    if (sbr == 0){
        sbr = 1;
    }
    baudDiff = (clock / (sbr * 16)) - baudrate;
    uint16_t brfa = (2 * clock / (baudrate)) - 32 * sbr;
    if (baudDiff > (baudrate - (clock / (16 * (sbr + 1))))) {
        sbr++;
    }

    uart->BDH = (uart->BDH & ~UART_BDH_SBR_MASK) | (uint8_t)(sbr >> 8);
    uart->BDL = (uint8_t)sbr;

    uart->C4 = (uart->C4 & ~UART_C4_BRFA_MASK) | (brfa & UART_C4_BRFA_MASK);
    uart->C2 |= UART_C2_TE_MASK | UART_C2_RE_MASK; //Transmitter on, Receiver on

    uart->C4 |= 9U;

    enableInterrupt(InterruptType::RX_FULL);

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
    } else if(UART5 == uart){
        NVIC_ClearPendingIRQ(UART5_RX_TX_IRQn);
        NVIC_EnableIRQ(UART5_RX_TX_IRQn);
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
        RingBuffer_PutChar(myTxRingBuffer, *(reinterpret_cast<char*>(const_cast<void*>(data)) + i));
    }
    // exit critical section
    __enable_irq();
    // enable TX empty interrupt
    enableInterrupt(InterruptType::TX_EMPTY);
}

void NXP_Uart::write(uint8_t data) {
    RingBuffer_PutChar(myTxRingBuffer, data);
    enableInterrupt(InterruptType::TX_EMPTY);
}

uint8_t NXP_Uart::read() {
    uint8_t c;
    RingBuffer_GetChar(myRxRingBuffer, &c);
    return c;
}

void NXP_Uart::enableInterrupt(InterruptType interrupt){
    uart->C2 |= static_cast<uint8_t >(interrupt);
}

void NXP_Uart::disableInterrupt(InterruptType interrupt){
    uart->C2 &= ~(static_cast<uint8_t >(interrupt));
}

void UART_IRQ(UART_Type* uart, RingBuffer* txRingBufferLocal, RingBuffer* rxRingBufferLocal) {
    if(uart->S1 & UART_S1_TDRE_MASK){
        uart->S1 |= UART_S1_TDRE_MASK;
        uint8_t c;
        if (RingBuffer_GetChar(txRingBufferLocal, &c)) {
            uart->D = c;
        } else {
            uart->C2 &= ~(UART_C2_TIE_MASK);
        }
    } if (uart->S1 & UART_S1_RDRF_MASK) {
        uart->S1 |= UART_S1_RDRF_MASK;
        RingBuffer_PutChar(rxRingBufferLocal, uart->D);
    }
}

extern "C" {
void UART0_RX_TX_IRQHandler() {
    UART_IRQ(UART0, &txRingBuffer[0], &rxRingBuffer[0]);
}

void UART1_RX_TX_IRQHandler() {
    UART_IRQ(UART1, &txRingBuffer[1], &rxRingBuffer[1]);
}

void UART2_RX_TX_IRQHandler() {
    UART_IRQ(UART2, &txRingBuffer[2], &rxRingBuffer[2]);
}

void UART3_RX_TX_IRQHandler() {
    UART_IRQ(UART3, &txRingBuffer[3], &rxRingBuffer[3]);
}

void UART4_RX_TX_IRQHandler() {
    UART_IRQ(UART4, &txRingBuffer[4], &rxRingBuffer[4]);
}

void UART5_RX_TX_IRQHandler() {
    UART_IRQ(UART5, &txRingBuffer[5], &rxRingBuffer[5]);
}
}