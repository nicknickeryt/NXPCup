/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 * HALina uart controller
 *
 */
#pragma once

#include "HALina.hpp"

namespace halina {

    template<int rx_size, int tx_size>
    class UART {

    public:
        UART() {
        }

        virtual void init() = 0;

        virtual void write(void const*) = 0;

        virtual char read() = 0;
    };

}