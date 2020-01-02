/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 * HALina pwm controller
 *
 */
#pragma once

#include <gsl/gsl>

 class PWM {
public:
    virtual void set_duty_cycle(float) = 0;
    virtual void off() = 0;
    virtual void on() = 0;
    virtual float get_duty_cycle() = 0;
};