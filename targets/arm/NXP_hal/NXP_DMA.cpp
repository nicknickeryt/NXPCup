#include <device/MKV58F24_features.h>
#include "NXP_DMA.h"

NXP_DMA* dmaHandlers[32];
uint8_t NXP_DMA::DMAIndexChannel = 0;

static const IRQn_Type DMA_IRQn[] = {DMA0_DMA16_IRQn, DMA1_DMA17_IRQn, DMA2_DMA18_IRQn, DMA3_DMA19_IRQn, DMA4_DMA20_IRQn, DMA5_DMA21_IRQn, DMA6_DMA22_IRQn, DMA7_DMA23_IRQn, DMA8_DMA24_IRQn, DMA9_DMA25_IRQn, DMA10_DMA26_IRQn, DMA11_DMA27_IRQn, DMA12_DMA28_IRQn, DMA13_DMA29_IRQn, DMA14_DMA30_IRQn, DMA15_DMA31_IRQn};



NXP_DMA::NXP_DMA(int channel, dma_request_source_t source) {
    if (channel == -1) return;
    dmaHandlers[channel] = this;
    this->channel = channel;
    this->callbackHandler = callbackHandler;
    this->source = source;
    initialised = true;
}

void NXP_DMA::init(void (*callbackHandler)(uint32_t* args), uint32_t* args) {
    if (!initialised) return;

    this->args = args;
    this->callbackHandler = callbackHandler;

    SIM->SCGC6 |= SIM_SCGC6_DMAMUX_MASK; //enables DMA McUX clock gate
    SIM->SCGC7 |= SIM_SCGC7_DMA_MASK;//enables DMA clock gate, default value is enable

//    TCD = &DMA0->TCD[channel];

//    DMA0->ERQ |= (1u << channel);//enables p request
//    DMA0->CR = 0;
//    DMA0->ES = 0;
//    DMA0->ERQ = 0;
//    DMA0->EEI = 0;
//    DMA0->INT = 0;
//    DMA0->EARS = 0;
//    DMA0->ERR = 0xFFFF;

    DMAMUX0->CHCFG[channel] = 0; //clears register for changing source and trigger
    DMAMUX0->CHCFG[channel] = DMAMUX_CHCFG_ENBL_MASK | DMAMUX_CHCFG_SOURCE(source);

    if (channel <= 15) {
        NVIC_ClearPendingIRQ(DMA_IRQn[channel]);
        NVIC_EnableIRQ(DMA_IRQn[channel]);
    } else if (channel <= 31) {
        NVIC_ClearPendingIRQ(DMA_IRQn[channel - 16]);
        NVIC_EnableIRQ(DMA_IRQn[channel - 16]);
    }
}



void callback(uint8_t cal1, uint8_t cal2) {
    if (DMA0->INT & (1u << cal1)) {
        DMA0->INT |= (1u << cal1);
        dmaHandlers[cal1]->callbackHandler(dmaHandlers[cal1]->args);
    } else if (DMA0->INT & (1u << cal2)) {
        DMA0->INT |= (1u << cal2);
        dmaHandlers[cal2]->callbackHandler(dmaHandlers[cal1]->args);
    }
}

extern "C" {
void DMA0_DMA16_IRQHandler(void) { callback(0, 16); }
void DMA1_DMA17_IRQHandler(void) { callback(1, 17); }
void DMA2_DMA18_IRQHandler(void) { callback(2, 18); }
void DMA3_DMA19_IRQHandler(void) { callback(3, 19); }
void DMA4_DMA20_IRQHandler(void) { callback(4, 20); }
void DMA5_DMA21_IRQHandler(void) { callback(5, 21); }
void DMA6_DMA22_IRQHandler(void) { callback(6, 22); }
void DMA7_DMA23_IRQHandler(void) { callback(7, 23); }
void DMA8_DMA24_IRQHandler(void) { callback(8, 24); }
void DMA9_DMA25_IRQHandler(void) { callback(9, 25); }
void DMA10_DMA26_IRQHandler(void) { callback(10, 26); }
void DMA11_DMA27_IRQHandler(void) { callback(11, 27); }
void DMA12_DMA28_IRQHandler(void) { callback(12, 28); }
void DMA13_DMA29_IRQHandler(void) { callback(13, 29); }
void DMA14_DMA30_IRQHandler(void) { callback(14, 30); }
void DMA15_DMA31_IRQHandler(void) { callback(15, 31); }
void DMA_Error_IRQHandler(void) {

}
}
