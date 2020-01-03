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
#include "NXP_gpio.hpp"

class NXP_PWM : public halina::PWM{
private:
    float dutyCycle;
    FTM_Type* ftm;
    NXP_PORT& portFirst;
    NXP_PORT& portSecond;
    uint8_t channelFirst;
    uint8_t channelSecond;
    uint32_t clockPrescaler;

public:
    NXP_PWM(FTM_Type* ftm, NXP_PORT& portFirst, NXP_PORT& portSecond, uint8_t channelFirst, uint8_t channelSecond, uint32_t clockPrescaler) :
            ftm(ftm), portFirst(portFirst), portSecond(portSecond), channelFirst(channelFirst), channelSecond(channelSecond), clockPrescaler(clockPrescaler) { }

    void setDutyCycle(int32_t value) override;

    void init() override;

    int32_t getDutyCycle() override;

};