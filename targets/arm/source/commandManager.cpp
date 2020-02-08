#include <command_terminal/Command.h>
#include "commandManager.h"
#include "NXP_Kitty.hpp"

void motorsCallback(const char* data) {
    auto [m] = parser::get<float>(data);
    Kitty::kitty().motors.setValue(m, m);
}