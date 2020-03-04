/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 * HALina adc driver
 *
 */

#pragma once

#include <gsl/gsl>

namespace halina {

    class ADC {
    public:
        virtual void init() = 0;

        virtual void startConversion() = 0;
    };
}
