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

class NXP_PWM {
private:
    FTM_Type* ftm;
    NXP_PORT& portFirst;
    NXP_PORT& portSecond;

    uint32_t frequency;
    uint8_t dividerIndex = 0;
    uint16_t modulo = 0;

    constexpr static uint8_t dividers[8] = {1, 2, 4, 8, 16, 32, 64, 128};
public:
    uint8_t channelFirst;
    uint8_t channelSecond;

    NXP_PWM(FTM_Type* ftm, NXP_PORT& portFirst, NXP_PORT& portSecond, uint8_t channelFirst, uint8_t channelSecond, uint32_t frequency) :
            ftm(ftm), portFirst(portFirst), portSecond(portSecond),frequency(frequency), channelFirst(channelFirst), channelSecond(channelSecond) { }

    void setDutyCycle(float dutyCycle, uint8_t channel);

    void init();

    uint32_t getTicksPerSecond() {
        return CLOCK_GetFreq(kCLOCK_FastPeriphClk) / dividers[dividerIndex];
    }

    void setRawPeriod( uint16_t period, uint8_t channel) {
        ftm->CONTROLS[channel].CnV = FTM_CnV_VAL(std::clamp(period, (uint16_t)0, modulo));
    }

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