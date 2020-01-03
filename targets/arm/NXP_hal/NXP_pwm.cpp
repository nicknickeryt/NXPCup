/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 * NXP HALina implementation of pwm driver
 *
 */

#include "fsl_pwm.h"
#include "NXP_pwm.hpp"

void NXP_PWM::setDutyCycle(int32_t value){
    dutyCycle = value;
    if (portFirst.checkPort()) {
        ftm->CONTROLS[channelFirst].CnV = FTM_CnV_VAL(dutyCycle);
    }
    if (portFirst.checkPort()) {
        ftm->CONTROLS[channelSecond].CnV = FTM_CnV_VAL(dutyCycle);
    }
}

void NXP_PWM::init() {
    ftm->MODE = (FTM_MODE_FAULTM(0x00) | FTM_MODE_WPDIS_MASK);
    ftm->MOD = FTM_MOD_MOD(clockPrescaler - 1);

    if (portFirst.checkPort()) {
        ftm->CONTROLS[channelFirst].CnSC = FTM_CnSC_MSB_MASK | FTM_CnSC_ELSB_MASK;
        portFirst.setMux();
    }
    if (portSecond.checkPort()) {
        portSecond.setMux();
        ftm->CONTROLS[channelSecond].CnSC = FTM_CnSC_MSB_MASK | FTM_CnSC_ELSB_MASK;
    }

    setDutyCycle(0);

    ftm->SC = (FTM_SC_CLKS(0x01) | FTM_SC_PS(0x06) | FTM_SC_TOIE_MASK);
}

int32_t NXP_PWM::getDutyCycle(){
    return dutyCycle;
}

