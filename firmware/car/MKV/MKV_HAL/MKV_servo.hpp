/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 * MKV HALina implementation of servo driver
 *
 */

#pragma once

#include <bits/algorithmfwd.h>
#include "HALina.hpp"
#include "MKV_pwm.hpp"

class Kitty;

class MKV_Servo : public halina::Servo{
private:
    float offset;
    uint16_t centerTicks;
    MKV_PWM& pwm;
    uint8_t maxDegreeDeviation;
    uint32_t maxTicksDeviation;
    float servoMultiplier;
    float currentValue = 0.0;

public:
    MKV_Servo(float offset, MKV_PWM& pwm, uint8_t maxDegreeDeviation, float servoMultiplier) : offset(offset), pwm(pwm), servoMultiplier(servoMultiplier) {
        this->maxDegreeDeviation = std::clamp(maxDegreeDeviation, (uint8_t)0, (uint8_t)90);
    }

    void init() override;

    void set(float) override;

    void setOffset(float offset);

    float get(){return currentValue;};

    void disable() {
        pwm.setRawPeriod(0, pwm.getChannel());
    }
};
