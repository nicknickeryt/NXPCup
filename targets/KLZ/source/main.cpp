/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 * Main file for KL25Z
 *
 */

#include "NXP_Kitty.hpp"

int main() {
    BOARD_BootClockRUN();
    Kitty& kitty = Kitty::kitty();
    kitty.init();

    while(true) {
        kitty.proc();
    }
}
