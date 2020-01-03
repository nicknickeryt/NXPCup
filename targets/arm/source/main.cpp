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
    kitty.uart.write("Hello world xD\n");
    kitty.display.print(4321);

    while (true){
        kitty.proc();

        // todo delete this test!
        static uint32_t i;
        static uint32_t counter;
        if(10000 == i++){
            kitty.display.print(++counter);
            i = 0;
            kitty.servo.set(counter);
            if(counter == 9999){
                counter = 0;
            }
        }
    }
}