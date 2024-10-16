/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 * Main Kitty class
 *
 */

#define LOG_CHANNEL KITTY

#include "NXP_Kitty.hpp"

#include "clock_config.h"
#include "pin_mux.h"


#include "logger.h"
bool algorithmTrigger       = false;
bool commandTerminalTrigger = false;
bool frameTrigger           = false;

bool pixyTrigger = false;
void pit_generalHandler(uint32_t*) {
    algorithmTrigger       = true;
    commandTerminalTrigger = true;
    frameTrigger           = true;
    pixyTrigger            = true;
}

static void logWrite(char c, [[maybe_unused]] void* const context) {
    Kitty::kitty().uartDebug.write(c);
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

    FTM_Init();
    uartDebug.init();
    log_setWriteFunction(logWrite);
    uartCommunication.init();
    uartCommunication.initDMA();

    ledLine.init();
    display.init();
    servo.init();
    servo.set(0.0);
    servo.disable();
    camera.init();
    pitCamera.init();
    pitSendCameraData.init();
    encoderRight.init();
    encoderLeft.init();
    menu.init();
    motors.init();
    motors.setValue(0.2, 0.2);
    // commandManager.init(printCommandManager);

    camera.start();
    encodersPit.appendCallback(NXP_Encoder::ISR, reinterpret_cast<uint32_t*>(&encoderRight));
    encodersPit.appendCallback(NXP_Encoder::ISR, reinterpret_cast<uint32_t*>(&encoderLeft));
    encodersPit.init();
    // uartCommunication.setRedirectHandler([](uint8_t ch) {Kitty::kitty().commandManager.put_char(ch);});
    display.enable();

    uartCommunication.write("Bejbi don't hurt me", 19);

    menu.init();
    camera.start();
    log_notice("KiTTy init finished");
}


void Kitty::FTM_Init() {
    SIM->SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK;
    SIM->SOPT2 |= SIM_SOPT2_TIMESRC(1);
    SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;
    SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
    SIM->SCGC6 |= SIM_SCGC6_FTM0_MASK;
    SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
    SIM->SCGC6 |= SIM_SCGC6_FTM3_MASK;
}

void Kitty::magicDiodComposition() {
    static uint32_t licznik   = 0;
    static int8_t   ledIndex  = 0;
    static uint8_t  direction = 0;
    static uint8_t  oldLed    = 0;
    licznik++;
    if (licznik == 5000) {
        licznik = 0;
        if (direction == 0) {
            oldLed = ledIndex;
            ledIndex++;
            if (ledIndex == 8) {
                ledIndex  = 6;
                direction = 1;
                oldLed    = 7;
            }
            ledLine.at(ledIndex).set();
            ledLine.at(oldLed).reset();
        } else if (direction == 1) {
            oldLed = ledIndex;
            ledIndex--;
            if (ledIndex == -1) {
                ledIndex  = 1;
                direction = 0;
                oldLed    = 0;
            }
            ledLine.at(ledIndex).set();
            ledLine.at(oldLed).reset();
        }
    }
}