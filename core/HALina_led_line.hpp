/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 * HALina led Line file
 *
 */
#pragma once

#include "HALina.hpp"
#include <gsl/gsl>

namespace halina {

    class  LedLine {
        halina::GPIO &LED0;
        halina::GPIO &LED1;
        halina::GPIO &LED2;
        halina::GPIO &LED3;
        halina::GPIO &LED4;
        halina::GPIO &LED5;
        halina::GPIO &LED6;
        halina::GPIO &LED7;

        halina::GPIO *LED[8] = {&LED0, &LED1, &LED2, &LED3, &LED4, &LED5, &LED6, &LED7};

    public:
        LedLine(halina::GPIO &LED0, halina::GPIO &LED1, halina::GPIO &LED2, halina::GPIO &LED3, halina::GPIO &LED4,
                        halina::GPIO &LED5, halina::GPIO &LED6, halina::GPIO &LED7) :
                LED0(LED0), LED1(LED1), LED2(LED2), LED3(LED3), LED4(LED4), LED5(LED5), LED6(LED6), LED7(LED7) {}

        void init() {
            for (int i = 0; i < 8; i++) {
                LED[i]->init();
            }
        }

        halina::GPIO &at(uint8_t i) {
            if (i >= 8) {
                return *LED[0];
            }
            return *LED[i];
        }
    };
}
