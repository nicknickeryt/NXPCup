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

    void proc(){
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
                at(led_index).set();
                at(old_led).reset();
            } else if (direction == 1) {
                old_led = led_index;
                led_index--;
                if (led_index == -1) {
                    led_index = 1;
                    direction = 0;
                    old_led = 0;
                }
                at(led_index).set();
                at(old_led).reset();
            }
        }
    }
    };
}
