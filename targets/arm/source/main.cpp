#include "NXP_hal.hpp"

using namespace halina;

int main(void){
    Kitty& kitty = Kitty::kitty();

    kitty.init();

    while (true){
        kitty.proc();
    }
}