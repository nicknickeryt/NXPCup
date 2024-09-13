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


    uint16_t         dataBuf[128] = {0};
    NXP_Camera::Type type         = NXP_Camera::Type::CAMERA_1;
    // size_t           i            = 0;


    // kitty.servo.set(-1.0);

    // Ustawia obydwa silniki na 10% mocy do przodu (zakres od -1.0 do 1.0, podobnie jak w servo)
    kitty.motors.setValue(0.3, 0.3);
    // log_notice("left: %d, right: %d", kitty.motors.getValue().first, kitty.motors.getValue().second);

    // motors.run() chyba włącza silniki, a motors.block() wyłącza.
    kitty.motors.run();

    // Po 10s wyłączamy silniki
    // delay_ms(10000);
    // kitty.motors.block();

    while (true) {
        kitty.proc();
        // kitty.algorithmUnit.analyze();
        kitty.camera.getData(type, dataBuf);
        int32_t position = kitty.newAlgorithm.calculatePosition(dataBuf);
        // int32_t position = kitty.newAlgorithm.proc(dataBuf);
        kitty.servo.set(static_cast<float>(position)/51.0f);
    }
}
