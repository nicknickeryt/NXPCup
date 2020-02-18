/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 * HALina buttons controller
 *
 */

#pragma once

#include "HALina_gpio.hpp"

namespace halina{
    class Buttons{
        constexpr static auto buttonNumber = 4;
    private:
        halina::GPIO &BUTTON0;
        halina::GPIO &BUTTON1;
        halina::GPIO &BUTTON2;
        halina::GPIO &BUTTON3;

        halina::GPIO *BUTTON[buttonNumber] = {&BUTTON0, &BUTTON1, &BUTTON2, &BUTTON3};

    public:
        Buttons(GPIO &BUTTON0, GPIO &BUTTON1, GPIO &BUTTON2, GPIO &BUTTON3) :
        BUTTON0(BUTTON0), BUTTON1(BUTTON1), BUTTON2(BUTTON2), BUTTON3(BUTTON3){}

        void init(){
            for(auto i = 0; i < buttonNumber; i++){
                BUTTON[i]->init();
            }
        }

        GPIO &at(uint8_t i){
            if(i >= buttonNumber){
                return *BUTTON[0];
            }
            return *BUTTON[i];
        }
    };
}