/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 * MKV HALina implementation of motor driver
 *
*/

#pragma once

#include <utility>
#include "HALina.hpp"
#include "MKV_pwm.hpp"

class Kitty;
class MKV_Motors;

class MKV_Motor : public halina::Motor{
    friend class MKV_Motors;
private:
    enum class MotorDirection{
    FORWARDS = 0,
    BACKWARDS = 1
};
    int32_t speed = 0;
    MKV_PWM& pwmForward;
    MKV_PWM& pwmBackward;
    PWM_Filter filter;
    MKV_GPIO enablePin;
    MotorDirection motorDirection = MotorDirection::FORWARDS;
public:
    MKV_Motor(MKV_PWM& pwmForward, MKV_PWM& pwmBackward, MKV_GPIO& enablePin) :
            pwmForward(pwmForward), pwmBackward(pwmBackward), enablePin(enablePin) { }

    void init() override;

    void setValue(float) override;

    float getValue() override;

    void block() override { enablePin.reset(); }

    void run() override { enablePin.set(); };

};

class MKV_Motors {
    MKV_Motor& left;
    MKV_Motor& right;

public:
    void init() {
        left.init();
        right.init();
    }

    void setValue(float leftValue, float rightValue) {
        left.setValue(leftValue);
        right.setValue(rightValue);
    }

    bool enabled() {
        return left.enablePin.get() && right.enablePin.get();
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

    MKV_Motors(MKV_Motor& left, MKV_Motor& right) : left(left), right(right) { }
};