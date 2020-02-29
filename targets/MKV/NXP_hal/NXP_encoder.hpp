/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 * NXP Encoder driver header
 *
 */

#pragma once

#include "NXP_gpio.hpp"

class NXP_Encoder{
public:
    enum class Mode {
        SingleCounter,
        Normal
    };
private:
    FTM_Type* ftm;
    NXP_PORT& pinA;
    NXP_PORT& pinB;
    Mode mode;
    uint16_t lastTicksValue = 0;
public:
    NXP_Encoder(FTM_Type* ftm, NXP_PORT& pinA, NXP_PORT& pinB, Mode mode) : ftm(ftm), pinA(pinA), pinB(pinB), mode(mode) { }

    void init() {
        pinA.setMux();
        pinB.setMux();

        if (ftm == FTM0) {
            SIM->SCGC6 |= SIM_SCGC6_FTM0_MASK;
        } else if (ftm == FTM1) {
            SIM->SCGC6 |= SIM_SCGC6_FTM1_MASK;
        } else if (ftm == FTM2) {
            SIM->SCGC6 |= SIM_SCGC6_FTM2_MASK;
        } else if (ftm == FTM3) {
            SIM->SCGC6 |= SIM_SCGC6_FTM3_MASK;
        }

        ftm->MOD = 0xFFFF;
        ftm->CNTIN = 0;

        ftm->MODE |= FTM_MODE_WPDIS_MASK;
        ftm->MODE |= FTM_MODE_FTMEN_MASK;

        ftm->QDCTRL &= ~FTM_QDCTRL_QUADMODE_MASK;
        ftm->QDCTRL |= FTM_QDCTRL_QUADEN_MASK;

        if (mode == Mode::SingleCounter) {
            ftm->QDCTRL |= FTM_QDCTRL_QUADMODE_MASK;
        }
        ftm->SC |= FTM_SC_CLKS(3);
    }

    uint16_t getTicks() {
        return lastTicksValue;
    }

    void updateTicks() {
        lastTicksValue = ftm->CNT;
        ftm->CNT = 0;
    }

    static void ISR(uint32_t* arg) {
        auto encoder = (NXP_Encoder*) arg;
        encoder->updateTicks();
    }
};