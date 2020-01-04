/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 * NXP HALina implementation of servo driver
 *
 */

#pragma once

#include "HALina.hpp"
#include "NXP_pwm.hpp"

class Kitty;

class NXP_Servo : public halina::Servo{
private:
    Kitty& kitty;
    NXP_PWM& pwm;
    PWM_Filter filter;

public:
    int32_t servoMinValue;
    int32_t servoMaxValue;
    int32_t servoCenterValue;

public:
    NXP_Servo(Kitty& kitty_, NXP_PWM& pwm, int32_t minValue, int32_t maxValue) : kitty(kitty_), pwm(pwm), servoMinValue(minValue), servoMaxValue(maxValue), servoCenterValue((maxValue + minValue)/2) { }

    void init() override;

    void set(float) override;

    float get() override;
};

