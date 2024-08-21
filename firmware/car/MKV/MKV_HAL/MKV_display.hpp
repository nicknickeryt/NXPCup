/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 * MKV HALina implementation of 7-segment display driver
 *
 */

#pragma once

#include <HALina_display.hpp>
#include "HALina.hpp"
#include "halina_config.hpp"

class MKV_Display : public HALina_Display{
private:
    bool displayEnable = false;
public:
    void update() override;
    void updateISR(uint32_t prescaler);
    void init() override;
    void enable(){displayEnable = true;}
    void disable(){displayEnable = false; segmentsOff();}

    MKV_Display() {
        SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;
    }

    enum class SEGMENT : uint32_t {
        A = DISPLAY_A,
        B = DISPLAY_B,
        C = DISPLAY_C,
        D = DISPLAY_D,
        E = DISPLAY_E,
        F = DISPLAY_F,
        G = DISPLAY_G,
        DP = DISPLAY_DP
    };

    enum class DIGIT : uint32_t {
        A1 = DISPLAY_A1,
        A2 = DISPLAY_A2,
        A3 = DISPLAY_A3,
        A4 = DISPLAY_A4
    };


    static void digitOn(DIGIT digit) {
        DISPLAY_GPIO->PCOR = static_cast<uint32_t>(digit);
    }

    static void digitOff(DIGIT digit) {
        DISPLAY_GPIO->PSOR = static_cast<uint32_t>(digit);
    }

    static void digitToggle(DIGIT digit) {
        DISPLAY_GPIO->PTOR = static_cast<uint32_t>(digit);
    }

    static void segmentOn(SEGMENT segment) {
        DISPLAY_GPIO->PCOR = static_cast<uint32_t >(segment);
    }

    static void segmentOff(SEGMENT segment) {
        DISPLAY_GPIO->PSOR = static_cast<uint32_t >(segment);
    }

    static void segmentToggle(SEGMENT segment) {
        DISPLAY_GPIO->PTOR = static_cast<uint32_t >(segment);
    }

    static void digitsOff() {
        constexpr uint32_t off = static_cast<uint32_t >(DIGIT::A1)
                                 | static_cast<uint32_t >(DIGIT::A2)
                                 | static_cast<uint32_t >(DIGIT::A3)
                                 | static_cast<uint32_t >(DIGIT::A4);
        DISPLAY_GPIO->PSOR = off;
    }

    static void segmentsOff() {
        constexpr uint32_t off = static_cast<uint32_t >(SEGMENT::A)
                                 | static_cast<uint32_t >(SEGMENT::B)
                                 | static_cast<uint32_t >(SEGMENT::C)
                                 | static_cast<uint32_t >(SEGMENT::D)
                                 | static_cast<uint32_t >(SEGMENT::E)
                                 | static_cast<uint32_t >(SEGMENT::F)
                                 | static_cast<uint32_t >(SEGMENT::G)
                                 | static_cast<uint32_t >(SEGMENT::DP);
        DISPLAY_GPIO->PSOR = off;
    };
};