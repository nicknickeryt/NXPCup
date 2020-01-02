#ifndef PROJECT_NAME_KITTY_H
#define PROJECT_NAME_KITTY_H

#include "hal.h"
#include "LEDLine.h"
struct KITTY {
//    hal::GPIO& LED1;
//    hal::GPIO& LED2;

    LEDLine& LED_line;


    KITTY(LEDLine& LED_line) : LED_line(LED_line) {}
};

KITTY& kitty();

#endif //PROJECT_NAME_KITTY_H
