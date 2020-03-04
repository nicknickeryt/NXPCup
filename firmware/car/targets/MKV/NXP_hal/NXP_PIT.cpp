#include "NXP_PIT.hpp"

NXP_PIT* pits[4];

extern "C" {
void PIT0_IRQHandler(void) {
    PIT->CHANNEL[0].TFLG = PIT_TFLG_TIF_MASK;
    pits[0]->runCallback();
}

void PIT1_IRQHandler(void) {
    PIT->CHANNEL[1].TFLG = PIT_TFLG_TIF_MASK;
    pits[1]->runCallback();
}

void PIT2_IRQHandler(void) {
    PIT->CHANNEL[2].TFLG = PIT_TFLG_TIF_MASK;
    pits[2]->runCallback();
}

void PIT3_IRQHandler(void) {
    PIT->CHANNEL[3].TFLG = PIT_TFLG_TIF_MASK;
    pits[3]->runCallback();
}
}

void NXP_PIT::enable() {
    SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;       // Enable clock to the PIT
    PIT->MCR = 0x00U;                       // turn on PIT
}

void NXP_PIT::disable() {
    SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;       // Enable clock to the PIT
    PIT->MCR |= PIT_MCR_MDIS_MASK;          // turn on PIT
}

bool NXP_PIT::init() {
    if (!callbackNumber) {
        return false;
    }

    enable();
    PIT->CHANNEL[channel].LDVAL = (CLOCK_GetFreq(kCLOCK_BusClk) / frequency) - 1; // set timer counting
    PIT->CHANNEL[channel].TCTRL = 0;
    channelEnable();

    switch (channel) {
        case 0:
            NVIC_ClearPendingIRQ(PIT0_IRQn);
            NVIC_EnableIRQ(PIT0_IRQn);
            pits[0] = this;
            break;
        case 1:
            NVIC_ClearPendingIRQ(PIT1_IRQn);
            NVIC_EnableIRQ(PIT1_IRQn);
            pits[1] = this;
            break;
        case 2:
            NVIC_ClearPendingIRQ(PIT2_IRQn);
            NVIC_EnableIRQ(PIT2_IRQn);
            pits[2] = this;
            break;
        case 3:
            NVIC_ClearPendingIRQ(PIT3_IRQn);
            NVIC_EnableIRQ(PIT3_IRQn);
            pits[3] = this;
            break;
    }
    return true;
}

void NXP_PIT::channelDisable() {
    PIT->CHANNEL[channel].TCTRL &= ~PIT_TCTRL_TIE_MASK; // timer disable interrupt
    PIT->CHANNEL[channel].TCTRL &= ~PIT_TCTRL_TEN_MASK; // timer disable
}

void NXP_PIT::channelEnable() {
    PIT->CHANNEL[channel].TCTRL |= PIT_TCTRL_TIE_MASK; // timer enable interrupt
    PIT->CHANNEL[channel].TCTRL |= PIT_TCTRL_TEN_MASK; // timer enable
}
