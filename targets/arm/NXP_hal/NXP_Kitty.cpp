#include "NXP_hal.hpp"

void Kitty::init() {
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();

    uart.init();
}

void Kitty::proc() {
    uart.proc();
}