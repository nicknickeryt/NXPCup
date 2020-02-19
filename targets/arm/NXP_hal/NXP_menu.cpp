/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 * NXP HALina implementation of menu
 *
*/

#define LOG_CHANNEL MENU
#define MENU_LOG_CHANNEL 2
#define MENU_LOG_CHANNEL_LEVEL LOG_LEVEL_DEBUG

#include "NXP_Kitty.hpp"
#include "logger.h"

void NXP_Menu::button0InterruptHandler(){Kitty::kitty().menu.stateMachine.handle(fsm::ChangeParameterDown{});};
void NXP_Menu::button1InterruptHandler(){Kitty::kitty().menu.stateMachine.handle(fsm::ChangeParameterUp{});};
void NXP_Menu::button2InterruptHandler(){Kitty::kitty().menu.stateMachine.handle(fsm::ValueDown{});};
void NXP_Menu::button3InterruptHandler(){Kitty::kitty().menu.stateMachine.handle(fsm::ValueUp{});};

void NXP_Menu::init() {
    buttons.init();
    switches.init();
    stateMachine.handle(fsm::StartMenu{});
    display.print("----");
}

bool NXP_Menu::proc(){
    display.update();
    return isMenuRunning;
}

namespace fsm {
    void Idle::on_entry(const StartMenu &) const {
        log_debug("Idle::on_entry::StartMenu");
        menu.display.print("----");
    }

    void Idle::on_entry(const EmergencyBreak &) const {
        log_debug("Idle::on_entry::EmergencyBreak");
        menu.display.print("----");

    }

    void Parameters::on_entry(ChangeParameterUp const&) {
        log_debug("Parameters::on_entry::ChangeParameter")
        parametersCounter++;
        log_debug("Current parameter: %d", parametersCounter);
    }

    void Parameters::on_entry(ChangeParameterDown const&) {
        log_debug("Parameters::on_entry::ChangeParameter")
        parametersCounter--;
        log_debug("Current parameter: %d", parametersCounter);
    }

    void Parameters::on_entry(ValueUp const&) const {
        log_debug("Parameters::on_entry::ValueUp");
    }

    void Parameters::on_entry(ValueDown const&) const {
        log_debug("Parameters::on_entry::ValueDown");
    }

    void Race::on_entry(const StartRace &) const {
        log_debug("Race::on_entry::StartRace");
        menu.isMenuRunning = false;
    }
}