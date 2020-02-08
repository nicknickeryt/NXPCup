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
#include "command_terminal/command_manager.h"
#include "commandManager.h"
#include "NXP_encoder.hpp"

#define LOG_CHANNEL APP
#define APP_LOG_CHANNEL 1
#define APP_LOG_CHANNEL_LEVEL LOG_LEVEL_DEBUG

#include "logger.h"

void enableInterrupts() {
    __enable_irq();
}

void disableInterrupts() {
    __disable_irq();
}

void printCommandManager(char data) {
    Kitty::kitty().uartCommunication.write(data);
}

CommandManager<3,'\n',false> commandManager (enableInterrupts, disableInterrupts, {
    Command("m", "", motorsCallback),
    Command("s", "", servoCallback),
    Command("st", "", stopCallback)
});

using namespace halina;

bool printflag = false;

void printFlagFunction(uint32_t*) {
    printflag = true;
}

int main(){
    BOARD_BootClockRUN();
    Kitty& kitty = Kitty::kitty();

    kitty.init();

    NXP_PIT dddd = {NXP_PIT::CHANNEL::_3, 100, printFlagFunction, nullptr};
    dddd.init();

    Kitty::kitty().uartCommunication.setRedirectHandler([](uint8_t ch) {commandManager.put_char(ch);});
    commandManager.init(printCommandManager);

    while (true){
        commandManager.run();
        kitty.proc();

        if (printflag) {
            printflag = false;
            log_info("%d %d\n\r", Kitty::kitty().encoderLeft.getTicks(), Kitty::kitty().encoderRight.getTicks());
        }
    }
}



