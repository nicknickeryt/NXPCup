/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 * NXP HALina implementation of uart
 *
 */

#pragma once

#include "HALina.hpp"
#include "ring_buffer.h"
#include "NXP_gpio.hpp"

class NXP_Uart : public halina::UART {

public:
    UART_Type* uart;
private:
    uint32_t baudrate;
    NXP_PORT& rxPin;
    NXP_PORT& txPin;

    constexpr static auto txBufferSize = 1024;
    constexpr static auto rxBufferSize = 1024;

    uint8_t txBuffer[txBufferSize] = {0};
    uint8_t rxBuffer[rxBufferSize] = {0};

    RingBuffer* myTxRingBuffer = nullptr;
    RingBuffer* myRxRingBuffer = nullptr;
public:
    enum class InterruptType : uint8_t {
        TX_EMPTY = UART_C2_TIE_MASK,
        TX_COMPLETE = UART_C2_TCIE_MASK,
        RX_FULL = UART_C2_RIE_MASK
    };

public:
    void enableInterrupt(InterruptType interrupt);

    void disableInterrupt(InterruptType interrupt);

    void setMyBuffers(RingBuffer* rxRingBuffer, RingBuffer* txRingBuffer) {
        myRxRingBuffer = rxRingBuffer;
        myTxRingBuffer = txRingBuffer;
        RingBuffer_Init(rxRingBuffer, rxBuffer, rxBufferSize);
        RingBuffer_Init(txRingBuffer, txBuffer, txBufferSize);
    }

    NXP_Uart(UART_Type* uart, uint32_t baudrate, NXP_PORT& rxPin, NXP_PORT& txPin);

    void init() override;
    void write(void const* data, uint16_t length) override;
    void write(uint8_t data) override;
    uint8_t read() override;
    static void proc();
};