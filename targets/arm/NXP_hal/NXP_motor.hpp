/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 * NXP HALina implementation of motor driver
 *
*/

#pragma once

#include "HALina.hpp"
#include "NXP_pwm.hpp"

class Kitty;

class NXP_Motor : public halina::Motor{
private:
    constexpr static auto NUMPR = 10;
private:
    Kitty& kitty;
    int32_t speed = 0;
    NXP_PWM& pwm;
    NXP_GPIO enablePin;

public:
    NXP_Motor(Kitty& kitty_, NXP_PWM& pwm, NXP_GPIO enablePin) : kitty(kitty_), pwm(pwm), enablePin(enablePin) {}

    void init() override;

    void setValue(int32_t) override;

    int32_t getValue() override;

    void block() override { enablePin.reset(); }

    void run() override { enablePin.set(); };

};