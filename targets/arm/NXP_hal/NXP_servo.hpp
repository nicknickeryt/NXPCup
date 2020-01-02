/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 * NXP HALina implementation of servo
 *
 */

#pragma once

#include "NXP_hal.hpp"

class NXP_Servo : public halina::Servo{
public:
    NXP_Servo();

    void set(float position) override;

    float get() override;
};
