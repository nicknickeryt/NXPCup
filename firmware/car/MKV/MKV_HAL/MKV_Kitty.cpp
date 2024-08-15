#include "MKV_Kitty.hpp"

#include "clock_config.h"
#include "pin_mux.h"

void Kitty::init() {
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    SysTick_Config(SystemCoreClock / 1000);
    NVIC_ClearPendingIRQ(SysTick_IRQn);
    NVIC_EnableIRQ(SysTick_IRQn);

    ledLine.init();
    ledLine.at(0).set();
}
