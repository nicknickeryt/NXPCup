/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 * NXP Main file
 *
 */

#include "NXP_hal.hpp"

using namespace halina;

int main(void){
    Kitty& kitty = Kitty::kitty();

    kitty.init();
    kitty.uart.write("Hello world xD\n");

    while (true){
        kitty.proc();

    }
}