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

class NXP_Motor {
public:
class NXP_MotorLeft : public halina::Motor {
    friend class NXP_Motor;
    private:
    constexpr static auto NUMPR = 10;
    private:
        int32_t speed = 0;
        NXP_Motor& motor;
        NXP_PWM& pwm;
    public:
    NXP_MotorLeft(NXP_Motor& motor, NXP_PWM& pwm) : motor(motor), pwm(pwm){ }

    void init() override;

    void setValue(int32_t) override;

    int32_t getValue() override;
    };

    class NXP_MotorRight : public halina::Motor {
        friend class NXP_Motor;
    private:
        constexpr static auto NUMPR = 10;
    private:
        int32_t speed = 0;
        NXP_Motor& motor;
        NXP_PWM& pwm;
    public:

        NXP_MotorRight(NXP_Motor& motor, NXP_PWM& pwm) : motor(motor), pwm(pwm){ }

        void init() override;

        void setValue(int32_t) override;

        int32_t getValue() override;
    };

private:
    Kitty& kitty;
public:
    NXP_MotorLeft motorLeft;
    NXP_MotorRight motorRight;

public:
    NXP_Motor(Kitty& kitty_, NXP_PWM& left, NXP_PWM& right) : kitty(kitty_), motorLeft(*this, left), motorRight(*this,right) {}
};