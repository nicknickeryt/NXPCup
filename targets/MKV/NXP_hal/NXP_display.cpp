/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 * NXP HALina implementation of 7-segment display driver
 *
 */

#include "NXP_display.hpp"

uint32_t displayCharacters [] {
    uint32_t(DISPLAY_A | DISPLAY_B | DISPLAY_C | DISPLAY_D | DISPLAY_E | DISPLAY_F),
    uint32_t(DISPLAY_B | DISPLAY_C),
    uint32_t(DISPLAY_A | DISPLAY_B | DISPLAY_E | DISPLAY_D | DISPLAY_G),
    uint32_t(DISPLAY_A | DISPLAY_B | DISPLAY_C | DISPLAY_D | DISPLAY_G),
    uint32_t(DISPLAY_B | DISPLAY_C | DISPLAY_F | DISPLAY_G),
    uint32_t(DISPLAY_A | DISPLAY_C | DISPLAY_D | DISPLAY_F | DISPLAY_G),
    uint32_t(DISPLAY_A | DISPLAY_C | DISPLAY_D | DISPLAY_E | DISPLAY_F | DISPLAY_G),
    uint32_t(DISPLAY_A | DISPLAY_B | DISPLAY_C),
    uint32_t(DISPLAY_A | DISPLAY_B | DISPLAY_C | DISPLAY_D | DISPLAY_E | DISPLAY_F | DISPLAY_G),
    uint32_t(DISPLAY_A | DISPLAY_B | DISPLAY_C | DISPLAY_D | DISPLAY_F | DISPLAY_G),
    uint32_t(DISPLAY_G)
};

void NXP_Display::init() {
    DISPLAY_PORT->PCR[0] = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;
    DISPLAY_PORT->PCR[1] = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;
    DISPLAY_PORT->PCR[2] = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;
    DISPLAY_PORT->PCR[5] = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;
    DISPLAY_PORT->PCR[8] = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;
    DISPLAY_PORT->PCR[9] = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;
    DISPLAY_PORT->PCR[10] = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;
    DISPLAY_PORT->PCR[11] = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;
    DISPLAY_PORT->PCR[12] = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;
    DISPLAY_PORT->PCR[13] = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;
    DISPLAY_PORT->PCR[14] = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;
    DISPLAY_PORT->PCR[15] = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;

    DISPLAY_GPIO->PDDR |= DISPLAY_A1 | DISPLAY_A2 | DISPLAY_A3 | DISPLAY_A4;
    DISPLAY_GPIO->PDDR |= DISPLAY_A | DISPLAY_B | DISPLAY_C | DISPLAY_D;
    DISPLAY_GPIO->PDDR |= DISPLAY_E | DISPLAY_F | DISPLAY_G | DISPLAY_DP;

    digitsOff();
    segmentsOff();
}

void NXP_Display::update() {
    static int refreshCounter;

    // y = SYSTICK_RATE_HZ/REFRESH_RATE_HZ
    // x = (y/4)-1
    // |A1______A2______A3______A4______|
    //  __<---->__<---->__<---->__<---->
    //      x       x       x       x
    // <------------------------------->
    // 				y
    if(displayEnable) {
        if (refreshCounter++ < (((SYSTICK_RATE_HZ / REFRESH_RATE_HZ) / 4) - 1)) {
            return;
        } else {
            refreshCounter = 0;
        }

        updateISR(1);
    }else{
        segmentsOff();
    }
}

void NXP_Display::updateISR(uint32_t prescaler){
    static uint32_t refreshCnt;
    static uint32_t bufferCounter;
    if(displayEnable) {
        if (prescaler <= refreshCnt++) {
            digitsOff();
            segmentsOff();
            if (displayBuffer[bufferCounter] == static_cast<uint8_t>(HALina_Display::SPECIAL_CHARACTERS::DASH)) {
                DISPLAY_GPIO->PCOR = displayCharacters[10];
            } else if (displayBuffer[bufferCounter] ==
                       static_cast<uint8_t>(HALina_Display::SPECIAL_CHARACTERS::SPACE)) {
                digitsOff();
            } else {
                uint8_t character = displayBuffer[bufferCounter];
                if (character <= 9) {
                    if (dot[bufferCounter]) {
                        DISPLAY_GPIO->PCOR = displayCharacters[character] | static_cast<uint32_t >(SEGMENT::DP);
                    } else {
                        DISPLAY_GPIO->PCOR = displayCharacters[character];
                    }
                }
            }


            switch (bufferCounter) {
                case 0:
                    digitOn(DIGIT::A1);
                    bufferCounter++;;
                    break;
                case 1:
                    digitOn(DIGIT::A2);
                    bufferCounter++;
                    break;
                case 2:
                    digitOn(DIGIT::A3);
                    bufferCounter++;
                    break;
                case 3:
                    digitOn(DIGIT::A4);
                    bufferCounter = 0;
                    break;
            }
            refreshCnt = 0;
        }
    }else{
        segmentsOff();
    }
}