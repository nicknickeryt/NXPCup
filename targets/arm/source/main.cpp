#include "NXP_hal.hpp"

using namespace halina;

int main(void){
    Kitty& kitty = Kitty::kitty();

    kitty.init();
    kitty.uart.write("Hello world xD\n");

    while (true){
        kitty.proc();

    }
}