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

void NXP_Menu::button0InterruptHandler(){Kitty::kitty().menu.stateMachine.handle(fsm::ChangeParameter{});};
void NXP_Menu::button1InterruptHandler(){};
void NXP_Menu::button2InterruptHandler(){};
void NXP_Menu::button3InterruptHandler(){};

void NXP_Menu::init() {
    buttons.init();
    switches.init();
    stateMachine.handle(fsm::StartMenu{});
}

void NXP_Menu::proc(){

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

    void Parameters::on_entry(ChangeParameter const&) {
        log_debug("Parameters::on_entry::ChangeParameter")
        parametersCounter++;
        if(parametersCounter >= parameters.size()){
            parametersCounter = 0;
        }
    }

    void Parameters::on_entry(ValueUp const&) const {
        log_debug("Parameters::on_entry::ValueUp");
        *parameters.at(parametersCounter)++;
    }

    void Parameters::on_entry(ValueDown const&) const {
        log_debug("Parameters::on_entry::ValueDown");
    }

    void Parameters::on_entry(AddParameter const& p) {
        log_debug("Parameters::on_entry::ValueDown");
        parameters.push_back(p.parameter);
    }

    void Race::on_entry(const StartRace &) const {
        log_debug("Race::on_entry::StartRace");
    }
}