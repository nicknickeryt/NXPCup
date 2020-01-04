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
    uint32_t a = 10;
    int32_t b = -3;
    float c = 5.67;
    char d = 'e';
    log_notice("TEST xDDD a=%d, b=%d, c=%f, d=%c", a, b, c, d);
    kitty.servo.set(0.0);

    kitty.display.print(6969);

    while (true){
        kitty.proc();

    }
}