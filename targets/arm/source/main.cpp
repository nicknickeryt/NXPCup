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

#define LOG_CHANNEL APP
#define APP_LOG_CHANNEL 1
#define APP_LOG_CHANNEL_LEVEL LOG_LEVEL_DEBUG

#include "logger.h"

using namespace halina;

int main(){
    BOARD_BootClockRUN();
    Kitty& kitty = Kitty::kitty();

    kitty.init();
    kitty.servo.set(0.0);
    kitty.display.print(6969);

    log_notice("Application started");

    while (true){
        kitty.proc();

    }
}



