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
    struct ValueUp{};
    struct ValueDown{};
    struct StartRace{};
    struct EmergencyBreak{};
    struct Idle;
    struct Parameters;
    struct Race;

    struct Idle : public Transitions<On<ChangeParameterUp, Parameters>, On<StartRace, Race>>{
    private:
        NXP_Menu& menu;
    public:
        Idle(NXP_Menu& menu) : menu(menu){}
        void on_entry(EmergencyBreak const&) const;
        void on_entry(StartMenu const&) const;
    };

    struct Parameters : public Transitions<
            On<ChangeParameterUp, Parameters>,
            On<ValueUp, Parameters>,
            On<ValueDown, Parameters>,
            On<StartRace, Race>>{
    private:
        NXP_Menu& menu;
        int8_t parametersCounter = 0;
    public:
        Parameters(NXP_Menu& menu) : menu(menu){}
        void on_entry(ChangeParameterUp const&);
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
    constexpr static auto defaultDelay = 200.;
private:
    halina::Buttons& buttons;
    halina::Switches& switches;
    float buttonCounter = 0.;
    float buttonCompare = defaultDelay;
    NXP_Display& display;
    bool isMenuRunning = true;
    bool button1Pressed = false, button2Pressed = false, button3Pressed = false, button4Pressed = false;
public:
    struct MenuParameters {
        std::vector<std::variant<uint32_t *, uint16_t *, uint8_t *, float *, int32_t *, int16_t *, int8_t *, int *>> parameters;
        std::vector<float> divider;
    };
private:
    MenuParameters parameters;
    using StateMachine = fsm::StateMachine<fsm::Idle, fsm::Parameters, fsm::Race>;
    StateMachine stateMachine{*this, *this, *this};

public:
    NXP_Menu(halina::Buttons& buttons, halina::Switches& switches, NXP_Display &display) : buttons(buttons), switches(switches), display(display){}

    void init();

    bool proc(bool &systickTrigger);

    void addParameter(uint32_t* data, uint8_t divider = 1){ parameters.parameters.emplace_back(data); parameters.divider.emplace_back(divider);};
    void addParameter(uint16_t* data, uint8_t divider = 1){ parameters.parameters.emplace_back(data); parameters.divider.emplace_back(divider);};
    void addParameter(uint8_t* data, uint8_t divider = 1){ parameters.parameters.emplace_back(data); parameters.divider.emplace_back(divider);};
    void addParameter(int32_t* data, uint8_t divider = 1){ parameters.parameters.emplace_back(data); parameters.divider.emplace_back(divider);};
    void addParameter(int16_t* data, uint8_t divider = 1){ parameters.parameters.emplace_back(data); parameters.divider.emplace_back(divider);};
    void addParameter(int8_t* data, uint8_t divider = 1){ parameters.parameters.emplace_back(data); parameters.divider.emplace_back(divider);};
    void addParameter(float* data, float divider = 1){ parameters.parameters.emplace_back(data); parameters.divider.emplace_back(divider);};
    void addParameter(int* data, uint8_t divider = 1){ parameters.parameters.emplace_back(data); parameters.divider.emplace_back(divider);};

    static void buttonHandler();
};
