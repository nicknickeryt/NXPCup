#pragma once

#include "MKV_gpio.hpp"
#include "HALina_led_line.hpp"

class Kitty {
  private:

    // LEDS
    MKV_GPIO LED0 = MKV_GPIO(PORTA, GPIOA, 29U);
    MKV_GPIO LED1 = MKV_GPIO(PORTA, GPIOA, 28U);
    MKV_GPIO LED2 = MKV_GPIO(PORTA, GPIOA, 27U);
    MKV_GPIO LED3 = MKV_GPIO(PORTA, GPIOA, 26U);
    MKV_GPIO LED4 = MKV_GPIO(PORTA, GPIOA, 25U);
    MKV_GPIO LED5 = MKV_GPIO(PORTA, GPIOA, 24U);
    MKV_GPIO LED6 = MKV_GPIO(PORTA, GPIOA, 17U);
    MKV_GPIO LED7 = MKV_GPIO(PORTA, GPIOA, 16U);

    halina::LedLine ledLine = {LED0, LED1, LED2, LED3, LED4, LED5, LED6, LED7};

    // SWITCHES
    MKV_GPIO SWITCH0 = {PORTB, GPIOB, 20U, MKV_GPIO::Mode::INPUT};
    MKV_GPIO SWITCH1 = {PORTB, GPIOB, 21U, MKV_GPIO::Mode::INPUT};
    MKV_GPIO SWITCH2 = {PORTB, GPIOB, 22U, MKV_GPIO::Mode::INPUT};
    MKV_GPIO SWITCH3 = {PORTB, GPIOB, 23U, MKV_GPIO::Mode::INPUT};

    halina::Switches switches = {SWITCH0, SWITCH1, SWITCH2, SWITCH3};

    // BUTTONS
    MKV_GPIO BUTTON0 = {PORTC, GPIOC, 16U, MKV_GPIO::Mode::INPUT};
    MKV_GPIO BUTTON1 = {PORTC, GPIOC, 17U, MKV_GPIO::Mode::INPUT};
    MKV_GPIO BUTTON2 = {PORTC, GPIOC, 18U, MKV_GPIO::Mode::INPUT};
    MKV_GPIO BUTTON3 = {PORTC, GPIOC, 19U, MKV_GPIO::Mode::INPUT};
    halina::Buttons buttons = {BUTTON0, BUTTON1, BUTTON2, BUTTON3};

    Kitty() = default;

  public:
    void init();

    static Kitty& kitty() {
        static Kitty staticKitty;
        return staticKitty;
    }
};