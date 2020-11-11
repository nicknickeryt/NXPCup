/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 * Algorithm for KL25z
 *
 */

#include "VL53L0X.h"
#include "NXP_gpio.hpp"
#include "NXP_I2C.hpp"
#include "NXP_uart.hpp"
#include "NXP_frame.hpp"
#include <array>

#pragma once

class Algorithm {
    constexpr static auto sendThreshold = 250;
    constexpr static auto sensorNumber = 6;
private:
    VL53L0X right;
    VL53L0X rightFront;
    VL53L0X left;
    VL53L0X leftFront;
    VL53L0X additional_1;
    VL53L0X additional_2;

    NXP_Frame frame;
    std::array<NXP_GPIO, 4> &ledDebug;

public:
    Algorithm(NXP_I2C &i2c, NXP_Uart &toMKV, std::array<NXP_GPIO, 4> &ledDebug) :
            right(i2c),
            rightFront(i2c),
            left(i2c),
            leftFront(i2c),
            additional_1(i2c),
            additional_2(i2c),
            frame(toMKV, sensorNumber),
            ledDebug(ledDebug){}

    void init();

    void proc(volatile bool&);
};
