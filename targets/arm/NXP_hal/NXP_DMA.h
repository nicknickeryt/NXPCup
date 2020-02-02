#pragma once

#include <cstdint>
#include "MKV58F24.h"

class NXP_DMA {
    static uint8_t DMAIndexChannel;
    constexpr static uint8_t DMAIndexChannelMax = 31;

    uint8_t channel;
    dma_request_source_t source;
    bool initialised = false;

public:
//    decltype(&DMA0->TCD[0]) TCD = nullptr;
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

    void enableRequest() {
        DMA0->ERQ |= (1u << channel);

//        DMA0->SERQ = DMA_SERQ_SERQ(channel);
//        DMA0->SSRT = DMA_SSRT_SSRT(channel);
    }

    void setInitialValues() {

        DMA0->TCD[channel].SOFF = 1;//Source address signed offset

//        DMA0->TCD[channel].CITER_ELINKNO = 0x00;//CITER=1
//        DMA0->TCD[channel].BITER_ELINKNO = 0x00;//BITER=1
        DMA0->TCD[channel].NBYTES_MLNO = 1;//byte number
        DMA0->TCD[channel].DOFF = 0;//destination address signed offset
        DMA0->TCD[channel].ATTR =  0;//8 bit transfer size, register default value is undefined
        DMA0->TCD[channel].SLAST = 0;//restores the source address to the initial value
        DMA0->TCD[channel].DLAST_SGA = 0;//restores the destination address to the initial value
        DMA0->TCD[channel].CSR = DMA_CSR_INTMAJOR_MASK | DMA_CSR_DREQ_MASK | DMA_CSR_START_MASK;
    }

    void setDestinationAddress(uint32_t address) {
        DMA0->TCD[channel].DADDR = address;//defines destination data address
    }

    void setSourceAddress(uint32_t address, uint32_t bytesCount) {
        DMA0->TCD[channel].SADDR = address;//defines source data address
        DMA0->TCD[channel].BITER_ELINKNO = bytesCount;
        DMA0->TCD[channel].CITER_ELINKNO = bytesCount;
    }
};
