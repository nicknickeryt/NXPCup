/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 * NXP HALina implementation of 7-segment display driver
 *
 */

#include "NXP_display.hpp"

void NXP_Display::init(void) {
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

    signsOff();
    segmentsOff();
}

void NXP_Display::print(const char* data) {
    memset(displayBuffer, 0, 4);
    for(auto i = 0; i < 4; i++){
        if(data[i] == '-' ||
           data[i] == '1' ||
           data[i] == '2' ||
           data[i] == '3' ||
           data[i] == '4' ||
           data[i] == '5' ||
           data[i] == '6' ||
           data[i] == '7' ||
           data[i] == '8' ||
           data[i] == '9' ||
           data[i] == '0'){
        }else{
            return;
        }
    }
    for(auto i = 0; i < 4; i++){
        displayBuffer[i] = data[i];
    }
}

void NXP_Display::print(uint32_t data) {
    memset(displayBuffer, 0, 4);
    if(data < 10000) {
        displayBuffer[0] = (data / 1000) + 48;
        data -= ((data / 1000) * 1000);
        displayBuffer[1] = (data / 100) + 48;
        data -= ((data / 100) * 100);
        displayBuffer[2] = data / 10 + 48;
        data -= ((data / 10) * 10);
        displayBuffer[3] = data + 48;
    }
}

void NXP_Display::update() noexcept{
    static int refreshCounter;
    static int bufferCounter;

    // y = SYSTICK_RATE_HZ/REFRESH_RATE_HZ
    // x = (y/4)-1
    // |A1______A2______A3______A4______|
    //  __<---->__<---->__<---->__<---->
    //      x       x       x       x
    // <------------------------------->
    // 				y

    if(refreshCounter++ <  (((SYSTICK_RATE_HZ / REFRESH_RATE_HZ) / 4) - 1)) {
        return;
    } else {
        refreshCounter = 0;
    }
    signsOff();
    segmentsOff();

    switch (displayBuffer[bufferCounter]) {
        case '0':
            DISPLAY_GPIO->PCOR = DISPLAY_A | DISPLAY_B | DISPLAY_C | DISPLAY_D | DISPLAY_E | DISPLAY_F;
            break;
        case '1':
            DISPLAY_GPIO->PCOR = DISPLAY_B | DISPLAY_C;
            break;
        case '2':
            DISPLAY_GPIO->PCOR = DISPLAY_A | DISPLAY_B | DISPLAY_E | DISPLAY_D | DISPLAY_G;
            break;
        case '3':
            DISPLAY_GPIO->PCOR = DISPLAY_A | DISPLAY_B | DISPLAY_C | DISPLAY_D | DISPLAY_G;
            break;
        case '4':
            DISPLAY_GPIO->PCOR = DISPLAY_B | DISPLAY_C | DISPLAY_F | DISPLAY_G;
            break;
        case '5':
            DISPLAY_GPIO->PCOR = DISPLAY_A | DISPLAY_C | DISPLAY_D | DISPLAY_F | DISPLAY_G;
            break;
        case '6':
            DISPLAY_GPIO->PCOR = DISPLAY_A | DISPLAY_C | DISPLAY_D | DISPLAY_E | DISPLAY_F | DISPLAY_G;
            break;
        case '7':
            DISPLAY_GPIO->PCOR = DISPLAY_A | DISPLAY_B | DISPLAY_C;
            break;
        case '8':
            DISPLAY_GPIO->PCOR = DISPLAY_A | DISPLAY_B | DISPLAY_C | DISPLAY_D | DISPLAY_E | DISPLAY_F | DISPLAY_G;
            break;
        case '9':
            DISPLAY_GPIO->PCOR = DISPLAY_A | DISPLAY_B | DISPLAY_C | DISPLAY_D | DISPLAY_F | DISPLAY_G;
            break;
        case '-':
            DISPLAY_GPIO->PCOR = DISPLAY_G;
            break;
        default:
            break;
    }

    switch (bufferCounter) {
        case 0:
            A1_On();
            bufferCounter++;;
            break;
        case 1:
            A2_On();
            bufferCounter++;
            break;
        case 2:
            A3_On();
            bufferCounter++;
            break;
        case 3:
            A4_On();
            bufferCounter = 0;
            break;
    }
}