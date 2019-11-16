#ifndef PROJECT_NAME_LEDLINE_H
#define PROJECT_NAME_LEDLINE_H

#include "hal.h"
#include <gsl/gsl>

class LEDLine {
    hal::GPIO& LED0;
    hal::GPIO& LED1;
    hal::GPIO& LED2;
    hal::GPIO& LED3;
    hal::GPIO& LED4;
    hal::GPIO& LED5;
    hal::GPIO& LED6;
    hal::GPIO& LED7;

    hal::GPIO* LED [8] = {&LED0, &LED1, &LED2, &LED3, &LED4, &LED5, &LED6, &LED7};

public:
    LEDLine ( hal::GPIO& LED0, hal::GPIO& LED1, hal::GPIO& LED2, hal::GPIO& LED3, hal::GPIO& LED4, hal::GPIO& LED5, hal::GPIO& LED6, hal::GPIO& LED7) :
            LED0(LED0), LED1(LED1), LED2(LED2), LED3(LED3), LED4(LED4), LED5(LED5), LED6(LED6), LED7(LED7) { }

    void init() {
        for (int i =0; i < 8; i ++) {
            LED[i]->init();
        }
    }
    hal::GPIO& at(uint8_t i) {
        if (i >= 8) {
            return *LED[0];
        }
        return *LED[i];
    }
};

#endif //PROJECT_NAME_LEDLINE_H
