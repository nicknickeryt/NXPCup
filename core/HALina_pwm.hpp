/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 * HALina pwm controller
 *
 */
#pragma once

#include <gsl/gsl>

namespace halina {

    class PWM {
    public:
        virtual void init() = 0;

        virtual void setDutyCycle(int32_t value) = 0;

        virtual int32_t getDutyCycle() = 0;
    };
}