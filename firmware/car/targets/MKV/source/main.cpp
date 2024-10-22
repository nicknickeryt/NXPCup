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
#include "printf.h"

size_t lastLogTimepoint;
#define LOG_UPDATE_INTERVAL 1000

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

        if(lastLogTimepoint + LOG_UPDATE_INTERVAL < kitty.millis()) {
            lastLogTimepoint = kitty.millis();
            //fctprintf(logWrite, NULL, "%d ", kitty.millis()); //milis test

            for(size_t i = 0; i<sizeof(kitty.cameraDataBuf)/sizeof(uint16_t); i++) { 
                uint16_t *buffer = static_cast<uint16_t*>(kitty.cameraDataBuf);
                fctprintf(logWrite, NULL, "%d, ", buffer[i]);
            }
            fctprintf(logWrite, NULL, "\r\n");
        }
    }
}