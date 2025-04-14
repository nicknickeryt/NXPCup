/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 * Main Kitty class
 *
 */

#define LOG_CHANNEL KITTY

#include "logger.h"

#include "NXP_Kitty.hpp"
#include "clock_config.h"
#include "pin_mux.h"
#include "printf.h"
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

void logWrite(char c, [[maybe_unused]] void* const context) { Kitty::kitty().uartDebug.write(c); }

uint_fast64_t Kitty::milliseconds = 0;
extern "C" {
bool systickTrigger = false;
void SysTick_Handler(void) {
    Kitty::millisIncrease();
    systickTrigger = true;
}
}

void Kitty::uartCallback(uint8_t receivedByte) {
    switch (receivedByte) {
        case 's':
            fctprintf(logWrite, NULL, "\nkittyStop\n", 0);
            kitty().menu.setTriggeredOff(false);
            kitty().motors.setValue(0, 0);
            kitty().motors.setEnabled(false);
            kitty().servo.set(0);
            kitty().servo.disable();
            break;
        case 'p':
            fctprintf(logWrite, NULL, "\nkittyPause\n", 0);
            kitty().differential.setStartRPM(0);
            fctprintf(logWrite, NULL, "\nkittySV%02u\n", (uint8_t)(kitty().differential.getStartRPM() / 100));
            break;
        case 'o':
            fctprintf(logWrite, NULL, "\nkittyResume\n", 0);
            kitty().differential.setStartRPM(0.3);
            fctprintf(logWrite, NULL, "\nkittySV%02u\n", (uint8_t)(kitty().differential.getStartRPM() / 100));
            break;
        case 'r':
            fctprintf(logWrite, NULL, "\nkittyRun\n", 0);
            kitty().menu.setTriggeredOff(true);
            kitty().motors.setEnabled(true);
            kitty().newAlgorithm.clearPatterns();
            kitty().servo.init();
            kitty().menu.startRace();
            break;
        case '+': // 43
            kitty().differential.setStartRPM(kitty().differential.getStartRPM() + 100);
            fctprintf(logWrite, NULL, "\nkittySV%02u\n", (uint8_t)(kitty().differential.getStartRPM() / 100));
            break;
        case '-': // startVelocity--
            kitty().differential.setStartRPM(kitty().differential.getStartRPM() - 100);
            fctprintf(logWrite, NULL, "\nkittySV%02u\n", (uint8_t)(kitty().differential.getStartRPM() / 100));
            break;
        case 'a': // diffRatio++  
            kitty().differential.setDiffValue(kitty().differential.getDiffValue() + 1);
            fctprintf(logWrite, NULL, "\nkittyDR%02u\n", (uint8_t)(kitty().differential.getDiffValue()));
            break;
        case 'b': // diffRatio--
            kitty().differential.setDiffValue(kitty().differential.getDiffValue() - 1);
            fctprintf(logWrite, NULL, "\nkittyDR%02u\n", (uint8_t)(kitty().differential.getDiffValue()));
            break;
        default: break;
    }
}

void Kitty::uartKLZCallback(uint8_t data) {
    Kitty::kitty().uartFrame.deserialize(&data, sizeof(data));
}

void Kitty::onKLZDataReceivedCallback(uint8_t* data, size_t length) {
    if (length == 0 || length > 2) {
        return;
    }
    
    uint16_t distance = 0;
    distance = static_cast<uint16_t>(data[0] << 8) | data[1];
    fctprintf(logWrite, NULL, "UART_Des KLZ: %d\r\n", distance); 

    Kitty::kitty().differential.setKlzDistance(distance);
}

void Kitty::init() {
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    SysTick_Config(SystemCoreClock / 1000);
    NVIC_ClearPendingIRQ(SysTick_IRQn);
    NVIC_EnableIRQ(SysTick_IRQn);

    FTM_Init();
    uartDebug.init();
    uartDebug.enableInterrupt(NXP_Uart::InterruptType::RX_FULL);
    uartDebug.setRedirectHandler(uartCallback);
    log_setWriteFunction(logWrite);
    uartCommunication.init();
    uartCommunication.initDMA();

    uartKLZ.init();
    uartKLZ.initDMA();
    uartKLZ.enableInterrupt(NXP_Uart::InterruptType::RX_FULL);
    uartKLZ.setRedirectHandler(uartKLZCallback);

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

void Kitty::proc() {
    magicDiodComposition();
    camera.getData(cameraDataBuf);

    float position = newAlgorithm.calculatePosition(cameraDataBuf, millis());

    ////////////////////////////// Uart Log ////////////////////////////////
    if (lastLogTimepoint + LOG_UPDATE_INTERVAL < millis()) {
        lastLogTimepoint = millis();    

        // fctprintf(logWrite, NULL, "klz: %d\r\n", (kitty().differential.getKlzDistance()));        
        // fctprintf(logWrite, NULL, "isbrk?: %d\r\n", (kitty().differential.isBreakTriggered()));
        // fctprintf(logWrite, NULL, "brkPidOut: .%u\r\n", (uint8_t) (100 * (differential.brakingPIDOutput  )));


        fctprintf(logWrite, NULL, "\r\nCAML");
        for (size_t i = 0; i < 128; i++) {
            uint16_t* buffer = static_cast<uint16_t*>(cameraDataBuf);
            fctprintf(logWrite, NULL, ".%hhu", buffer[i] / 158);
        }
        fctprintf(logWrite, NULL, ".%hhu", (uint8_t)(position + 63));
        fctprintf(logWrite, NULL, ".%hhu", newAlgorithm.getBrightness() / 158);
        fctprintf(logWrite, NULL, ".%u", newAlgorithm.getCrossings());
        fctprintf(logWrite, NULL, ".%u", encoderRight.getRPM());

        fctprintf(logWrite, NULL, ".%u", (uint8_t) differential.getKlzDistance());
        fctprintf(logWrite, NULL, ".%u", (uint8_t) (100 * (differential.getLeft() + 1)  )); // -1:1 -> 0:200
        fctprintf(logWrite, NULL, ".%u", (uint8_t) (100 * (differential.getRight() + 1) )); // -1:1 -> 0:200

        if (!menu.isTriggeredOff()) fctprintf(logWrite, NULL, "\nkittySV%02u\n", (uint8_t)(kitty().differential.getStartRPM() / 100));
    }

    // If menu is active, do not move
    if (menu.proc(millis())) {
        return;
    }
      
    float servoPosition = -(position / 19.0f);

    servo.set(servoPosition);
    differential.proc(position, millis());
    motors.setValue(differential.getLeft(), differential.getRight());
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