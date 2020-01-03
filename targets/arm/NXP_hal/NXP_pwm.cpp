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
    ftm->CONTROLS[4].CnV = FTM_CnV_VAL(dutyCycle);
}

void NXP_PWM::init(){
    ftm->MODE = (FTM_MODE_FAULTM(0x00) | FTM_MODE_WPDIS_MASK);
    ftm->MOD = FTM_MOD_MOD(clockMod-1);
    ftm->CONTROLS[4].CnSC = FTM_CnSC_MSB_MASK | FTM_CnSC_ELSB_MASK;
    setDutyCycle(0);
    port.set_mux();
    ftm->SC = (FTM_SC_CLKS(0x01) | FTM_SC_PS(0x06) | FTM_SC_TOIE_MASK);
}

int32_t NXP_PWM::getDutyCycle(){
    return dutyCycle;
}

