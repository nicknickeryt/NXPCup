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
        Servo();

        virtual void set(float position);

        virtual float get();
    };

}
