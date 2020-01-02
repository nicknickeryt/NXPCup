/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 * NXP HALina implementation of motor driver
 *
*/

#pragma once

#include "HALina.hpp"
#include "HALina_config.hpp"

class PID{
public:
    void init();
};


class NXP_RightMotor : public halina::Motor {
public:
    constexpr static auto motorEn = ((uint32_t)(1<<4));
    constexpr static auto motorFb = ((uint32_t)(1<<3));
    constexpr static auto motorMaxRatio = int32_t(MOTOR_CLOCK_MOD-1);
    constexpr static auto motorMinRatio = int32_t(-motorMaxRatio);
    constexpr static auto motorZeroRatio = int32_t(0);
    constexpr static auto NUMPR = 10;
    constexpr static auto motorEnSetHigh() {GPIOE->PSOR = motorEn; }
    constexpr static auto motorEnSetLow() {GPIOE->PCOR = motorEn; }
    constexpr static auto motorEnToggle() {GPIOE->PTOR = motorEn; }

private:
    int32_t speed = 0;
    int32_t error = 0;
    int32_t before = 0;
    int32_t speedPid = 0;
    int32_t encoder = 0;
    int32_t wheelDistance = 0;

public:
    void init() override;

    void setValue(int32_t) override;

    int32_t getValue() override;
};


class NXP_LeftMotor : public halina::Motor {
public:
    constexpr static auto motorEn = ((uint32_t)(1<<4));
    constexpr static auto motorFb = ((uint32_t)(1<<3));
    constexpr static auto motorMaxRatio = int32_t(MOTOR_CLOCK_MOD-1);
    constexpr static auto motorMinRatio = int32_t(-motorMaxRatio);
    constexpr static auto motorZeroRatio = int32_t(0);
    constexpr static auto NUMPR = 10;
    constexpr static auto motorEnSetHigh() {GPIOE->PSOR = motorEn; }
    constexpr static auto motorEnSetLow() {GPIOE->PCOR = motorEn; }
    constexpr static auto motorEnToggle() {GPIOE->PTOR = motorEn; }

private:
    int32_t speed = 0;
    int32_t error = 0;
    int32_t before = 0;
    int32_t speedPid = 0;
    int32_t encoder = 0;
    int32_t wheelDistance = 0;

public:
    void init() override;

    void setValue(int32_t) override;

    int32_t getValue() override;
};