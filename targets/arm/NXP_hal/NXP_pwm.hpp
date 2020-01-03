/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 * NXP HALina implementation of pwm driver
 *
 */

#include "HALina.hpp"

#pragma once

class NXP_PWM : public halina::PWM{
private:
    float dutyCycle;

public:
    // clock consts
    static constexpr auto magicNumber = 0.99;
    static constexpr auto clockPrescaler = 64;
    static constexpr auto inputClockFrequency = DEFAULT_SYSTEM_CLOCK;
    static constexpr auto outputClock = 50;
    static constexpr auto clockMod = inputClockFrequency/clockPrescaler/outputClock/magicNumber;

public:
    NXP_PWM() = default;

    void setDutyCycle(int32_t value) override;

    void init() override;

    int32_t getDutyCycle() override;

};