/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 * HALina switches controller
 *
 */

#pragma once

#include "HALina_gpio.hpp"

namespace halina{
    class Switches{
        constexpr static auto switchesNumber = 4;
    private:
        halina::GPIO &SWITCH0;
        halina::GPIO &SWITCH1;
        halina::GPIO &SWITCH2;
        halina::GPIO &SWITCH3;

        halina::GPIO *SWITCHES[switchesNumber] = {&SWITCH0, &SWITCH1, &SWITCH2, &SWITCH3};

    public:
        Switches(GPIO &SWITCH0, GPIO &SWITCH1, GPIO &SWITCH2, GPIO &SWITCH3) :
                SWITCH0(SWITCH0), SWITCH1(SWITCH1), SWITCH2(SWITCH2), SWITCH3(SWITCH3){}

        void init(){
            for(auto i = 0; i < switchesNumber; i++){
                SWITCHES[i]->init();
            }
        }

        GPIO &at(uint8_t i){
            if(i >= switchesNumber){
                return *SWITCHES[0];
            }
            return *SWITCHES[i];
        }
    };
}