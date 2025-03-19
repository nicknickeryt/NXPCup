/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 * Main Kitty class
 *
 */

#include "NXP_Kitty.hpp"
extern "C" {
#include "clock_config.h"
}

#define LOG_CHANNEL             KITTY
#define KITTY_LOG_CHANNEL       2
#define KITTY_LOG_CHANNEL_LEVEL LOG_LEVEL_DEBUG

#include "logger.h"

uint_fast64_t Kitty::milliseconds = 0;
extern "C" {
volatile bool systickTrigger = false;
void          SysTick_Handler(void) {
    Kitty::millisIncrease();
    static auto counter = 0;
    if (200 == counter++) {
        systickTrigger = true;
        counter        = 0;
    }
}
}

void Kitty::init() {
    BOARD_InitBootClocks();
    //    SysTick_Config(SystemCoreClock / 1000);
    //    NVIC_ClearPendingIRQ(SysTick_IRQn);
    //    NVIC_EnableIRQ(SysTick_IRQn);
    //
    //    uartCommunication.init();
    uartDebug.init();
    sensors.init();
    //    algorithm.init();
    //    uartCommunication.write("xD", 2);
    log_notice("Procek wstal pomyslnie!");
    sensors.selectOutput(1);
}

void Kitty::proc() {
    kitty.magicDiodComposition();
    kitty.menu.proc();

    kitty.camera.getData(kitty.cameraDataBuf);

    int32_t position = kitty.newAlgorithm.calculatePosition(kitty.cameraDataBuf);
    kitty.servo.set(static_cast<float>(position) / 70.0f);
    kitty.differential.proc(position);
    kitty.motors.setValue(kitty.differential.getLeft(), kitty.differential.getRight());


    ////////////////////////////// Uart Log ////////////////////////////////

    if (lastLogTimepoint + LOG_UPDATE_INTERVAL < kitty.millis()) {
        lastLogTimepoint = kitty.millis();

        fctprintf(logWrite, NULL, "99999,");
        fctprintf(logWrite, NULL, "%u,", kitty.newAlgorithm.getBrightness());
        fctprintf(logWrite, NULL, "%u,", (position + 120) / 2);

        for (size_t i = 0; i < 128; i++) {
            uint16_t* buffer = static_cast<uint16_t*>(kitty.cameraDataBuf);
            fctprintf(logWrite, NULL, "%d,", buffer[i]);
        }

        fctprintf(logWrite, NULL, "\r\n");
    }
}