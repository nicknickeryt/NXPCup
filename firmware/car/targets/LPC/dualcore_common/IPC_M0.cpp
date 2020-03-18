#include "IPC.h"
#include "cmsis.h"
#include "core_cm0.h"

template <> void IPC<CPU::M0>::setNVIC()  {
    NVIC_SetPriority(M4_IRQn, 7);
    NVIC_EnableIRQ(M4_IRQn);
}
