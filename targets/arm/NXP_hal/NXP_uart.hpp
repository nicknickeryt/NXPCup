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

    bool DMAenable = false;
    bool DMAworking = false;

    constexpr static auto txBufferSize = 1024;
    constexpr static auto rxBufferSize = 1024;

    uint8_t txBuffer[txBufferSize] = {0};
    uint8_t rxBuffer[rxBufferSize] = {0};
public:
    void (*redirectHandler)(uint8_t);
    CyclicBuffer_data<DMAData<uint8_t>, 20> dmaData;
    enum class InterruptType : uint8_t {
        TX_EMPTY = UART_C2_TIE_MASK,
        TX_COMPLETE = UART_C2_TCIE_MASK,
        RX_FULL = UART_C2_RIE_MASK
    };

    enum class ActionDMA {
        PAUSE,
        RESUME
    };

public:
    void setRedirectHandler( void (*redirectHandler)(uint8_t)) {
        this->redirectHandler = redirectHandler;
    }
    void enableInterrupt(InterruptType interrupt);

    void disableInterrupt(InterruptType interrupt);

    NXP_Uart(UART_Type* uart, uint32_t baudrate, NXP_PORT& rxPin, NXP_PORT& txPin, NXP_DMA& dmaTX);

    void init() override;

    static void DMAcallback(uint32_t* args);

    void initDMA() ;

    void appendDMA(uint8_t* dataPointer, uint32_t dataSize);

    bool sendDma();

    void actionDMA(ActionDMA action) {
        if (action == ActionDMA::RESUME) {
            if (dmaTX.getStatus()) {
                DMAenable = true;
            }
        } else if (action == ActionDMA::PAUSE) {
            DMAenable = false;
        }
    }

    void write(void const* data, uint16_t length) override;
    void write(uint8_t data) override;
    uint8_t read() override;
    static void proc();
};