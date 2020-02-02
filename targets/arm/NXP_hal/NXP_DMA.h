#pragma once

#include <cstdint>
#include "MKV58F24.h"
#include "HALinia_DMA.h"

class NXP_DMA : HALina_DMA {
    int8_t channel = 0;
    dma_request_source_t source;
    bool initialised = false;
public:
    uint32_t* args;
    void (*callbackHandler)(uint32_t* args) = nullptr;

    NXP_DMA(dma_request_source_t source);

    inline bool getStatus() {
        return initialised;
    }
    void init(void (*callbackHandler)(uint32_t* args), uint32_t* args) ;
    void enableRequest();
    void setInitialValues();
    void setDestinationAddress(uint32_t address);
    void setSourceAddress(uint32_t address, uint32_t bytesCount);
    static NXP_DMA& emptyDMA();
};
