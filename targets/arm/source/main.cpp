/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 * NXP Main file
 *
 */

#include <drivers/fsl_uart.h>
#include <drivers/fsl_uart_edma.h>
#include "HALina.hpp"
#include "NXP_Kitty.hpp"
#include "clock_config.h"

#define LOG_CHANNEL APP
#define APP_LOG_CHANNEL 1
#define APP_LOG_CHANNEL_LEVEL LOG_LEVEL_DEBUG

#include "logger.h"

using namespace halina;

static uint_fast64_t milliseconds = 0;
void millisIncrease() {
    milliseconds++;
}

uint32_t millis() {
    return milliseconds;
}

extern "C" {
    void SysTick_Handler(void) {
        millisIncrease();
    }
}

int main(){
    BOARD_BootClockRUN();

    // init SysTick
    SysTick_Config(SystemCoreClock / 1000);
    NVIC_ClearPendingIRQ(SysTick_IRQn);
    NVIC_EnableIRQ(SysTick_IRQn);
    //
    Kitty& kitty = Kitty::kitty();
    kitty.init();

    while (true){
        kitty.proc();
    }
}



