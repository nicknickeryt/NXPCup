#include "NXP_menu.hpp"

#include <string>

std::string formatString(float value) {
    int intValue = static_cast<int>(value);

    if (intValue > 9999) intValue = 9999;
    if (intValue < 0) intValue = 0;

    char buffer[5];                                     // 4 znaki + null terminator
    snprintf(buffer, sizeof(buffer), "%04d", intValue); // wiodące zera, lub "%4d" dla spacji

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
        if (currentMenuPage > 3) currentMenuPage = MenuPage::PAGE_STARTRPM;

        switch (currentMenuPage) {
            case MenuPage::PAGE_STARTRPM:
                differential.setStartRPM(3000); // TODO handle this better
                display.print("RP\\[");
                break;
            case MenuPage::PAGE_DIFFRATIO: display.print("DIFF"); break;
            case MenuPage::PAGE_BREAK: display.print("BRAK"); break;
            case MenuPage::PAGE_0RPM:
                differential.setStartRPM(0.0);
                display.print("SERV");
                break;
            default: break;
        }

        display.update();
        menuDebounceTimer = currentMillis;
        menuLabelTimer = currentMillis;
        return true;
    }


    switch (currentMenuPage) {
        case PAGE_STARTRPM: {
            std::string rpmText = formatString(differential.getStartRPM());
            display.print(rpmText.c_str());

            if (!buttons.at(MenuButton::BUTTON_VALUE_PLUS).get()) {
                differential.setStartRPM(differential.getStartRPM() + 100);
                menuDebounceTimer = currentMillis;
            } else if (!buttons.at(MenuButton::BUTTON_VALUE_MINUS).get()) {
                differential.setStartRPM(differential.getStartRPM() - 100);
                menuDebounceTimer = currentMillis;
            }

            break;
        }
        case MenuPage::PAGE_DIFFRATIO: {
            std::string diffRatioText = formatString(differential.getDiffValue());
            display.print(diffRatioText.c_str());

            if (!buttons.at(MenuButton::BUTTON_VALUE_PLUS).get()) {
                differential.setDiffValue(differential.getDiffValue() + 1);
                menuDebounceTimer = currentMillis;
            } else if (!buttons.at(MenuButton::BUTTON_VALUE_MINUS).get()) {
                differential.setDiffValue(differential.getDiffValue() - 1);
                menuDebounceTimer = currentMillis;
            }
            break;
        }

        case MenuPage::PAGE_BREAK: {
            std::string brakeDividerText = formatString(differential.getBrakeDivider());
            display.print(brakeDividerText.c_str());

            if (!buttons.at(MenuButton::BUTTON_VALUE_PLUS).get()) {
                differential.setBrakeDivider(differential.getBrakeDivider() + 5);
                menuDebounceTimer = currentMillis;
            } else if (!buttons.at(MenuButton::BUTTON_VALUE_MINUS).get()) {
                differential.setBrakeDivider(differential.getBrakeDivider() - 5);
                menuDebounceTimer = currentMillis;
            }
            break;
        }

        case MenuPage::PAGE_0RPM: {
            display.print("----");
            break;
        }
        default: break;
    }

    if (!buttons.at(MenuButton::BUTTON_RUN).get()) startRace();

    display.update();

    return true;
}


void NXP_Menu::startRace() {
    motors.run();
    menuTrigger = true;
    display.disable();
}