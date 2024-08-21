/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 * MKV HALina implementation of pwm driver
 *
 */
#pragma once

#include "HALina.hpp"
#include "clock_config.h"
#include "MKV_gpio.hpp"

class Kitty;

class MKV_PWM {
private:
    FTM_Type* ftm;
    MKV_PORT& port;

    uint32_t frequency;
    uint8_t dividerIndex = 0;
    uint16_t modulo = 0;
    float currentValue = 0;
    uint16_t channel = 0;

    constexpr static uint8_t dividers[8] = {1, 2, 4, 8, 16, 32, 64, 128};
public:
    MKV_PWM(FTM_Type* ftm, MKV_PORT& port, uint8_t channel, uint32_t frequency) :
            ftm(ftm), port(port), frequency(frequency), channel(channel){ }

    void setDutyCycle(float dutyCycle);

    float getDutyCycle();

    void init();

    uint8_t getChannel(){return channel;}

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