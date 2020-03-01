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
bool frameTrigger = false;

bool pixyTrigger = false;
void pit_generalHandler(uint32_t*) {
    algorithmTrigger = true;
    commandTerminalTrigger = true;
    frameTrigger = true;
    pixyTrigger = true;
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
    uartPixy.init();

    ledLine.init();
    display.init();
    servo.init();
    camera.init();
    pitCamera.init();
    pitSendCameraData.init();
    encoderRight.init();
    encoderLeft.init();
    menu.init();
    motors.init();
    commandManager.init(printCommandManager);
    pixy.init();

    motors.run();
    camera.start();
    encodersPit.appendCallback(NXP_Encoder::ISR, reinterpret_cast<uint32_t *>(&encoderRight));
    encodersPit.appendCallback(NXP_Encoder::ISR, reinterpret_cast<uint32_t *>(&encoderLeft));
    encodersPit.init();
    uartCommunication.setRedirectHandler([](uint8_t ch) {Kitty::kitty().commandManager.put_char(ch);});
    display.enable();

    uartCommunication.write("Bejbi don't hurt me", 19);

    menu.addParameter(&algorithmUnit.speed, 0.01);
    servo.set(0.0);
    camera.start();
    log_notice("KiTTy init finished");
    algorithmUnit.checkSwitches();
}

void Kitty::proc() {
    static int16_t motorsValues[2];
    static int16_t encodersValues[2];
    static uint8_t linesValues[2];
    float leftSpeedToModify = algorithmUnit.speed;
    float rightSpeedToModify = algorithmUnit.speed;
    uint16_t encoderLeftSample = encoderLeft.getTicks();
    uint16_t encoderRightSample = encoderRight.getTicks();
    if (!menu.proc(systickTrigger)) {
        log_notice("L_in: %f R_in: %f", algorithmUnit.speed, algorithmUnit.speed);
        algorithmUnit.pid.calculate(&leftSpeedToModify, &rightSpeedToModify, encoderLeftSample, encoderRightSample);
        log_notice("L_out: %f R_out: %f", leftSpeedToModify, rightSpeedToModify);
        motors.setValue(leftSpeedToModify, rightSpeedToModify);
    }

    if (frameTrigger) {
        static uint8_t line1 = 10;
        static uint8_t line2 = 5;
        static bool speedUp;
        static bool slowDown;
        static bool emergency;
        static bool stop;
        static bool crossroad;
        motorsValues[0] = int16_t(algorithmUnit.speed * 100.0);
        motorsValues[1] = int16_t(algorithmUnit.speed * 100.0);
        encodersValues[0] = int16_t(encoderLeftSample);
        encodersValues[1] = int16_t(encoderRightSample);
        linesValues[0] = line1;
        linesValues[1] = line2;
        uint8_t obstacleSide = 1;
        speedUp ^= 1U;
        slowDown ^= 1U;
        emergency ^= 1U;
        stop ^= 1U;
        crossroad ^= 1U;;
        frame.setPayload(linesValues, motorsValues, encodersValues, int16_t(servo.get() * 100), obstacleSide, speedUp,
                         slowDown, emergency, stop, crossroad);
        frame.sendFrameProc();
        frameTrigger = false;
    }

    if (pixyTrigger) {
        pixyTrigger = false;
        pixy.control();
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