#include "NXP_menu.hpp"

#include <string>

bool NXP_Menu::proc() {
    // when menu start page is fired, disable menu forever
    if (!initialized || menuTrigger) return false;


    else if (!buttons.at(0).get()) {
        differential.setStartRPM(differential.getStartRPM() + 100);
        delay_ms(300);
    } else if (!buttons.at(1).get()) {
        differential.setStartRPM(differential.getStartRPM() - 100);
        delay_ms(300);
    } else if (!buttons.at(2).get()) {
        differential.setStartRPM(0.0);
        startRace();
    } else if (!buttons.at(3).get()) {
        startRace();
    }
    displayMenuPage();
    display.update();

    return true;
}

std::string formatString(float value) {
    int intValue = static_cast<int>(value);

    if (intValue > 9999) intValue = 9999;
    if (intValue < 0) intValue = 0;

    char buffer[5]; // 4 znaki + null terminator
    snprintf(buffer, sizeof(buffer), "%04d", intValue); // wiodące zera, lub "%4d" dla spacji

    return std::string(buffer);
}

void NXP_Menu::displayMenuPage() {
    std::string rpmText = formatString(differential.getStartRPM());
    display.print(rpmText.c_str());
}

void NXP_Menu::startRace() {
    motors.run();
    menuTrigger = true;
    display.disable();
}