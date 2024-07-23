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

    kitty.display.print(1234);

    

    uint16_t dataBuf[128] = {0};
    NXP_Camera::Type type = NXP_Camera::Type::CAMERA_1;
    size_t i = 0;


    kitty.servo.set(-1.0);

    while (true) {
        kitty.proc();

    //kitty.algorithmUnit.analyze();
        //kitty.camera.getData(type, dataBuf);

        /*delay_ms(1000);
        // log_notice("%d", dataBuf[i%128]);

        for (int j = 0; j<128; j++) {
            delay_ms(1);
            log_notice("%d",dataBuf[j]);
        }*/
    }
}
