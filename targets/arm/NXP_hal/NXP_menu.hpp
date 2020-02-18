/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 * NXP HALina implementation of menu
 *
 */

#include "HALina_switches.hpp"
#include "HALina_buttons.hpp"
#include "NXP_display.hpp"

#pragma once


class NXP_Menu {

    enum class state{
        SET_SPEED,
        START
    }currentState;

    halina::Buttons& buttons;
    halina::Switches& switches;
    NXP_Display& display;

private:
    static bool button0Flag;
    static bool button1Flag;
    static bool button2Flag;
    static bool button3Flag;

public:
    static void button0InterruptHandler(){ button0Flag = true; };
    static void button1InterruptHandler(){ button1Flag = true; };
    static void button2InterruptHandler(){ button2Flag = true; };
    static void button3InterruptHandler(){ button3Flag = true; };

    NXP_Menu(halina::Buttons& buttons, halina::Switches& switches, NXP_Display &display) : buttons(buttons), switches(switches), display(display){}

    void init();

    void proc();
};
