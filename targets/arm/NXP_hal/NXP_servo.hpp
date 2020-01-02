/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 * NXP HALina implementation of servo driver
 *
 */

#pragma once

#include "HALina.hpp"
#include "NXP_pwm.hpp"

class NXP_Servo : public halina::Servo{
private:
    NXP_PWM pwm;

    class Filter{
    private:
        static constexpr auto tapNumber = 25;
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
    };

    Filter filter;

public:
    NXP_Servo() = default;

    void init() override;

    void set(float) override;

    float get() override;
};

