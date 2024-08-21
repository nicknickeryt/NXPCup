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

    class Uart {

    public:
        Uart() {
        }

        virtual void init() = 0;

        virtual void write(void const* data, uint16_t length) = 0;

        virtual void write(uint8_t data) = 0;

        virtual uint8_t read() = 0;
    };

}