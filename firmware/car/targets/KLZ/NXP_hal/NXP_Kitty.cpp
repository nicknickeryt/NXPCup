/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 * Main Kitty class
 *
 */

#include "NXP_Kitty.hpp"
extern "C"{
    #include "clock_config.h"
}

#define LOG_CHANNEL KITTY
#define KITTY_LOG_CHANNEL 2
#define KITTY_LOG_CHANNEL_LEVEL LOG_LEVEL_DEBUG

#include "logger.h"

uint_fast64_t Kitty::milliseconds = 0;
extern "C" {
volatile bool systickTrigger = false;
void SysTick_Handler(void) {
    Kitty::millisIncrease();
    static auto counter = 0;
    if(200 == counter++) {
        systickTrigger = true;
        counter = 0;
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
//    uartDebug.init();
//    algorithm.init();
//    uartCommunication.write("xD", 2);
//    log_notice("Procek wstal pomyslnie!");
}

void Kitty::proc() {
//    algorithm.proc(systickTrigger);
}