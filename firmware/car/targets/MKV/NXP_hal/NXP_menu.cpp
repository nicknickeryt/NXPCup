#include "NXP_menu.hpp"

#include <string>

bool NXP_Menu::proc() {
    // when menu start page is fired, disable menu forever
    if (!initialized || menuTrigger) return false;


    else if (!buttons.at(0).get()) {
        differential.setStartVelocity(differential.getStartVelocity() + 0.05);
        delay_ms(300);

    } else if (!buttons.at(1).get()) {
        differential.setStartVelocity(differential.getStartVelocity() - 0.05);
        delay_ms(300);

    } else if (!buttons.at(3).get()) {
        startRace();
    }
    displayMenuPage();
    display.update();

    return true;
}

const char* formatString(float value) {
    std::string s = "  ";

    int intValue = static_cast<int>(value * 100.0f);
    if (intValue > 99) intValue = 99; 
    if (intValue < 0) intValue = 0;  

    s.append(std::to_string(intValue));

    return s.c_str();
}

void NXP_Menu::displayMenuPage() {
    display.print(formatString(differential.getStartVelocity())); // this is now algorithm-based control
}

void NXP_Menu::startRace() {
    motors.run();
    menuTrigger = true;
    display.disable();
}