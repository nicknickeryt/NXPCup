#include "HALina_buttons.hpp"
#include "HALina_switches.hpp"
#include "NXP_display.hpp"
#include "NXP_motor.hpp"
#include "NXP_Params.hpp"

#include <algorithms/camera.hpp>
#include <algorithms/motor.hpp>

#include <cstdint>
#include <iomanip>
#include <sstream>

#pragma once

class NXP_Menu {
  private:
    bool initialized = false;

    halina::Buttons&  buttons;
    halina::Switches& switches;
    NXP_Display&      display;
    NXP_Motors&       motors;
    Differential&     differential;
    Algorithm&        algorithm;
    NXP_Params&      params;

    bool menuTrigger = false;

    enum MenuPage { PAGE_STARTRPM, PAGE_DIFFRATIO, PAGE_BREAK, PAGE_0RPM, PAGE_PID_KP };

    enum MenuButton { BUTTON_PAGE, BUTTON_VALUE_PLUS, BUTTON_VALUE_MINUS, BUTTON_RUN };

    MenuPage currentMenuPage = PAGE_STARTRPM;

    int      menuLabelTimer      = 0;
    uint32_t menuLabelShowTimeMs = 800;

    int      menuDebounceTimer  = 0;
    uint32_t menuDebounceTimeMs = 175;

  public:
    NXP_Menu(halina::Buttons& buttons, halina::Switches& switches, NXP_Display& display, NXP_Motors& motors, Differential& differential, Algorithm& algorithm, NXP_Params& params) :
        buttons(buttons), switches(switches), display(display), motors(motors), differential(differential), algorithm(algorithm), params(params) {}

    void init() {
        buttons.init();
        display.print("HEJA");
        initialized = true;
    }

    bool proc(uint32_t currentMillis);

    void startRace(uint32_t currentMillis);

    bool isTriggeredOff() { return menuTrigger; }

    void setTriggeredOff(bool state) {
        menuTrigger = state;
        motors.block();
        display.enable();
    }
};