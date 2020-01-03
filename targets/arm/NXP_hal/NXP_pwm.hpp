/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 * NXP HALina implementation of pwm driver
 *
 */
#pragma once

#include "HALina.hpp"
#include "clock_config.h"

class NXP_PWM : public halina::PWM{
private:
    float dutyCycle;

public:
    static constexpr auto clockPrescaler = 64;
    static constexpr auto outputClockPrescaler = 50;
    static constexpr auto clockMod = BOARD_BOOTCLOCKRUN_CORE_CLOCK/clockPrescaler/outputClockPrescaler;

public:
    NXP_PWM() = default;

    void setDutyCycle(int32_t value) override;

    void init() override;

    int32_t getDutyCycle() override;

};