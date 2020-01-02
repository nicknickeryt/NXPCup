/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 * HALina servo controller
 *
 */
#pragma once

namespace halina {

    class Servo {
    public:
        Servo() = default;

        virtual void init() = 0;

        virtual void set(float position) = 0;

        virtual float get() = 0;

    };

}
