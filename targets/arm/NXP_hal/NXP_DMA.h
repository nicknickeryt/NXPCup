#pragma once

#include <cstdint>
#include "MKV58F24.h"

class NXP_DMA {
    static uint8_t DMAIndexChannel;
    constexpr static uint8_t DMAIndexChannelMax = 31;
    decltype(&DMA0->TCD[0]) TCD = nullptr;
    uint8_t channel;
    dma_request_source_t source;
    bool initialised = false;

public:
    uint32_t* args;
    void (*callbackHandler)(uint32_t* args) = nullptr;
    static int getDMAChannel() {
        if (DMAIndexChannel < DMAIndexChannelMax) {
            DMAIndexChannel++;
            return DMAIndexChannel;
        } else {
            return -1;
        }
    }

    NXP_DMA(int channel, dma_request_source_t source);

    void init(void (*callbackHandler)(uint32_t* args), uint32_t* args) ;
};
