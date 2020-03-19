#include "IPC.h"
#include "cmsis.h"
#include "core_cm4.h"
#include "board.h"
#include "chip_lpc43xx.h"
#include "creg_18xx_43xx.h"

template <> void IPC<CPU::M4>::setNVIC() {
    NVIC_SetPriority(M0APP_IRQn, 7);
    NVIC_EnableIRQ(M0APP_IRQn);
}

template <> void IPC<CPU::M4>::sendSignal(){
    __DSB();
    __SEV();

}

extern "C" void M0APP_IRQHandler(void) {
    Chip_CREG_ClearM0AppEvent();
    Board_LED_Toggle(1);
}
