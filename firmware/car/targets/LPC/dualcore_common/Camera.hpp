#pragma once

#include "NXP_SCU.hpp"

class Camera {
    NXP_SCU SIOD = {NXP_SCU::Type::PIN,
                    NXP_SCU::Function::_0,
                    (NXP_SCU::Mode_PULLUP | NXP_SCU::Mode_INBUFF_EN | NXP_SCU::Mode_ZIF_DIS | NXP_SCU::Mode_HIGHSPEEDSLEW_EN),
                    0, 0 };
    NXP_SCU SIOC = {NXP_SCU::Type::PIN,
                    NXP_SCU::Function::_0,
                    (NXP_SCU::Mode_INACT | NXP_SCU::Mode_INBUFF_EN | NXP_SCU::Mode_ZIF_DIS | NXP_SCU::Mode_HIGHSPEEDSLEW_EN),
                    0, 1 };
    NXP_SCU PWDN = {NXP_SCU::Type::PIN,
                    NXP_SCU::Function::_0,
                    (NXP_SCU::Mode_INACT | NXP_SCU::Mode_INBUFF_EN | NXP_SCU::Mode_ZIF_DIS | NXP_SCU::Mode_HIGHSPEEDSLEW_EN),
                    1, 15};
    NXP_SCU RSTB = {NXP_SCU::Type::PIN,
                    NXP_SCU::Function::_0,
                    (NXP_SCU::Mode_PULLUP | NXP_SCU::Mode_INBUFF_EN | NXP_SCU::Mode_ZIF_DIS | NXP_SCU::Mode_HIGHSPEEDSLEW_EN),
                    1, 16};
    NXP_SCU Y0 = {NXP_SCU::Type::PIN, NXP_SCU::Function::_0,
                  (NXP_SCU::Mode_INACT | NXP_SCU::Mode_INBUFF_EN | NXP_SCU::Mode_ZIF_DIS | NXP_SCU::Mode_HIGHSPEEDSLEW_EN),
                  1, 7};
    NXP_SCU Y1 = {NXP_SCU::Type::PIN,NXP_SCU::Function::_0,
                  (NXP_SCU::Mode_INACT | NXP_SCU::Mode_INBUFF_EN | NXP_SCU::Mode_ZIF_DIS | NXP_SCU::Mode_HIGHSPEEDSLEW_EN),
                  1, 8};
    NXP_SCU Y2 = {NXP_SCU::Type::PIN,NXP_SCU::Function::_0,
                  (NXP_SCU::Mode_INACT | NXP_SCU::Mode_INBUFF_EN | NXP_SCU::Mode_ZIF_DIS | NXP_SCU::Mode_HIGHSPEEDSLEW_EN),
                  1, 9 };
    NXP_SCU Y3 = {NXP_SCU::Type::PIN,NXP_SCU::Function::_0,
                  (NXP_SCU::Mode_INACT | NXP_SCU::Mode_INBUFF_EN | NXP_SCU::Mode_ZIF_DIS | NXP_SCU::Mode_HIGHSPEEDSLEW_EN),
                  1, 10};
    NXP_SCU Y4 = {NXP_SCU::Type::PIN,NXP_SCU::Function::_0,
                  (NXP_SCU::Mode_INACT | NXP_SCU::Mode_INBUFF_EN | NXP_SCU::Mode_ZIF_DIS | NXP_SCU::Mode_HIGHSPEEDSLEW_EN),
                  1, 11};
    NXP_SCU Y5 = {NXP_SCU::Type::PIN,NXP_SCU::Function::_0,
                  (NXP_SCU::Mode_INACT | NXP_SCU::Mode_INBUFF_EN | NXP_SCU::Mode_ZIF_DIS | NXP_SCU::Mode_HIGHSPEEDSLEW_EN),
                  1, 12};
    NXP_SCU Y6 = {NXP_SCU::Type::PIN,NXP_SCU::Function::_0,
                  (NXP_SCU::Mode_INACT | NXP_SCU::Mode_INBUFF_EN | NXP_SCU::Mode_ZIF_DIS | NXP_SCU::Mode_HIGHSPEEDSLEW_EN),
                  1, 13};
    NXP_SCU Y7 = {NXP_SCU::Type::PIN,NXP_SCU::Function::_0,
                  (NXP_SCU::Mode_INACT | NXP_SCU::Mode_INBUFF_EN | NXP_SCU::Mode_ZIF_DIS | NXP_SCU::Mode_HIGHSPEEDSLEW_EN),
                  1, 14};
    NXP_SCU HSYNC = {NXP_SCU::Type::PIN,NXP_SCU::Function::_0,
                     (NXP_SCU::Mode_INACT | NXP_SCU::Mode_INBUFF_EN | NXP_SCU::Mode_ZIF_DIS | NXP_SCU::Mode_HIGHSPEEDSLEW_EN),
                     1, 6};
    NXP_SCU VSYNC = {NXP_SCU::Type::PIN,NXP_SCU::Function::_0,
                     (NXP_SCU::Mode_INACT | NXP_SCU::Mode_INBUFF_EN | NXP_SCU::Mode_ZIF_DIS | NXP_SCU::Mode_HIGHSPEEDSLEW_EN),
                     2, 12};
    NXP_SCU PCLK = {NXP_SCU::Type::PIN,NXP_SCU::Function::_0,
                    (NXP_SCU::Mode_INACT | NXP_SCU::Mode_INBUFF_EN | NXP_SCU::Mode_ZIF_DIS | NXP_SCU::Mode_HIGHSPEEDSLEW_EN),
                    2, 13};

    NXP_SCU CLK = {NXP_SCU::Type::CLK_OUT, NXP_SCU::Function::_1,
                  (NXP_SCU::Mode_INACT | NXP_SCU::Mode_INBUFF_EN | NXP_SCU::Mode_ZIF_DIS | NXP_SCU::Mode_HIGHSPEEDSLEW_EN),
                  0 };

    NXP_GPIO red = {0x1, 0x1, halina::GPIO::Mode::OUTPUT};
    NXP_GPIO green = {0x1, 0x2, halina::GPIO::Mode::OUTPUT};
    NXP_GPIO blue = {0x2, 0x7, halina::GPIO::Mode::OUTPUT};

    NXP_GPIO white = {0x1, 0x10, halina::GPIO::Mode::OUTPUT};

//    NXP_GPIO button = {0x1, 0x10, halina::GPIO::Mode::INPUT};

public:
    void init() {
        SIOD.init();
        SIOC.init();
        PWDN.init();
        RSTB.init();

        Y0.init();
        Y1.init();
        Y2.init();
        Y3.init();
        Y4.init();
        Y5.init();
        Y6.init();
        Y7.init();
        HSYNC.init();
        VSYNC.init();
        PCLK.init();

        // CLK out to 17MHz
        Chip_Clock_SetDivider(CLK_IDIV_B, CLKIN_MAINPLL, 12);
        Chip_Clock_SetBaseClock(CLK_BASE_OUT, CLKIN_IDIVB, false, false);
        CLK.init();

        // LEDs
        red.init();
        green.init();
        blue.init();
        white.init();
    }
};