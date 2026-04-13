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
        if (currentMenuPage > 17) currentMenuPage = MenuPage::PAGE_STARTRPM; // DO THIS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

        switch (currentMenuPage) {
            case MenuPage::PAGE_STARTRPM:
                display.print("RP\\[");
                break;
            case MenuPage::PAGE_SERVODIV: display.print("SDIV"); break;
            case MenuPage::PAGE_SERVOALPHA: display.print("SALP"); break;
            case MenuPage::PAGE_CORR: display.print("CORR"); break;
            case MenuPage::PAGE_PID_KP: display.print("PIDP"); break;
            case MenuPage::PAGE_PID_KI: display.print("PIDI"); break;
            case MenuPage::PAGE_BRAKE_ALL: display.print("BRKA"); break;
            case MenuPage::PAGE_BRAKE_ONE: display.print("BRK1"); break;
            case MenuPage::PAGE_BRAKE_CLAMP: display.print("BCLA"); break;
            case MenuPage::PAGE_RPMOFFSET: display.print("ROFF"); break;
            case MenuPage::PAGE_DIFFCLAMP: display.print("DCLA"); break;
            case MenuPage::PAGE_CROSSCUT: display.print("CCUT"); break;
            case MenuPage::PAGE_BRIGHTNESSALPHA: display.print("BALP"); break;
            case MenuPage::PAGE_CROSSING_MULT: display.print("C\\[U"); break;
            case MenuPage::PAGE_CROSSINGALPHA: display.print("CALP"); break;
            case MenuPage::PAGE_CAMFREQ: display.print("CFRE"); break;
            case MenuPage::PAGE_ALGORITHM_OFFSET: display.print("AOFF"); break;
            case MenuPage::PAGE_PATTERN_TIMEOUT: display.print("TI\\["); break;
            case MenuPage::PAGE_0RPM:
                motors.setEnabled(false);
                display.print("TEST");
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

        case MenuPage::PAGE_0RPM: {
            display.print("----");
            break;
        }

        case MenuPage::PAGE_SERVODIV: {
            std::string servoDivText = formatString(params.getServoDivider() * 10.0f);
            display.print(servoDivText.c_str());

            if (!buttons.at(MenuButton::BUTTON_VALUE_PLUS).get()) {
                params.setServoDivider(params.getServoDivider() + 0.1f);
                menuDebounceTimer = currentMillis;
            } else if (!buttons.at(MenuButton::BUTTON_VALUE_MINUS).get()) {
                params.setServoDivider(params.getServoDivider() - 0.1f);
                menuDebounceTimer = currentMillis;
            }
            break;
        }

        case MenuPage::PAGE_SERVOALPHA: {
            std::string brakeDividerText = formatString(params.getAlgorithmFilterAlpha() * 100.0f);
            display.print(brakeDividerText.c_str());

            if (!buttons.at(MenuButton::BUTTON_VALUE_PLUS).get()) {
                params.setAlgorithmFilterAlpha(params.getAlgorithmFilterAlpha() + 0.05f);
                menuDebounceTimer = currentMillis;
            } else if (!buttons.at(MenuButton::BUTTON_VALUE_MINUS).get()) {
                params.setAlgorithmFilterAlpha(params.getAlgorithmFilterAlpha() - 0.05f);
                menuDebounceTimer = currentMillis;
            }
            break;
        }

        case MenuPage::PAGE_CORR: {
            std::string brakeDividerText = formatString(params.getPatternCorrelationThreshold() * 100.0f);
            display.print(brakeDividerText.c_str());

            if (!buttons.at(MenuButton::BUTTON_VALUE_PLUS).get()) {
                params.setPatternCorrelationThreshold(params.getPatternCorrelationThreshold() + 0.01f);
                menuDebounceTimer = currentMillis;
            } else if (!buttons.at(MenuButton::BUTTON_VALUE_MINUS).get()) {
                params.setPatternCorrelationThreshold(params.getPatternCorrelationThreshold() - 0.01f);
                menuDebounceTimer = currentMillis;
            }
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

        case MenuPage::PAGE_PID_KI: {
            display.print(formatString(params.getPidKi() * 10000).c_str());

            if (!buttons.at(MenuButton::BUTTON_VALUE_PLUS).get()) {
                params.setPidKi(params.getPidKi() + 0.0001f, differential.getLeftPID(), differential.getRightPID());
                menuDebounceTimer = currentMillis;
            } else if (!buttons.at(MenuButton::BUTTON_VALUE_MINUS).get()) {
                params.setPidKi(params.getPidKi() - 0.0001f, differential.getLeftPID(), differential.getRightPID());
                menuDebounceTimer = currentMillis;
            }
            break;
        }


        case MenuPage::PAGE_BRAKE_ALL: {
            std::string servoDivText = formatString(params.getBrakeAll() * 10.0f);
            display.print(servoDivText.c_str());

            if (!buttons.at(MenuButton::BUTTON_VALUE_PLUS).get()) {
                params.setBrakeAll(params.getBrakeAll() + 0.5f);
                menuDebounceTimer = currentMillis;
            } else if (!buttons.at(MenuButton::BUTTON_VALUE_MINUS).get()) {
                params.setBrakeAll(params.getBrakeAll() - 0.5f);
                menuDebounceTimer = currentMillis;
            }
            break;
        }


        case MenuPage::PAGE_BRAKE_ONE: {
            std::string servoDivText = formatString(params.getBrakeOne() * 10.0f);
            display.print(servoDivText.c_str());

            if (!buttons.at(MenuButton::BUTTON_VALUE_PLUS).get()) {
                params.setBrakeOne(params.getBrakeOne() + 0.5f);
                menuDebounceTimer = currentMillis;
            } else if (!buttons.at(MenuButton::BUTTON_VALUE_MINUS).get()) {
                params.setBrakeOne(params.getBrakeOne() - 0.5f);
                menuDebounceTimer = currentMillis;
            }
            break;
        }


        case MenuPage::PAGE_BRAKE_CLAMP: {
            std::string servoDivText = formatString(params.getBrakeClamp() * 100.0f);
            display.print(servoDivText.c_str());

            if (!buttons.at(MenuButton::BUTTON_VALUE_PLUS).get()) {
                params.setBrakeClamp(params.getBrakeClamp() + 0.05f);
                menuDebounceTimer = currentMillis;
            } else if (!buttons.at(MenuButton::BUTTON_VALUE_MINUS).get()) {
                params.setBrakeClamp(params.getBrakeClamp() - 0.05f);
                menuDebounceTimer = currentMillis;
            }
            break;
        }



        case MenuPage::PAGE_RPMOFFSET: {
            std::string servoDivText = formatString(params.getRPMOffset());
            display.print(servoDivText.c_str());

            if (!buttons.at(MenuButton::BUTTON_VALUE_PLUS).get()) {
                params.setRPMOffset(params.getRPMOffset() + 50);
                menuDebounceTimer = currentMillis;
            } else if (!buttons.at(MenuButton::BUTTON_VALUE_MINUS).get()) {
                params.setRPMOffset(params.getRPMOffset() - 50);
                menuDebounceTimer = currentMillis;
            }
            break;
        }

        
        case MenuPage::PAGE_DIFFCLAMP: {
            std::string servoDivText = formatString(params.getDiffClamp() * 100.0f);
            display.print(servoDivText.c_str());

            if (!buttons.at(MenuButton::BUTTON_VALUE_PLUS).get()) {
                params.setDiffClamp(params.getDiffClamp() + 0.05f);
                menuDebounceTimer = currentMillis;
            } else if (!buttons.at(MenuButton::BUTTON_VALUE_MINUS).get()) {
                params.setDiffClamp(params.getDiffClamp() - 0.05f);
                menuDebounceTimer = currentMillis;
            }
            break;
        }


        case MenuPage::PAGE_CROSSCUT: {
            std::string servoDivText = formatString(params.getCrossingsCut());
            display.print(servoDivText.c_str());

            if (!buttons.at(MenuButton::BUTTON_VALUE_PLUS).get()) {
                params.setCrossingsCut(params.getCrossingsCut() + 1);
                menuDebounceTimer = currentMillis;
            } else if (!buttons.at(MenuButton::BUTTON_VALUE_MINUS).get()) {
                params.setCrossingsCut(params.getCrossingsCut() - 1);
                menuDebounceTimer = currentMillis;
            }
            break;
        }


        case MenuPage::PAGE_BRIGHTNESSALPHA: {
            std::string brakeDividerText = formatString(params.getBrightnessMeanAlpha() * 100.0f);
            display.print(brakeDividerText.c_str());

            if (!buttons.at(MenuButton::BUTTON_VALUE_PLUS).get()) {
                params.setBrightnessMeanAlpha(params.getBrightnessMeanAlpha() + 0.05f);
                menuDebounceTimer = currentMillis;
            } else if (!buttons.at(MenuButton::BUTTON_VALUE_MINUS).get()) {
                params.setBrightnessMeanAlpha(params.getBrightnessMeanAlpha() - 0.05f);
                menuDebounceTimer = currentMillis;
            }
            break;
        }


        case MenuPage::PAGE_CROSSING_MULT: {
            display.print(formatString(params.getCrossingsBrightnessMultiplier() * 100).c_str());

            if (!buttons.at(MenuButton::BUTTON_VALUE_PLUS).get()) {
                params.setCrossingsBrightnessMultiplier(params.getCrossingsBrightnessMultiplier() + 0.05f);
                menuDebounceTimer = currentMillis;
            } else if (!buttons.at(MenuButton::BUTTON_VALUE_MINUS).get()) {
                params.setCrossingsBrightnessMultiplier(params.getCrossingsBrightnessMultiplier() - 0.05f);
                menuDebounceTimer = currentMillis;
            }
            break;
        }


        case MenuPage::PAGE_CROSSINGALPHA: {
            std::string brakeDividerText = formatString(params.getCameraCrossingFilterAlpha() * 100.0f);
            display.print(brakeDividerText.c_str());

            if (!buttons.at(MenuButton::BUTTON_VALUE_PLUS).get()) {
                params.setCameraCrossingFilterAlpha(params.getCameraCrossingFilterAlpha() + 0.05f);
                menuDebounceTimer = currentMillis;
            } else if (!buttons.at(MenuButton::BUTTON_VALUE_MINUS).get()) {
                params.setCameraCrossingFilterAlpha(params.getCameraCrossingFilterAlpha() - 0.05f);
                menuDebounceTimer = currentMillis;
            }
            break;
        }


        case MenuPage::PAGE_CAMFREQ: {
            std::string servoDivText = formatString(params.getCameraPitFrequency() / 1000.0f);
            display.print(servoDivText.c_str());

            if (!buttons.at(MenuButton::BUTTON_VALUE_PLUS).get()) {
                params.setCameraPitFrequency(cameraPit, params.getCameraPitFrequency() + 1000);
                menuDebounceTimer = currentMillis;
            } else if (!buttons.at(MenuButton::BUTTON_VALUE_MINUS).get()) {
                params.setCameraPitFrequency(cameraPit, params.getCameraPitFrequency() - 1000);
                menuDebounceTimer = currentMillis;
            }
            break;
        }

        case MenuPage::PAGE_ALGORITHM_OFFSET: {
            display.print(formatString(params.getAlgorithmOffset() * 100).c_str());

            if (!buttons.at(MenuButton::BUTTON_VALUE_PLUS).get()) {
                params.setAlgorithmOffset(params.getAlgorithmOffset() + 0.1f);
                menuDebounceTimer = currentMillis;
            } else if (!buttons.at(MenuButton::BUTTON_VALUE_MINUS).get()) {
                params.setAlgorithmOffset(params.getAlgorithmOffset() - 0.1f);
                menuDebounceTimer = currentMillis;
            }
            break;
        }

        case MenuPage::PAGE_PATTERN_TIMEOUT: {
            std::string servoDivText = formatString(params.getPatternDetectTimeoutMs());
            display.print(servoDivText.c_str());

            if (!buttons.at(MenuButton::BUTTON_VALUE_PLUS).get()) {
                params.setPatternDetectTimeoutMs(params.getPatternDetectTimeoutMs() + 100);
                menuDebounceTimer = currentMillis;
            } else if (!buttons.at(MenuButton::BUTTON_VALUE_MINUS).get()) {
                params.setPatternDetectTimeoutMs(params.getPatternDetectTimeoutMs() - 100);
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