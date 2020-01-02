#ifndef PROJECT_NAME_HALINA_KITTY_HPP
#define PROJECT_NAME_HALINA_KITTY_HPP

#include "HALina.hpp"
#include "LEDLine.hpp"
struct KITTY {
    LEDLine& LED_line;

    KITTY(LEDLine& LED_line) : LED_line(LED_line) {}
};

KITTY& kitty();

#endif //PROJECT_NAME_HALINA_KITTY_HPP
