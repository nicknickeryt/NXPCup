#include <command_terminal/Command.h>
#include "commandManager.h"
#include "NXP_Kitty.hpp"

void motorsCallback(const char* data) {
    auto [m] = parser::get<float>(data);
    Kitty::kitty().motors.setValue(m, m);
}

void servoCallback(const char* data) {
    auto [s] = parser::get<float>(data);
    Kitty::kitty().servo.set(s);
}

void stopCallback(const char*) {
    Kitty::kitty().motors.setValue(0.0f, 0.0f);
    Kitty::kitty().servo.disable();
}