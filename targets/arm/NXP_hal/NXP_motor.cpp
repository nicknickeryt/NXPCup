/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 * NXP HALina implementation of motor driver
 *
*/

#include "NXP_motor.hpp"

void NXP_RightMotor::init() {
    SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
    SIM->SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK;
    SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
    SIM->SCGC6 |= SIM_SCGC6_FTM3_MASK;

    PORTE->PCR[4] = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;
    GPIOE->PDDR |= motorEn;
    motorEnSetHigh();

    FTM3->MODE = (FTM_MODE_FAULTM(0x00) | FTM_MODE_WPDIS_MASK);
    FTM3->MOD = FTM_MOD_MOD(MOTOR_CLOCK_MOD-1);
    FTM3->CONTROLS[2].CnSC = FTM_CnSC_MSB_MASK | FTM_CnSC_ELSB_MASK;	//MRB
    FTM3->CONTROLS[3].CnSC = FTM_CnSC_MSB_MASK | FTM_CnSC_ELSB_MASK;	//MRF

    setValue(0);

    PORTE->PCR[5] = PORT_PCR_MUX(0x06);
    PORTE->PCR[6] = PORT_PCR_MUX(0x06);

    FTM3->SC = (FTM_SC_CLKS(0x01) | FTM_SC_PS(0x01));
}

void NXP_LeftMotor::init(){
    // clocks was started in Right motor

    PORTE->PCR[3] = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;
    GPIOE->PDDR |= motorFb;

    FTM3->CONTROLS[0].CnSC = FTM_CnSC_MSB_MASK | FTM_CnSC_ELSB_MASK;	//MLB
    FTM3->CONTROLS[1].CnSC = FTM_CnSC_MSB_MASK | FTM_CnSC_ELSB_MASK;	//MLF

    PORTE->PCR[7] = PORT_PCR_MUX(0x06);
    PORTE->PCR[8] = PORT_PCR_MUX(0x06);

    setValue(0);
}

void NXP_RightMotor::setValue(int32_t value) {
    static int32_t mean[NUMPR];
    for (uint8_t i =0; i < (NUMPR-1); i ++)
    {
        mean[i] = mean[i+1];
    }
    mean[NUMPR-1] = value;

    value = 0;
    for (uint8_t i =0; i < NUMPR; i ++)
    {
        value += mean[i];
    }
    value /= NUMPR;

    if (value > 0) {
        //MLB
        FTM3->CONTROLS[0].CnV = FTM_CnV_VAL(motorZeroRatio);
        if (value > motorMaxRatio) {
            //MLF
            FTM3->CONTROLS[1].CnV = FTM_CnV_VAL(motorMaxRatio);
        } else {
            //MLF
            FTM3->CONTROLS[1].CnV = FTM_CnV_VAL(value);
        }
    } else if (value < 0) {
        //MLF
        FTM3->CONTROLS[1].CnV = FTM_CnV_VAL(motorZeroRatio);
        if (value < motorMinRatio) {
            //MLB
            FTM3->CONTROLS[0].CnV = FTM_CnV_VAL(motorMaxRatio);
        } else {
            //MLB
            FTM3->CONTROLS[0].CnV = FTM_CnV_VAL(-value);
        }
    } else {
        //MLF
        FTM3->CONTROLS[1].CnV = FTM_CnV_VAL(motorZeroRatio);
        //MLB
        FTM3->CONTROLS[0].CnV = FTM_CnV_VAL(motorZeroRatio);
    }
}

void NXP_LeftMotor::setValue(int32_t value) {
    static int32_t mean[NUMPR];
    for (uint8_t i =0; i < (NUMPR-1); i ++)
    {
        mean[i] = mean[i+1];
    }
    mean[NUMPR-1] = value;

    value = 0;
    for (uint8_t i =0; i < NUMPR; i ++)
    {
        value += mean[i];
    }
    value /= NUMPR;
    if (value > 0) {
        //MRB
        FTM3->CONTROLS[2].CnV = FTM_CnV_VAL(motorZeroRatio);
        if (value > motorMaxRatio) {
            //MRF
            FTM3->CONTROLS[3].CnV = FTM_CnV_VAL(motorMaxRatio);
        } else {
            //MRF
            FTM3->CONTROLS[3].CnV = FTM_CnV_VAL(value);
        }
    } else if (value < 0) {
        //MRF
        FTM3->CONTROLS[3].CnV = FTM_CnV_VAL(motorZeroRatio);
        if (value < motorMinRatio) {
            //MRB
            FTM3->CONTROLS[2].CnV = FTM_CnV_VAL(motorMaxRatio);
        } else {
            //MRB
            FTM3->CONTROLS[2].CnV = FTM_CnV_VAL(-value);
        }
    } else {
        //MRF
        FTM3->CONTROLS[3].CnV = FTM_CnV_VAL(motorZeroRatio);
        //MRB
        FTM3->CONTROLS[2].CnV = FTM_CnV_VAL(motorZeroRatio);
    }
}

int32_t NXP_RightMotor::getValue() {
    //Motor stopped
    if (FTM3->CONTROLS[2].CnV == motorZeroRatio && FTM3->CONTROLS[3].CnV == motorZeroRatio) {
        return 0;
    }
    //Motor forward
    if(FTM3->CONTROLS[2].CnV == motorZeroRatio) {
        return (int32_t)FTM3->CONTROLS[3].CnV;
    }

    //Motor backward
    return -((int32_t)FTM3->CONTROLS[2].CnV);
}

int32_t NXP_LeftMotor::getValue() {
    //Motor stopped
    if (FTM3->CONTROLS[0].CnV == motorZeroRatio && FTM3->CONTROLS[1].CnV == motorZeroRatio) {
        return 0;
    }
    //Motor forward
    if(FTM3->CONTROLS[0].CnV == motorZeroRatio) {
        return (int32_t)FTM3->CONTROLS[1].CnV;
    }

    //Motor backward
    return -((int32_t)FTM3->CONTROLS[0].CnV);
}