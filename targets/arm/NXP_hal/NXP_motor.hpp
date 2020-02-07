/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 * NXP HALina implementation of motor driver
 *
*/

#pragma once

#include <utility>
#include "HALina.hpp"
#include "NXP_pwm.hpp"

class Kitty;
class NXP_Motors;

class NXP_Motor : public halina::Motor{
private:
    int32_t speed = 0;
    NXP_PWM& pwm;
    PWM_Filter filter;
    NXP_GPIO enablePin;
    friend class NXP_Motors;
public:
    NXP_Motor(NXP_PWM& pwm, NXP_GPIO& enablePin) :
        pwm(pwm), enablePin(enablePin) { }

    void init() override;

    void setValue(float) override;

    float getValue() override;

    void block() override { enablePin.reset(); }

    void run() override { enablePin.set(); };

};

class NXP_Motors {
    NXP_Motor& left;
    NXP_Motor& right;

public:
    void init() {
        left.init();
        right.init();
    }

    void setValue(float leftValue, float rightValue) {
        left.setValue(leftValue);
        right.setValue(rightValue);
    }

    std::pair<float, float> getValue() {
        std::pair<float, float> ret;
        ret.first = left.getValue();
        ret.second = right.getValue();
        return ret;
    }

    void block() {
        left.block();
        right.block();
    }
    void run() {
        left.run();
        right.run();
    }

    NXP_Motors(NXP_Motor& left, NXP_Motor& right) : left(left), right(right) { }
};