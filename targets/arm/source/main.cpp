/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 * NXP Main file
 *
 */

#include "HALina.hpp"
#include "NXP_Kitty.hpp"
#include "clock_config.h"

using namespace halina;

int main(){
    BOARD_BootClockRUN();
    Kitty& kitty = Kitty::kitty();

    kitty.init();
    kitty.uartDebug.write("Hello world xD\n");
    kitty.servo.set(0.0f);
    kitty.display.print(10.90f, 2);

    while (true){
        kitty.proc();
    }
}