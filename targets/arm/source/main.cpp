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

CommandManager<1,'\n',false> commandManager (enableInterrupts, disableInterrupts, {
    Command("m", "", motorsCallback)
});



using namespace halina;


int32_t EncoderGetChLeft() {
    int32_t tmp = (uint16_t)FTM2->CNT;
    FTM2->CNT = 0;
    if(tmp>0x7FFF){
        tmp=(int32_t)(tmp-(int32_t) 0x0000FFFF);
    }
//	tmp /= 2;
    return tmp;
}

volatile uint16_t dataaa = 0;

bool printflag = false;

void pitttt(uint8_t) {
    dataaa = FTM2->CNT;
    FTM2->CNT = 0;
    printflag = true;
}

int main(){
    BOARD_BootClockRUN();
    Kitty& kitty = Kitty::kitty();


    kitty.init();

    Kitty::kitty().encoderRightA.setMux();
    Kitty::kitty().encoderRightB.setMux();

    NXP_PIT ssssss = {NXP_PIT::CHANNEL::_2, 200, pitttt};
    ssssss.init();

    Kitty::kitty().uartCommunication.setRedirectHandler([](uint8_t ch) {commandManager.put_char(ch);});
    commandManager.init(printCommandManager);

    SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
    SIM->SCGC6 |= SIM_SCGC6_FTM2_MASK;

    FTM2->MOD = 0xFFFF;
    FTM2->CNTIN = 0;

    FTM2->MODE |= FTM_MODE_WPDIS_MASK;
    FTM2->MODE |= FTM_MODE_FTMEN_MASK;

    FTM2->QDCTRL &= ~FTM_QDCTRL_QUADMODE_MASK;
    FTM2->QDCTRL |= FTM_QDCTRL_QUADEN_MASK;
    FTM2->QDCTRL |= FTM_QDCTRL_QUADMODE_MASK;

    FTM2->SC |= FTM_SC_CLKS(3);

volatile uint32_t licznik = 0;

    while (true){
        commandManager.run();
        kitty.proc();


    if (printflag) {
        printflag = false;
        log_info("%d\n\r", dataaa);
    }


    }
}



