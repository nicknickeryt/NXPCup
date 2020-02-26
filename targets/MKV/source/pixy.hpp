/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 *
 *
 */
#pragma once

#include "NXP_uart.hpp"

class Pixy{
    private:
        static constexpr char startOfFrame = 'R';
        NXP_Uart& uart;

//        struct Packet{
//
//        };

    public:
        Pixy(NXP_Uart& uart) : uart(uart){};

        bool getData();

    private:
        void sendRequest();
};