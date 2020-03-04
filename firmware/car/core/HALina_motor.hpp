/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 * HALina motor driver file
 *
 */

#pragma once

namespace halina {

    class Motor {
    public:
        virtual void init() = 0;

        virtual void setValue(float) = 0;

        virtual float getValue() = 0;

        virtual void block() = 0;

        virtual void run() = 0;
    };
}