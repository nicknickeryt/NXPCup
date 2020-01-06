/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 * NXP Main file
 *
 */

#include <device/MKV58F24.h>
#include "HALina.hpp"
#include "NXP_Kitty.hpp"
#include "clock_config.h"
#include "NXP_PIT.hpp"

using namespace halina;

volatile bool flaga0 = false;
volatile bool flaga1 = false;
volatile bool flaga2 = false;
volatile bool flaga3 = false;

void handler0() {
    flaga0 = true;
}

void handler1() {
    flaga1 = true;
}

void handler2() {
    flaga2 = true;
}

void handler3() {
    flaga3 = true;
}


int main(){
    BOARD_BootClockRUN();
    Kitty& kitty = Kitty::kitty();

    kitty.init();
    kitty.uartDebug.write("Hello world xD\n");
    kitty.servo.set(0.0f);

    NXP_PIT pit0 = NXP_PIT(NXP_PIT::CHANNEL::_0, 2, handler0);
    NXP_PIT pit1 = NXP_PIT(NXP_PIT::CHANNEL::_1, 2, nullptr);
//    NXP_PIT pit2 = NXP_PIT(NXP_PIT::CHANNEL::_2, 3, handler2);
//    NXP_PIT pit3 = NXP_PIT(NXP_PIT::CHANNEL::_3, 4, handler3);

    pit0.init();
    pit1.init();
//    pit2.init();
//    pit3.init();


    while (true){
        kitty.proc();
        if (flaga0) {
            flaga0 = false;
            kitty.ledLine.at(0).toggle();
        }

        if (flaga1) {
            flaga1 = false;
            kitty.ledLine.at(1).toggle();
        }

        if (flaga2) {
            flaga2 = false;
            kitty.ledLine.at(2).toggle();
        }

        if (flaga3) {
            flaga3 = false;
            kitty.ledLine.at(3).toggle();
        }


    }
}



