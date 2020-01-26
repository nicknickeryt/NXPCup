/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 * NXP HALina implementation of adc
 *
 */

#pragma once

#include "HALina.hpp"
#include "fsl_hsadc.h"
#include "NXP_gpio.hpp"

using ADC_Channel = uint16_t;
using ADC_Mux = uint16_t;

class NXP_ADC : public halina::ADC{
public:
    // page 1026 from reference manual
    enum class ChannelSingleEnded : uint8_t {
        // channels A
                A_CH0 = 0u << 4u, A_CH1 = 1u << 4u, A_CH2 = 2u << 4u, A_CH3 = 3u << 4u, A_CH4 = 4u << 4u, A_CH5 = 5u << 4u,
        // channels A 6
                A_CH6 = 6u << 4u,
        A_CH8 = A_CH6 | 1u, A_CH10 = A_CH6 | 2u, A_CH12 = A_CH6 | 3u, A_CH14 = A_CH6 | 4u, A_CH16 = A_CH6 | 5u, A_CH18 = A_CH6 | 6u,
        // channels A 7
                A_CH7 = 7u << 4u,
        A_CH9 = A_CH7 | 1u, A_CH11 = A_CH7 | 2u, A_CH13 = A_CH7 | 3u, A_CH15 = A_CH7 | 4u, A_CH17 = A_CH7 | 5u,
        // channels B
                B_CH0 = 8u << 4u, B_CH1 = 9u << 4u, B_CH2 = 10u << 4u, B_CH3 = 11u << 4u, B_CH4 = 12u << 4u, B_CH5 = 13u << 4u,
        // channels B 14
                B_CH6 = 14u << 4u,
        B_CH8 = B_CH6 | 1u, B_CH10 = B_CH6 | 2u, B_CH12 = B_CH6 | 3u, B_CH14 = B_CH6 | 4u, B_CH16 = B_CH6 | 5u, B_CH18 = B_CH6 | 6u,
        // channels B 15
                B_CH7 = 15u << 4u,
        B_CH9 = B_CH7 | 1u, B_CH11 = B_CH7 | 2u, B_CH13 = B_CH7 | 3u, B_CH15 = B_CH7 | 4u, B_CH17 = B_CH7 | 5u
    };

    class Sample;

    class Converter {
    public:
        enum class Type : uint16_t {
            A = kHSADC_ConverterA,
            B = kHSADC_ConverterB
        };

        Type type;
        uint8_t samplesCount = 0;
        std::array<Sample *, 8> samples{};
        uint16_t conversionValues[8] = {0};
        void (*converterHandler)(uint8_t){};

        Converter(Type type) : type(type) {

        }

        bool appendSample(Sample *sample) {
            if (samplesCount < samples.size()) {
                samples.at(samplesCount) = sample;
                samplesCount++;
                return true;
            }
            return false;
        }
    };

    class Sample {
        bool initialised = false;
        NXP_PORT pinMux;
        ChannelSingleEnded channel;
        uint32_t currentValue;
    public:
        Converter::Type converterType;

        Sample (NXP_PORT& pinMux, ChannelSingleEnded channel): pinMux(pinMux), channel(channel){
            if (static_cast<uint8_t>(channel) <= static_cast<uint8_t>(ChannelSingleEnded::A_CH18)) {
                converterType = Converter::Type::A;
            } else {
                converterType = Converter::Type::B;
            }
        }
        void init(HSADC_Type* baseAdc, uint8_t indexInRegisterSample);
    };

    constexpr static auto sampleMask = 15U;

private:
    HSADC_Type* baseAdc;

    Converter A = {Converter::Type::A};
    Converter B = {Converter::Type::B};
public:

    NXP_ADC(HSADC_Type* baseAdc, void (*converterAHandler)(uint8_t), void (*converterBHandler)(uint8_t)) : baseAdc(baseAdc) {
        A.converterHandler = converterAHandler;
        B.converterHandler = converterBHandler;
    }

    bool appendSample(Sample *sample);

    void saveData(Converter::Type converterType);

    void init() override;

    uint16_t *getBufferValues(Converter::Type converterType);

    void startConversion() override;
};