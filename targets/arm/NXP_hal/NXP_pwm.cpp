/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 * NXP HALina implementation of pwm driver
 *
 */

#include "NXP_pwm.hpp"

void NXP_PWM::setDutyCycle(int32_t value){
    dutyCycle = value;
    FTM0->CONTROLS[4].CnV = FTM_CnV_VAL(dutyCycle);
}

void NXP_PWM::init(){
    FTM0->MODE = (FTM_MODE_FAULTM(0x00) | FTM_MODE_WPDIS_MASK);

    FTM0->MOD = FTM_MOD_MOD(clockMod-1);

    FTM0->CONTROLS[4].CnSC = FTM_CnSC_MSB_MASK | FTM_CnSC_ELSB_MASK;

    setDutyCycle(0);

    PORTA->PCR[7] = PORT_PCR_MUX(0x03);

    FTM0->SC = (FTM_SC_CLKS(0x01) | FTM_SC_PS(0x06) | FTM_SC_TOIE_MASK);
}

int32_t NXP_PWM::getDutyCycle(){
    return dutyCycle;
}

