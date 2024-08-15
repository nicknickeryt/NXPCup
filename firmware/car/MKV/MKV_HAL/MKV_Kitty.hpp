#pragma once

#include "MKV_gpio.hpp"
#include "HALina_led_line.hpp"

class Kitty {
  private:

    // LEDS
    MKV_GPIO LED0 = MKV_GPIO(PORTA, GPIOA, 16U);
    MKV_GPIO LED1 = MKV_GPIO(PORTA, GPIOA, 17U);
    MKV_GPIO LED2 = MKV_GPIO(PORTA, GPIOA, 24U);
    MKV_GPIO LED3 = MKV_GPIO(PORTA, GPIOA, 25U);
    MKV_GPIO LED4 = MKV_GPIO(PORTA, GPIOA, 26U);
    MKV_GPIO LED5 = MKV_GPIO(PORTA, GPIOA, 27U);
    MKV_GPIO LED6 = MKV_GPIO(PORTA, GPIOA, 28U);
    MKV_GPIO LED7 = MKV_GPIO(PORTA, GPIOA, 29U);

    // LEDLINE
    halina::LedLine ledLine = {LED0, LED1, LED2, LED3, LED4, LED5, LED6, LED7};

    Kitty() = default;

  public:
    void init();

    static Kitty& kitty() {
        static Kitty staticKitty;
        return staticKitty;
    }
};