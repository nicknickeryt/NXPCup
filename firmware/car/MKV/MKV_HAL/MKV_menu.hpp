
#include "HALina_buttons.hpp"
#include "HALina_switches.hpp"
#include "MKV_display.hpp"
#include "MKV_motor.hpp"

#include <cstdint>
#include <iomanip>
#include <sstream>

#pragma once

class MKV_Menu {
  private:
    bool    initialized = false;
    uint8_t currPage    = 0; // 0-> toggle start/stop page; 1-> change speed page
    uint8_t maxPage     = 1;
    uint8_t startPage   = 0;
    uint8_t speedPage   = 1;

    halina::Buttons&  buttons;
    halina::Switches& switches;
    MKV_Display&      display;
    MKV_Motors&       motors;

  public:
    MKV_Menu(halina::Buttons& buttons, halina::Switches& switches, MKV_Display& display, MKV_Motors& motors) : buttons(buttons), switches(switches), display(display), motors(motors) {}

    void init() { initialized = true; }

    void proc();

    void nextPage();
    void prevPage();

    void displayMenuPage(uint8_t page);

    void toggleMotors();
};