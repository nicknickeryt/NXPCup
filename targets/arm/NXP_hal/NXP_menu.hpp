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
#include "state_machine.hpp"
#include <vector>
#include <variant>

#pragma once

class NXP_Menu;
class Kitty;
namespace fsm{
    using namespace ::fsm;
    struct StartMenu{};
    struct ChangeParameterUp{};
    struct ChangeParameterDown{};
    struct ValueUp{};
    struct ValueDown{};
    struct StartRace{};
    struct EmergencyBreak{};
    template<typename T>
    struct AddParameter{
        T parameter;
    };

    struct Idle;
    struct Parameters;
    struct Race;

    struct Idle : public Transitions<On<ChangeParameterUp, Parameters>, On<ChangeParameterDown, Parameters>>{
    private:
        NXP_Menu& menu;
    public:
        Idle(NXP_Menu& menu) : menu(menu){}
        void on_entry(EmergencyBreak const&) const;
        void on_entry(StartMenu const&) const;
    };

    struct Parameters : public Transitions<
            On<ChangeParameterUp, Parameters>,
            On<ChangeParameterDown, Parameters>,
            On<ValueUp, Parameters>,
            On<ValueDown, Parameters>,
            On<StartRace, Race>>{
    private:
        NXP_Menu& menu;
        uint8_t parametersCounter = 0;
    public:
        Parameters(NXP_Menu& menu) : menu(menu){}
        void on_entry(ChangeParameterUp const&);
        void on_entry(ChangeParameterDown const&);
        void on_entry(ValueUp const&) const;
        void on_entry(ValueDown const&) const;
    };

    struct Race : public Transitions<On<EmergencyBreak, Idle>>{
    private:
        NXP_Menu& menu;
    public:
        Race(NXP_Menu& menu) : menu(menu){}
        void on_entry(StartRace const&) const;
    };
}

class NXP_Menu {
    friend fsm::Parameters;
    friend fsm::Idle;
    friend fsm::Race;
    halina::Buttons& buttons;
    halina::Switches& switches;
    NXP_Display& display;
    bool isMenuRunning = true;
    std::vector<uint32_t*> &parameters;

private:
    using StateMachine = fsm::StateMachine<fsm::Idle, fsm::Parameters, fsm::Race>;
    StateMachine stateMachine{*this, *this, *this};


public:
    static void button0InterruptHandler();
    static void button1InterruptHandler();
    static void button2InterruptHandler();
    static void button3InterruptHandler();

    NXP_Menu(halina::Buttons& buttons, halina::Switches& switches, NXP_Display &display, std::vector<uint32_t*> &parameters) : buttons(buttons), switches(switches), display(display), parameters(parameters){}

    void init();

    bool proc();

};
