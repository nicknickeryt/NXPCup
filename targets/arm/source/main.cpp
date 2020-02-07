/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 * NXP Main file
 *
 */

#include <drivers/fsl_uart.h>
#include <drivers/fsl_uart_edma.h>
#include "HALina.hpp"
#include "NXP_Kitty.hpp"
#include "clock_config.h"

#define LOG_CHANNEL APP
#define APP_LOG_CHANNEL 1
#define APP_LOG_CHANNEL_LEVEL LOG_LEVEL_DEBUG

//#include "logger.h"

using namespace halina;

int main(){
    BOARD_BootClockRUN();
    Kitty& kitty = Kitty::kitty();


    kitty.init();
//    Kitty::kitty().motorLeft.init();
//    Kitty::kitty().motorRight.init();
//
//    Kitty::kitty().motorLeftPwm.init();
//    Kitty::kitty().motorLeftPwm.setDutyCycle(0.2, Kitty::kitty().motorLeftPwm.channelFirst);
//    Kitty::kitty().motorLeftPwm.setDutyCycle(0.4, Kitty::kitty().motorLeftPwm.channelSecond);
//
//    Kitty::kitty().motorRightPwm.init();
//    Kitty::kitty().motorRightPwm.setDutyCycle(0.6, Kitty::kitty().motorRightPwm.channelFirst);
//    Kitty::kitty().motorRightPwm.setDutyCycle(0.8, Kitty::kitty().motorRightPwm.channelSecond);

//    Kitty::kitty().servoPwm.init();
//    NXP_Single_PWM mojepwm = {FTM0, 200,Kitty::kitty().servoPort};
//    mojepwm.init();

//Kitty::kitty().servoPwm.init();

    while (true){
        kitty.proc();
    }
}



