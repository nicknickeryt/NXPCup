#include "NXP_PIT.hpp"

std::array<std::function<void(void)>, 4> NXP_PIT::handlers;

extern "C" {
void PIT0_IRQHandler(void) {
    PIT->CHANNEL[0].TFLG = PIT_TFLG_TIF_MASK;
    NXP_PIT::handlers[0]();
}

void PIT1_IRQHandler(void) {
    PIT->CHANNEL[1].TFLG = PIT_TFLG_TIF_MASK;
    NXP_PIT::handlers[1]();
}

void PIT2_IRQHandler(void) {
    PIT->CHANNEL[2].TFLG = PIT_TFLG_TIF_MASK;
    NXP_PIT::handlers[2]();
}

void PIT3_IRQHandler(void) {
    PIT->CHANNEL[3].TFLG = PIT_TFLG_TIF_MASK;
    NXP_PIT::handlers[3]();
}
}
