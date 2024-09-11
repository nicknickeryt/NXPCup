#include "MKV_menu.hpp"

void MKV_Menu::proc() {
    if (!initialized) return;

    // TODO: debounce (ig?)
    // TODO: better 'speed' handling, speed should be set and get in (wip) algorithm class, not in motors class.
    // Algorithm should have some kind of speed factor that controls the output speed from the algorithm.
    // Motors may also be set separately, for better turn control, for example 0.1 left and 1.0 right when turing left.

    // check button status
    // Button layout:
    // 0 - prev page
    // 1 - next page
    // 2 - value down
    // 3 - value up
    if (buttons.at(1).get()) nextPage();
    else if (buttons.at(0).get())
        prevPage();

    else if (buttons.at(3).get()) {
        if (currPage == startPage) toggleMotors();
        else if (currPage == speedPage)
            motors.setValue(motors.getValue().first + 0.1, motors.getValue().second + 0.1);

    } else if (buttons.at(2).get()) {
        if (currPage == startPage) toggleMotors();
        else if (currPage == speedPage)
            motors.setValue(motors.getValue().first - 0.1, motors.getValue().second - 0.1);
    }
    displayMenuPage(currPage);
}

void MKV_Menu::nextPage() { currPage == maxPage ? currPage = 0 : currPage++; }

void MKV_Menu::prevPage() { currPage == 0 ? currPage = maxPage : currPage--; }

void MKV_Menu::displayMenuPage(uint8_t page) {
    if (page == startPage) display.print(motors.enabled());
    else if (page == speedPage)
        display.print(motors.getValue().first, 3); // this should later be changed to algorithm-based control
}

void MKV_Menu::toggleMotors() {
    bool motorStatus = motors.enabled();
    motorStatus ? motors.block() : motors.run();
    motorStatus ? display.print("OFF") :  display.print("ON");
}
