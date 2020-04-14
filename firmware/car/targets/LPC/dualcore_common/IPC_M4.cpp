#include "IPC.hpp"
#include "chip.h"

IPC<CPU::M4> *ipc_m4;

template <> void IPC<CPU::M4>::setNVIC() {
    NVIC_SetPriority(M0APP_IRQn, 7);
    NVIC_EnableIRQ(M0APP_IRQn);
    ipc_m4 = this;
}

template <> void IPC<CPU::M4>::sendSignal(){
    __DSB();
    __SEV();
}

extern "C" void M0APP_IRQHandler(void) {
    Chip_CREG_ClearM0AppEvent();
    ipc_m4->runCallback();
}
