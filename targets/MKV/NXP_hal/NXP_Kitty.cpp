/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 * Main Kitty class
 *
 */

#include <algorithms/algorithm_unit.hpp>
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

    FTM_Init();
    uartDebug.init();
    uartCommunication.init();
    uartCommunication.initDMA();
    uartToKLZ.init();
    ledLine.init();
    display.init();
    servo.init();
    camera.init();
    pitCamera.init();
    pitSendCameraData.init();
    encoderRight.init();
    encoderLeft.init();
    encodersPit.init();
    menu.init();
    motors.init();
    commandManager.init(printCommandManager);

    motors.run();
    camera.start();
    encodersPit.appendCallback(NXP_Encoder::ISR, reinterpret_cast<uint32_t *>(&encoderRight));
    encodersPit.appendCallback(NXP_Encoder::ISR, reinterpret_cast<uint32_t *>(&encoderLeft));
    uartCommunication.setRedirectHandler([](uint8_t ch) {Kitty::kitty().commandManager.put_char(ch);});
    display.enable();

    uartCommunication.write("Bejbi don't hurt me", 19);


    menu.addParameter(&algorithmUnit.startSpeed, 0.01);
    servo.set(0.0);
    camera.start();
    log_notice("KiTTy init finished");
    algorithmUnit.checkSwitches();
}

void Kitty::proc() {
    if(!menu.proc(systickTrigger)) {
        motors.setValue(algorithmUnit.startSpeed, algorithmUnit.startSpeed);
        if (commandTerminalTrigger) {
            commandManager.run();
            commandTerminalTrigger = false;
        }
        if (algorithmTrigger) {
            algorithmTrigger = false;
            camera.getData(NXP_Camera::Type::CAMERA_1, algorithmUnit.algorithmData.cameraData);
            algorithmUnit.analyze();
        }
    }
    magicDiodComposition();
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

void Kitty::magicDiodComposition(){
    static uint32_t licznik = 0;
    static int8_t led_index = 0;
    static uint8_t direction = 0;
    static uint8_t old_led = 0;
    licznik++;
    if (licznik == 50000) {
        licznik = 0;
        if (direction == 0) {
            old_led = led_index;
            led_index++;
            if (led_index == 8) {
                led_index = 6;
                direction = 1;
                old_led = 7;
            }
            ledLine.at(led_index).set();
            ledLine.at(old_led).reset();
        } else if (direction == 1) {
            old_led = led_index;
            led_index--;
            if (led_index == -1) {
                led_index = 1;
                direction = 0;
                old_led = 0;
            }
            ledLine.at(led_index).set();
            ledLine.at(old_led).reset();
        }
    }
}