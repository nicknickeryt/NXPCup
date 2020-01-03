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
    kitty.servo.set(0.0);

    kitty.display.print(6969);

    while (true){
        kitty.proc();

        // todo delete this test!
        static uint32_t i;
        static float counter = -1.0;
        if(10000 == i++){
            counter += 0.01;
            i = 0;
            kitty.servo.set(counter);
            if(counter >=  1.0){
                counter =  -1.0;
            }
        }
    }
}