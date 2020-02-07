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
    uint16_t centerTicks;
    NXP_PWM& pwm;
    uint8_t maxDegreeDeviation;
    uint32_t maxTicksDeviation;
    float servoMultiplier;

public:
    NXP_Servo(NXP_PWM& pwm, uint8_t maxDegreeDeviation, float servoMultiplier) :  pwm(pwm), servoMultiplier(servoMultiplier) {
        this->maxDegreeDeviation = std::clamp(maxDegreeDeviation, (uint8_t)0, (uint8_t)90);
    }

    void init() override;

    void set(float) override;

    float get() override;
};

