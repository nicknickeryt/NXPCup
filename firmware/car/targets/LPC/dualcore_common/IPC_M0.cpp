#include "IPC.hpp"
#include "cmsis.h"
#include "core_cm0.h"
#include "board.h"
#include "chip_lpc43xx.h"
#include "creg_18xx_43xx.h"

IPC<CPU::M0> *ipc_m0;

template <> void IPC<CPU::M0>::setNVIC() {
    NVIC_SetPriority(M4_IRQn, 7);
    NVIC_EnableIRQ(M4_IRQn);
    ipc_m0 = this;
}

template <> void IPC<CPU::M0>::sendSignal() {
    __DSB();
    __SEV();
}

extern "C" void M4_IRQHandler(void) {
    Chip_CREG_ClearM4Event();
    ipc_m0->runCallback();
}
