#ifndef PROJECT_NAME_FROG_H
#define PROJECT_NAME_FROG_H

#include "hal.h"
#include "LEDLine.h"
struct FROGGY {
//    hal::GPIO& LED1;
//    hal::GPIO& LED2;

    LEDLine& LED_line;


    FROGGY(LEDLine& LED_line) : LED_line(LED_line) {}
};

FROGGY& froggy(void);

#endif //PROJECT_NAME_FROG_H
