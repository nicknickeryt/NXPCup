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
#include <type_traits>
#include "command_terminal/cyclicBuffer.hpp"
#include "NXP_DMA.h"


class NXP_Uart : public halina::UART {
public:
    template <typename T>
    class DMAData {
    public:
        T* dataPointer;
        uint32_t dataSize;
        uint8_t dataTypeInRegister ;

        DMAData (T* dataPointer, uint32_t dataSize) : dataPointer(dataPointer), dataSize(dataSize) {
            if constexpr (std::is_same<T, uint8_t>::value) {
                dataTypeInRegister = 0b000u;
            } else if constexpr (std::is_same<T, uint16_t>::value) {
                dataTypeInRegister = 0b001u;
            } else if constexpr (std::is_same<T, uint32_t>::value) {
                dataTypeInRegister = 0b010u;
            }
        }

        DMAData() : dataPointer(nullptr), dataSize(0), dataTypeInRegister(0) { }
    };
    UART_Type* uart;
    RingBuffer rxRingBuffer;
    RingBuffer txRingBuffer;


private:
    uint32_t baudrate;
    NXP_PORT& rxPin;
    NXP_PORT& txPin;
    NXP_DMA& dmaTX;

    CyclicBuffer_data<DMAData<uint8_t>, 20> dmaData;

    bool DMAenable = false;
    bool DMAworking = false;

    constexpr static auto txBufferSize = 1024;
    constexpr static auto rxBufferSize = 1024;

    uint8_t txBuffer[txBufferSize] = {0};
    uint8_t rxBuffer[rxBufferSize] = {0};
public:
    enum class InterruptType : uint8_t {
        TX_EMPTY = UART_C2_TIE_MASK,
        TX_COMPLETE = UART_C2_TCIE_MASK,
        RX_FULL = UART_C2_RIE_MASK
    };

public:
    void enableInterrupt(InterruptType interrupt);

    void disableInterrupt(InterruptType interrupt);

    NXP_Uart(UART_Type* uart, uint32_t baudrate, NXP_PORT& rxPin, NXP_PORT& txPin, NXP_DMA& dmaTX);
//    NXP_Uart(UART_Type* uart, uint32_t baudrate, NXP_PORT& rxPin, NXP_PORT& txPin);

    void init() override;

    static void DMAcallback(uint32_t* args) {
        auto handler = (NXP_Uart*)args;
        handler->DMAworking = false;
    }

    void DMAinit() {
        DMAenable = true;

        // todo to nie ma być tutaj
        enableInterrupt(InterruptType::TX_EMPTY);
        uart->C5 |= UART_C5_TDMAS_MASK; // enable DMA in UART
        // todo end

        dmaTX.init(DMAcallback, (uint32_t*)this);
    }

    void appendDMA(uint8_t* dataPointer, uint32_t dataSize) {
        if (DMAenable) {
            dmaData.append(DMAData<uint8_t>(dataPointer, dataSize));
        }
    }

    bool sendDma() {
        DMAworking = true;
        uart->C5 |= UART_C5_TDMAS_MASK; // enable DMA in UART
        enableInterrupt(InterruptType::TX_EMPTY); // enable DMA in UART
        DMAData lastData = dmaData.get();
        DMA0->TCD[0].SADDR = (uint32_t)lastData.dataPointer; // defines source data address
        DMA0->TCD[0].CITER_ELINKNO = lastData.dataSize;
        DMA0->TCD[0].BITER_ELINKNO = lastData.dataSize;
        DMA0->SERQ = DMA_SERQ_SERQ(0);

        return true;
    }

    void write(void const* data, uint16_t length) override;
    void write(uint8_t data) override;
    uint8_t read() override;
    static void proc();
};