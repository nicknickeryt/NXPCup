/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 * HALina gpio class
 *
 */

#pragma once

#include <gsl/gsl>

namespace halina {

    class GPIO {

    public:
        enum class Mode : uint8_t {
            INPUT,
            OUTPUT,
            TIMER,
        };

        virtual void set() = 0;

        virtual void reset() = 0;

        virtual bool get() = 0;

        virtual void toggle() = 0;

        virtual void init() = 0;
    };

}