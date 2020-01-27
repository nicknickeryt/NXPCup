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

//    NXP_PIT pittt = NXP_PIT(NXP_PIT::CHANNEL::_3, 400, adc_trigger);

//    adc.appendSample(&camera1Sample);
//    adc.appendSample(&camera1Sample);
//    adc.appendSample(&camera1Sample);
//    adc.appendSample(&camera1Sample);
//
//    adc.appendSample(&camera2Sample);
//    adc.appendSample(&camera2Sample);
//    adc.appendSample(&camera2Sample);
//    adc.appendSample(&camera2Sample);
//    adc.init();


//    volatile uint16_t dddd= HSADC0->CTRL2;
//    volatile uint16_t ddd= HSADC0->CTRL1;
//    pittt.init();

    kitty.camera.start();
    while (true){
        kitty.proc();
//        kitty.servo.set(0.4f);
    }
}



