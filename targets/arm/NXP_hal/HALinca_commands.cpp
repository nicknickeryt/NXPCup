#include "HALina_commands.hpp"

void enableInterrupts() { }

void disableInterrupts() { }

void pingCallback(const char* data) {
    commandManager.print("pong");
    auto[data1, data2] = parser::get<float, float>(data);
    (void) data1;
    (void) data2;
}


CommandManager<1, '\n', false> commandManager (enableInterrupts, disableInterrupts, {
        Command("ping", "<float, float> returns pong", pingCallback)
});



