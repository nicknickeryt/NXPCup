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

        virtual void setValue(int32_t) = 0;

        virtual int32_t getValue() = 0;
    };
}