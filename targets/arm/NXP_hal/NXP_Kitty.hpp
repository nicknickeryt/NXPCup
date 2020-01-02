#pragma once

#include "NXP_hal.hpp"

//NXP_GPIO LED0(PORTA, GPIOA, 16U);
//NXP_GPIO LED1(PORTA, GPIOA, 17U);
//NXP_GPIO LED2(PORTA, GPIOA, 24U);
//NXP_GPIO LED3(PORTA, GPIOA, 25U);
//NXP_GPIO LED4(PORTA, GPIOA, 26U);
//NXP_GPIO LED5(PORTA, GPIOA, 27U);
//NXP_GPIO LED6(PORTA, GPIOA, 28U);
//NXP_GPIO LED7(PORTA, GPIOA, 29U);

class Kitty{
private:
    NXP_Uart uart;
private:
    Kitty() = default;

public:
    static Kitty& kitty() {
        static Kitty staticKitty;
        return staticKitty;
    }

    void init();

    void proc();

};