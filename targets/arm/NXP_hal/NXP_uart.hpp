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

void loggerWriteChar(char c);

class NXP_Uart : public halina::UART{
private:
    constexpr static auto txBufferSize=1024;
    uint32_t baudrate;

    enum class InterruptType{
        TX_EMPTY,
        TX_COMPLETE,
        RX_FULL
    };

public:
    UART_Type* uart;
    uint8_t txBuffer[txBufferSize] = {0};
    RingBuffer txRingBuffer = {nullptr, 0, 0, nullptr, nullptr};

private:
    void enableInterrupt(InterruptType interrupt);

    void disableInterrupt(InterruptType interrupt);

public:
    NXP_Uart(UART_Type* uart, uint32_t baudrate);

    void init() override;

    void write(void const* data, uint16_t length) override;

    char read() override;

   static  void proc();
};