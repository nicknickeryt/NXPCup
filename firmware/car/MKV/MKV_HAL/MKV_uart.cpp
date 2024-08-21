/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 * MKV HALina implementation of uart
 *
 */

#include "HALina.hpp"
#include "MKV_uart.hpp"
#include "fsl_clock.h"

using namespace halina;

uint8_t dmaInitBuffer[] = {'A', 'B', 'C', 'D', 'E'};
MKV_Uart* MKVUartHandlers[6];

void MKV_Uart::appendDMA(uint8_t* dataPointer, uint32_t dataSize) {
    if (DMAenable) {
        dmaData.append(DMAData<uint8_t>(dataPointer, dataSize));
        if (!DMAworking && RingBuffer_IsEmpty(&txRingBuffer)) {
            sendDma();
        }
    }
}

void MKV_Uart::DMAcallback(uint32_t* args) {
    auto handler = (MKV_Uart*)args;
    handler->DMAworking = false;
    handler->uart->C5 &= ~ UART_C5_TDMAS_MASK; // disable DMA in UART
    handler->enableInterrupt(InterruptType::TX_EMPTY); // enable DMA in UART
}

bool MKV_Uart::sendDma() {
    if (DMAenable) {
        DMAworking = true;
        uart->C5 |= UART_C5_TDMAS_MASK; // enable DMA in UART
        enableInterrupt(InterruptType::TX_EMPTY); // enable DMA in UART
        DMAData lastData = dmaData.get();
        dmaTX.setSourceAddress((uint32_t)lastData.dataPointer, lastData.dataSize);
        dmaTX.enableRequest();
        return true;
    }
    return false;
}

void MKV_Uart::initDMA() {
    DMAenable = true;

    dmaTX.init(DMAcallback, (uint32_t*)this);
    dmaTX.setDestinationAddress((uint32_t)(&uart->D));
    dmaTX.setSourceAddress((uint32_t)dmaInitBuffer, 0);
    dmaTX.setInitialValues();
}

MKV_Uart::MKV_Uart(UART_Type* uart, uint32_t baudrate, MKV_PORT& rxPin, MKV_PORT& txPin, MKV_DMA& dmaTX) :
    uart(uart), baudrate(baudrate), rxPin(rxPin), txPin(txPin), dmaTX(dmaTX), redirectHandler(nullptr) {
    if(UART0 == uart){
        MKVUartHandlers[0] = this;
    } else if(UART1 == uart){
        MKVUartHandlers[1] = this;
    } else if(UART2 == uart){
        MKVUartHandlers[2] = this;
    } else if(UART3 == uart){
        MKVUartHandlers[3] = this;
    } else if(UART4 == uart){
        MKVUartHandlers[4] = this;
    } else if(UART5 == uart){
        MKVUartHandlers[5] = this;
    }
    RingBuffer_Init(&rxRingBuffer, rxBuffer, rxBufferSize);
    RingBuffer_Init(&txRingBuffer, txBuffer, txBufferSize);
}

void MKV_Uart::init(){
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
    if (baudDiff > (baudrate - (clock / (16 * (sbr + 1))))) {
        sbr++;
    }

    uart->BDH = (uart->BDH & ~UART_BDH_SBR_MASK) | (uint8_t)(sbr >> 8);
    uart->BDL = (uint8_t)sbr;

    uart->C2 |= UART_C2_TE_MASK | UART_C2_RE_MASK; //Transmitter on, Receiver on
    uart->C1 = 0;
    uart->C4 |= 9U;

    //FIFO on
    uart->PFIFO = 0xAA;

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

void MKV_Uart::proc() {
    // todo implement when read method will be ready
}

void MKV_Uart::write(void const* data, uint16_t length){
    // enter critical section
    __disable_irq();
    // put data to ring buffer
    for(uint16_t i=0; i<length; i++){
        RingBuffer_PutChar(&txRingBuffer, *(reinterpret_cast<char*>(const_cast<void*>(data)) + i));
    }
    // exit critical section
    __enable_irq();
    if (!DMAworking) {
        // enable TX empty interrupt
        enableInterrupt(InterruptType::TX_EMPTY);
    }

}

void MKV_Uart::write(uint8_t data) {
    // enter critical section
    __disable_irq();
    // put data to ring buffer
    RingBuffer_PutChar(&txRingBuffer, data);
    // exit critical section
    __enable_irq();
    if (!DMAworking) {
        // enable TX empty interrupt
        enableInterrupt(InterruptType::TX_EMPTY);
    }
}

uint8_t MKV_Uart::read() {
    static uint8_t c;
    RingBuffer_GetChar(&rxRingBuffer, &c);
    return c;
}

void MKV_Uart::read(uint8_t* buffer, uint16_t length){
    if(buffer != nullptr){
        if(length <= RingBuffer_GetLen(&rxRingBuffer)){
            for(auto i=0; i<length; i++){
                RingBuffer_GetChar(&rxRingBuffer, &buffer[i]);
            }
        }
    }
}

bool MKV_Uart::isBufferEmpty(){
    return RingBuffer_IsEmpty(&rxRingBuffer);
}

uint16_t MKV_Uart::getBufferLevel(){
    return RingBuffer_GetLen(&rxRingBuffer);
}

void MKV_Uart::flushRxBuffer(){
    RingBuffer_Clear(&rxRingBuffer);
}

void MKV_Uart::enableInterrupt(InterruptType interrupt){
    uart->C2 |= static_cast<uint8_t >(interrupt);
}

void MKV_Uart::disableInterrupt(InterruptType interrupt){
    uart->C2 &= ~(static_cast<uint8_t >(interrupt));
}

void UART_IRQ(MKV_Uart* MKVUartHandler) {
    if(MKVUartHandler->uart->S1 & UART_S1_TDRE_MASK){
        MKVUartHandler->uart->S1 |= UART_S1_TDRE_MASK;
        uint8_t c;
        if (RingBuffer_GetChar(&(MKVUartHandler->txRingBuffer), &c)) {
            MKVUartHandler->uart->D = c;
        } else {
            if (MKVUartHandler->dmaData.isEmpty()) {
                MKVUartHandler->disableInterrupt(MKV_Uart::InterruptType::TX_EMPTY);
            } else {
//                __disable_irq();
                MKVUartHandler->sendDma();
//                __enable_irq();
            }
        }
    }
    if (MKVUartHandler->uart->S1 & UART_S1_RDRF_MASK) {
        MKVUartHandler->uart->S1 |= UART_S1_RDRF_MASK;
        if (MKVUartHandler->redirectHandler) {
            MKVUartHandler->redirectHandler(MKVUartHandler->uart->D);
        } else {
            RingBuffer_PutChar(&(MKVUartHandler->rxRingBuffer), MKVUartHandler->uart->D);
        }
    }
}

extern "C" {
void UART0_RX_TX_IRQHandler() {
    UART_IRQ(MKVUartHandlers[0]);
}

void UART1_RX_TX_IRQHandler() {
    UART_IRQ(MKVUartHandlers[1]);
}

void UART2_RX_TX_IRQHandler() {
    UART_IRQ(MKVUartHandlers[2]);
}

void UART3_RX_TX_IRQHandler() {
    UART_IRQ(MKVUartHandlers[3]);
}

void UART4_RX_TX_IRQHandler() {
    UART_IRQ(MKVUartHandlers[4]);
}

void UART5_RX_TX_IRQHandler() {
    UART_IRQ(MKVUartHandlers[5]);
}
}