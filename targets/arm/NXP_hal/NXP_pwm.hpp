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

class Kitty;

class NXP_PWM : public halina::PWM{
private:
    int32_t dutyCycle = 0;
    Kitty& kitty;
    FTM_Type* ftm;
    NXP_PORT& portFirst;
    NXP_PORT& portSecond;
    uint8_t channelFirst;
    uint8_t channelSecond;
    uint32_t clockPrescaler;


public:
    NXP_PWM(Kitty& kitty, FTM_Type* ftm, NXP_PORT& portFirst, NXP_PORT& portSecond, uint8_t channelFirst, uint8_t channelSecond, uint32_t clockPrescaler) :
            kitty(kitty), ftm(ftm), portFirst(portFirst), portSecond(portSecond), channelFirst(channelFirst), channelSecond(channelSecond), clockPrescaler(clockPrescaler) { }

    void setDutyCycle(int32_t value) override;

    void init() override;

    int32_t getDutyCycle() override;

};

class PWM_Filter{
private:
    static constexpr auto tapNumber = 25;
    constexpr static auto NUMPR = 20;
    int32_t filterTapsArray[tapNumber] = {1949, 2709, -1061, -71, 939, -1477, 1453, -661, -908,
                                          2992,-5110,6681,25501,6681,-5110,2992,-908,-661,1453,-1477,939,-71,-1061,2709,1949};

    int32_t historicalSamples[tapNumber];
    uint16_t lastSamplesIndex;

public:
    void init(){
        for(auto i = 0; i < tapNumber; ++i){
            historicalSamples[i] = 0;
        }
        lastSamplesIndex = 0;
    }

    void putSample(int32_t sample){
        historicalSamples[lastSamplesIndex++] = sample;
        if(lastSamplesIndex == tapNumber){
            lastSamplesIndex = 0;
        }
    }

    int64_t getSample(){
        int64_t result = 0;
        int index = lastSamplesIndex;
        for(auto i = 0; i < tapNumber; ++i) {
            index = index != 0 ? index-1 : tapNumber-1;
            result += (int64_t)historicalSamples[index] * filterTapsArray[i];
        };
        return result >> 16;
    }

    void runningAverage(int32_t* value){
        static int32_t avg[NUMPR];
        for (uint8_t i =0; i < (NUMPR-1); i ++)
        {
            avg[i] = avg[i+1];
        }
        avg[NUMPR-1] = *value;

        *value = 0;
        for (uint8_t i =0; i < NUMPR; i ++)
        {
            *value += avg[i];
        }
        *value /= NUMPR;
    }

};