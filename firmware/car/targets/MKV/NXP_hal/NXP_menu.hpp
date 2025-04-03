
#include "HALina_buttons.hpp"
#include "HALina_switches.hpp"
#include "NXP_display.hpp"
#include "NXP_motor.hpp"
#include  <algorithms/motor.hpp>

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
    Differential& differential;

    bool menuTrigger = false;

  public:
    NXP_Menu(halina::Buttons& buttons, halina::Switches& switches, NXP_Display& display, NXP_Motors& motors, Differential& differential) : buttons(buttons), switches(switches), display(display), motors(motors) , differential(differential) {}

    void init() {
        buttons.init();
        display.print(0001);
        initialized = true;
    }

    bool proc();

    void displayMenuPage();

    void startRace();
    
    bool isTriggeredOff() {
      return menuTrigger;
    }

    void setTriggeredOff(bool state) {
      menuTrigger = state;
      motors.block();
      display.enable();
    }
};