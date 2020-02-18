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
#include <Vector>

#pragma once

class NXP_Menu;
class Kitty;
namespace fsm{
    using namespace ::fsm;
    struct StartMenu{};
    struct ChangeParameter{};
    struct ValueUp{};
    struct ValueDown{};
    struct StartRace{};
    struct EmergencyBreak{};
    struct AddParameter{
        void* parameter;
    };

    struct Idle;
    struct Parameters;
    struct Race;

    struct Idle : public Transitions<On<ChangeParameter, Parameters>>{
    private:
        NXP_Menu& menu;
    public:
        Idle(NXP_Menu& menu) : menu(menu){}
        void on_entry(EmergencyBreak const&) const;
        void on_entry(StartMenu const&) const;
    };

    struct Parameters : public Transitions<On<ChangeParameter, Parameters>, On<AddParameter, Parameters>, On<StartRace, Race>>{
    private:
        NXP_Menu& menu;
        std::vector<void*> parameters;
        uint8_t parametersCounter = 0;
    public:
        Parameters(NXP_Menu& menu) : menu(menu){}
        void on_entry(ChangeParameter const&);
        void on_entry(ValueUp const&) const;
        void on_entry(ValueDown const&) const;
        void on_entry(AddParameter const&);
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

private:
    using StateMachine = fsm::StateMachine<fsm::Idle, fsm::Parameters, fsm::Race>;
    StateMachine stateMachine{*this, *this, *this};


public:
    static void button0InterruptHandler();
    static void button1InterruptHandler();
    static void button2InterruptHandler();
    static void button3InterruptHandler();

    NXP_Menu(halina::Buttons& buttons, halina::Switches& switches, NXP_Display &display) : buttons(buttons), switches(switches), display(display){}

    void init();

    void proc();

    void addParameter(void* parameter){stateMachine.handle(fsm::AddParameter{parameter});};
};
