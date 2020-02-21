/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 * Main Kitty class
 *
 */

#include "NXP_Kitty.hpp"

#define LOG_CHANNEL KITTY
#define KITTY_LOG_CHANNEL 2
#define KITTY_LOG_CHANNEL_LEVEL LOG_LEVEL_DEBUG

#include "logger.h"
bool algorithmTrigger = false;
bool commandTerminalTrigger = false;
void pit_generalHandler(uint32_t*) {
    algorithmTrigger = true;
    commandTerminalTrigger = true;
}

uint_fast64_t Kitty::milliseconds = 0;
extern "C" {
bool systickTrigger = false;
void SysTick_Handler(void) {
    Kitty::millisIncrease();
    systickTrigger = true;
}
}

void Kitty::init() {
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    SysTick_Config(SystemCoreClock / 1000);
    NVIC_ClearPendingIRQ(SysTick_IRQn);
    NVIC_EnableIRQ(SysTick_IRQn);

    uartCommunication.init();
    uartDebug.init();
    sensor.init();
    log_notice("Procek wstal pomyslnie!");

}

void Kitty::proc() {
    if(systickTrigger){
        static uint32_t counter;
        systickTrigger = false;
        if(200 <= counter++){
            uint16_t y = sensor.readRangeSingleMillimeters();
            log_notice("result: %d", y);
            counter = 0;
        }
    }
}