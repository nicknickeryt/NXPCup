/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 * NXP HALina implementation of adc
 *
 */

#include <drivers/fsl_hsadc.h>
#include "NXP_adc.hpp"

NXP_ADC* HSADCInstances[2];

NXP_ADC::NXP_ADC(HSADC_Type* baseAdc, void (*converterAHandler)(uint8_t), void (*converterBHandler)(uint8_t)) : baseAdc(baseAdc) {
        A.converterHandler = converterAHandler;
        B.converterHandler = converterBHandler;

        if (baseAdc == HSADC0) {
            HSADCInstances[0] = this;
        } else if (baseAdc == HSADC1) {
            HSADCInstances[1] = this;
        }
}

void NXP_ADC::init() {
    hsadc_config_t  hsadcConfig;
    HSADC_GetDefaultConfig(&hsadcConfig);
    hsadcConfig.enableSimultaneousMode = false;
    HSADC_Init(baseAdc, &hsadcConfig);

    for (uint8_t i = 0; i < A.samplesCount; i++) {
        A.samples.at(i)->init(baseAdc, i);
    }

    for (uint8_t i = 0; i < B.samplesCount; i++) {
        B.samples.at(i)->init(baseAdc, i + 8);
    }

    HSADC_ClearStatusFlags(baseAdc,  kHSADC_ConverterBEndOfScanFlag | kHSADC_ConverterBEndOfCalibrationFlag);
    HSADC_EnableSampleResultReadyInterrupts(baseAdc, HSADC_SAMPLE_MASK(sampleMask), true);

    if (baseAdc == HSADC0 && A.samplesCount > 0) {
        SIM->SCGC5 |= SIM_SCGC5_HSADC0_MASK;
        A.init(baseAdc);
        NVIC_ClearPendingIRQ(HSADC0_CCA_IRQn);
        NVIC_EnableIRQ(HSADC0_CCA_IRQn);
        HSADC_EnableInterrupts(baseAdc, kHSADC_ConverterAEndOfScanInterruptEnable);
    } else if (baseAdc == HSADC0 && B.samplesCount > 0) {
        SIM->SCGC5 |= SIM_SCGC5_HSADC0_MASK;
        B.init(baseAdc);
        NVIC_ClearPendingIRQ(HSADC0_CCB_IRQn);
        NVIC_EnableIRQ(HSADC0_CCB_IRQn);
        HSADC_EnableInterrupts(baseAdc, kHSADC_ConverterBEndOfScanInterruptEnable);
    } else if (baseAdc == HSADC1 && A.samplesCount > 0) {
        SIM->SCGC2 |= SIM_SCGC2_HSADC1_MASK;
        A.init(baseAdc);
        NVIC_ClearPendingIRQ(HSADC1_CCA_IRQn);
        NVIC_EnableIRQ(HSADC1_CCA_IRQn);
        HSADC_EnableInterrupts(baseAdc, kHSADC_ConverterAEndOfScanInterruptEnable);
    } else if (baseAdc == HSADC1 && B.samplesCount > 0) {
        SIM->SCGC2 |= SIM_SCGC2_HSADC1_MASK;
        B.init(baseAdc);
        NVIC_ClearPendingIRQ(HSADC1_CCB_IRQn);
        NVIC_EnableIRQ(HSADC1_CCB_IRQn);
        HSADC_EnableInterrupts(baseAdc, kHSADC_ConverterBEndOfScanInterruptEnable);
    }
}

void NXP_ADC::Converter::init(HSADC_Type* baseAdc)  {
    hsadc_converter_config_t  converterConfig;
    HSADC_GetDefaultConverterConfig(&converterConfig);
    HSADC_SetConverterConfig(baseAdc, (_hsadc_converter_id)type , &converterConfig);

    HSADC_EnableConverterPower(baseAdc, (_hsadc_converter_id)type, true);
    if (type == Converter::Type::A) {
        while ((kHSADC_ConverterAPowerDownFlag) == ((kHSADC_ConverterAPowerDownFlag) & HSADC_GetStatusFlags(baseAdc))) { ; }
    } else if (type == Converter::Type::B) {
        while ((kHSADC_ConverterBPowerDownFlag) == ((kHSADC_ConverterBPowerDownFlag) & HSADC_GetStatusFlags(baseAdc))) { ; }
    }
}

bool NXP_ADC::Converter::appendSample(Sample *sample) {
    if (samplesCount < samples.size()) {
        samples.at(samplesCount) = sample;
        samplesCount++;
        return true;
    }
    return false;
}

bool NXP_ADC::appendSample(Sample *sample) {
    if (sample->converterType == Converter::Type::A) {
        A.appendSample(sample);
        return true;
    } else if (sample->converterType == Converter::Type::B) {
        B.appendSample(sample);
        return true;
    }
    return false;
}

void NXP_ADC::saveData(Converter::Type converterType) {
    if (converterType == Converter::Type::A) {
        for(uint8_t i = 0; i < 8; i ++) {
            A.conversionValues[i] = HSADC_GetSampleResultValue(baseAdc, i);
        }
        A.converterHandler(0);
    } else if (converterType == Converter::Type::B) {
        for(uint8_t i = 0; i < 8; i ++) {
            B.conversionValues[i] = HSADC_GetSampleResultValue(baseAdc, i + 8);
        }
        B.converterHandler(0);
    }
}

uint16_t* NXP_ADC::getBufferValues(Converter::Type converterType){
    if (converterType == Converter::Type::A) {
        return A.conversionValues;
    } else if (converterType == Converter::Type::B) {
        return B.conversionValues;
    }
    return {};
}

void  NXP_ADC::startConversion() {
    if (A.samplesCount > 0) {
        HSADC_DoSoftwareTriggerConverter(baseAdc, kHSADC_ConverterA);
    } else if (B.samplesCount > 0) {
        HSADC_DoSoftwareTriggerConverter(baseAdc, kHSADC_ConverterB);
    }
}

extern "C" {
void HSADC_ERR_IRQHandler() {
    HSADC0->STAT |= HSADC_STAT_EOCALIA_MASK;
}

void HSADC0_CCA_IRQHandler() {
    HSADC0->STAT |= HSADC_STAT_EOSIB_MASK;
    HSADCInstances[0]->saveData(NXP_ADC::Converter::Type::A);
}

void HSADC0_CCB_IRQHandler() {
    HSADC0->STAT |= HSADC_STAT_EOSIB_MASK;
    HSADCInstances[0]->saveData(NXP_ADC::Converter::Type::B);
}

void HSADC1_CCA_IRQHandler() {
    HSADC0->STAT |= HSADC_STAT_EOSIB_MASK;
    HSADCInstances[1]->saveData(NXP_ADC::Converter::Type::A);
}

void HSADC1_CCB_IRQHandler() {
    HSADC0->STAT |= HSADC_STAT_EOSIB_MASK;
    HSADCInstances[1]->saveData(NXP_ADC::Converter::Type::B);
}
}

void NXP_ADC::Sample::init(HSADC_Type* baseAdc, uint8_t indexInRegisterSample) {
    if (!initialised) {
        pinMux.setMux();
        initialised = true;
    }

    hsadc_sample_config_t sampleConfig;
    HSADC_GetDefaultSampleConfig(&sampleConfig);
    HSADC_EnableConverter(baseAdc, (_hsadc_converter_id)converterType, true);

    sampleConfig.channelNumber = static_cast<uint8_t>(channel) >> 4u;
    sampleConfig.channel67MuxNumber = static_cast<uint8_t>(channel) & 0xFFu;

    HSADC_SetSampleConfig(baseAdc, indexInRegisterSample, &sampleConfig);
    HSADC_EnableSample(baseAdc, HSADC_SAMPLE_MASK(indexInRegisterSample), true);
}