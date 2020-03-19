#include "IPC.h"
#include "cmsis.h"
#include "core_cm0.h"
#include "board.h"
#include "chip_lpc43xx.h"
#include "creg_18xx_43xx.h"

template <> void IPC<CPU::M0>::setNVIC() {
    NVIC_SetPriority(M4_IRQn, 7);
    NVIC_EnableIRQ(M4_IRQn);
}

template <> void IPC<CPU::M0>::sendSignal() {
    __DSB();
    __SEV();
}

extern "C" void M4_IRQHandler(void) {
    Chip_CREG_ClearM4Event();
    Board_LED_Toggle(0);
}