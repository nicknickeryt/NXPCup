#include "NXP_menu.hpp"

#include <string>

std::string formatString(float value) {
    int intValue = static_cast<int>(value);

    if (intValue > 9999) intValue = 9999;
    if (intValue < 0) intValue = 0;

    char buffer[5];
    snprintf(buffer, sizeof(buffer), "%04d", intValue);
    return std::string(buffer);
}

// MENU BUTTONS MAPPING
// 0       1       2       3
// PAGE+   VAL+    VAL-    RUN

bool NXP_Menu::proc(uint32_t currentMillis) {
    // when menu start page is fired, disable menu forever
    if (!initialized || menuTrigger) return false;

    if (currentMillis - menuDebounceTimer < menuDebounceTimeMs) {
        display.update();
        return true;
    }

    // Handle display update when label is shown and do nothing else
    if (currentMillis - menuLabelTimer < menuLabelShowTimeMs) {
        display.update();
        return true;
    }


    if (!buttons.at(MenuButton::BUTTON_PAGE).get()) {
        currentMenuPage = static_cast<MenuPage>(currentMenuPage + 1);
        if (currentMenuPage > 4) currentMenuPage = MenuPage::PAGE_STARTRPM;

        switch (currentMenuPage) {
            case MenuPage::PAGE_STARTRPM:
                display.print("RP\\[");
                break;
            case MenuPage::PAGE_DIFFRATIO: display.print("DIFF"); break;
            case MenuPage::PAGE_BREAK: display.print("BRAK"); break;
            case MenuPage::PAGE_0RPM:
                motors.setEnabled(false);
                display.print("SERV");
                break;
            case MenuPage::PAGE_PID_KP:
                motors.setEnabled(true);
                display.print("PIDP");
                break;
            default: break;
        }

        display.update();
        menuDebounceTimer = currentMillis;
        menuLabelTimer    = currentMillis;
        return true;
    }


    switch (currentMenuPage) {
        case PAGE_STARTRPM: {
            std::string rpmText = formatString(params.getStartRPM());
            display.print(rpmText.c_str());

            if (!buttons.at(MenuButton::BUTTON_VALUE_PLUS).get()) {
                params.setStartRPM(params.getStartRPM() + 50);
                menuDebounceTimer = currentMillis;
            } else if (!buttons.at(MenuButton::BUTTON_VALUE_MINUS).get()) {
                params.setStartRPM(params.getStartRPM() - 50);
                menuDebounceTimer = currentMillis;
            }

            break;
        }
        case MenuPage::PAGE_DIFFRATIO: {
            std::string diffRatioText = formatString(params.getBrakeOne());
            display.print(diffRatioText.c_str());

            if (!buttons.at(MenuButton::BUTTON_VALUE_PLUS).get()) {
                params.setBrakeOne(params.getBrakeOne() + 1);
                menuDebounceTimer = currentMillis;
            } else if (!buttons.at(MenuButton::BUTTON_VALUE_MINUS).get()) {
                params.setBrakeOne(params.getBrakeOne() - 1);
                menuDebounceTimer = currentMillis;
            }
            break;
        }

        case MenuPage::PAGE_BREAK: {
            std::string brakeDividerText = formatString(params.getBrakeAll());
            display.print(brakeDividerText.c_str());

            if (!buttons.at(MenuButton::BUTTON_VALUE_PLUS).get()) {
                params.setBrakeAll(params.getBrakeAll() + 5);
                menuDebounceTimer = currentMillis;
            } else if (!buttons.at(MenuButton::BUTTON_VALUE_MINUS).get()) {
                params.setBrakeAll(params.getBrakeAll() - 5);
                menuDebounceTimer = currentMillis;
            }
            break;
        }

        case MenuPage::PAGE_0RPM: {
            display.print("----");
            break;
        }

        case MenuPage::PAGE_PID_KP: {
            display.print(formatString(params.getPidKp() * 100).c_str());

            if (!buttons.at(MenuButton::BUTTON_VALUE_PLUS).get()) {
                params.setPidKp(params.getPidKp() + 0.01f, differential.getLeftPID(), differential.getRightPID());
                menuDebounceTimer = currentMillis;
            } else if (!buttons.at(MenuButton::BUTTON_VALUE_MINUS).get()) {
                params.setPidKp(params.getPidKp() - 0.01f, differential.getLeftPID(), differential.getRightPID());
                menuDebounceTimer = currentMillis;
            }
            break;
        }

        default: break;
    }

    if (!buttons.at(MenuButton::BUTTON_RUN).get()) startRace(currentMillis);

    display.update();

    return true;
}


void NXP_Menu::startRace(uint32_t currentMillis) {
    delay_ms(3000);
    algorithm.clearPatterns(currentMillis);
    algorithm.setAlgorithmStartTime(currentMillis);
    motors.run();
    menuTrigger = true;
    display.disable();
}