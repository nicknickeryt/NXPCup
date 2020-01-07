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

using ADC_Channel = uint16_t;
using ADC_Mux = uint16_t;

class NXP_ADC : public halina::ADC{
public:
    enum class Converter : uint16_t{
        CONVERTER_A = kHSADC_ConverterA,
        CONVERTER_B = kHSADC_ConverterB
    };

private:
    HSADC_Type* adc;
    Converter converterType;
    ADC_Channel channel;
    ADC_Mux mux;
    bool enableDifferentialPair;

    uint32_t currentValue;

private:
    void autoCalibration();

    void enableInterrupts();

    void disableInterrupts();

public:
    NXP_ADC(HSADC_Type* adc, Converter converterType, ADC_Channel channel, ADC_Mux mux, bool differentialPair) :
                                    adc(adc), converterType(converterType), channel(channel), mux(mux),
                                    enableDifferentialPair(differentialPair){ ; }

    void init() override;

    int32_t getValue() override;

    void startConversion() override;

};