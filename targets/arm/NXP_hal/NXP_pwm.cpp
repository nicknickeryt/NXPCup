/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 * NXP HALina implementation of pwm driver
 *
 */

#include "fsl_pwm.h"
#include "NXP_pwm.hpp"
#include "NXP_Kitty.hpp"

void NXP_PWM::setDutyCycle(float dutyCycle, uint8_t channel){
    dutyCycle = std::clamp(dutyCycle, 0.0f, 1.0f);

    float period = dutyCycle * modulo;
    ftm->CONTROLS[channel].CnV = FTM_CnV_VAL((uint16_t)period);
}

void NXP_PWM::init() {


    ftm->MODE = (FTM_MODE_FAULTM(0x00) | FTM_MODE_WPDIS_MASK);
    if (portFirst.checkPort()) {
        ftm->CONTROLS[channelFirst].CnSC = FTM_CnSC_MSB_MASK | FTM_CnSC_ELSB_MASK;
        portFirst.setMux();
    }
    if (portSecond.checkPort()) {
        ftm->CONTROLS[channelSecond].CnSC = FTM_CnSC_MSB_MASK | FTM_CnSC_ELSB_MASK;
        portSecond.setMux();
    }

    ftm->SC = FTM_SC_CLKS(0x01); // System clock
    uint32_t period = CLOCK_GetFreq(kCLOCK_FastPeriphClk) / frequency;

    for (uint8_t i = 0; i < uint8_t(sizeof(dividers)); i++) {
        uint32_t mod = 0;
        mod = period / dividers[i];
        if (mod < std::numeric_limits<uint16_t>::max()) {
            dividerIndex = i;
            modulo = mod;
            break;
        }
    }
    
    if (ftm == FTM0) {
        SIM->SCGC6 |= SIM_SCGC6_FTM0_MASK;
    } else if (ftm == FTM1) {
        SIM->SCGC6 |= SIM_SCGC6_FTM1_MASK;
    } else if (ftm == FTM2) {
        SIM->SCGC6 |= SIM_SCGC6_FTM2_MASK;
    } else if (ftm == FTM3) {
        SIM->SCGC6 |= SIM_SCGC6_FTM3_MASK;
    }
    ftm->SC |= FTM_SC_PS(dividerIndex);
    ftm->CNT = 0;
    ftm->MOD = FTM_MOD_MOD(modulo);

    if (portFirst.checkPort()) {
        setDutyCycle(0.0f, channelFirst);
    }
    if (portSecond.checkPort()) {
        setDutyCycle(0.0f, channelSecond);
    }
}

//int32_t NXP_PWM::getDutyCycle(){
//    return dutyCycle;
//}

