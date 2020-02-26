/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 *
 *
 */

#pragma once
#include <cstdint>
#include <cstring>

class PixyPacketRequest{

};

class PixyPacketResponse{

};

struct SetLampRequest : public PixyPacketRequest{
    uint16_t sync = 0xc1ae;
    uint8_t type = 22;
    uint8_t payloadLength = 2;
    uint8_t upper = 0;
    uint8_t lower = 0;

    SetLampRequest(uint8_t upper, uint8_t lower) : upper(upper), lower(lower){}

    void serialize(uint8_t *data, uint8_t* length){
        memcpy(data, this, sizeof(*this));
        *length = sizeof(*this);
    }
};

