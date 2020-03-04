/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 * NXP frame
 *
 */

#pragma once
#include "NXP_uart.hpp"

struct NXP_Frame{
    constexpr static auto startFlagLength = 4;
private:
    NXP_Uart &uart;
    uint8_t dataLength;
    uint8_t startFlag[startFlagLength] = {255, 255, 255, 255};
    uint8_t dataBuffer[256];

public:
    NXP_Frame(NXP_Uart &uart, uint8_t dataLength) : uart(uart), dataLength(dataLength){

    }

    void send(uint8_t* data){
        memcpy(dataBuffer, startFlag, startFlagLength);
        memcpy(dataBuffer + startFlagLength, data, dataLength);
        uart.write(dataBuffer, dataLength + startFlagLength);
    }

};