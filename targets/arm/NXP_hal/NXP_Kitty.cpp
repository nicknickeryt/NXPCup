/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 * Main Kitty class
 *
 */

#include "NXP_Kitty.hpp"

void Kitty::init() {
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();

    uart.init();
    ledLine.init();
    display.init();
}

void Kitty::proc() {
    uart.proc();
    magicDiodComposition();
    display.update();
}

void Kitty::magicDiodComposition(){
    static uint32_t licznik = 0;
    static int8_t led_index = 0;
    static uint8_t direction = 0;
    static uint8_t old_led = 0;
    licznik++;
    if (licznik == 90000) {
        licznik = 0;
        if (direction == 0) {
            old_led = led_index;
            led_index++;
            if (led_index == 8) {
                led_index = 6;
                direction = 1;
                old_led = 7;
            }
            ledLine.at(led_index).set();
            ledLine.at(old_led).reset();
        } else if (direction == 1) {
            old_led = led_index;
            led_index--;
            if (led_index == -1) {
                led_index = 1;
                direction = 0;
                old_led = 0;
            }
            ledLine.at(led_index).set();
            ledLine.at(old_led).reset();
        }
    }
}