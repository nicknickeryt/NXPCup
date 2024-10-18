/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 * NXP Main file
 *
 */

#include "logger.h"

#include "HALina.hpp"
#include "NXP_Kitty.hpp"
#include "clock_config.h"

int main() {
    BOARD_BootClockRUN();
    Kitty& kitty = Kitty::kitty();
    kitty.init();

    while (true) {
        kitty.magicDiodComposition();
        if (!kitty.menu.proc()) break;
    }

    while (true) {
        kitty.magicDiodComposition();
        kitty.camera.getData(kitty.cameraDataBuf);
        int32_t position = kitty.newAlgorithm.calculatePosition(kitty.cameraDataBuf);
        kitty.servo.set(static_cast<float>(position) / 51.0f);
    }
}
