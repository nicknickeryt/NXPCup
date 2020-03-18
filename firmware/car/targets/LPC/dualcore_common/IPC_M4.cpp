#include "IPC.h"
#include "cmsis.h"
#include "core_cm4.h"

template <> void IPC<CPU::M4>::setNVIC() {
    NVIC_SetPriority(M0APP_IRQn, 7);
    NVIC_EnableIRQ(M0APP_IRQn);
}
