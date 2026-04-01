/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 * Main Kitty class
 *
 */

#include "HALina_led_line.hpp"
#include "algorithms/motor.hpp"
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

bool cameraDataReceived = false;

void logWrite(char c, [[maybe_unused]] void* const context) { Kitty::kitty().uartDebug.write(c); }

uint_fast64_t Kitty::milliseconds = 0;
extern "C" {
bool systickTrigger = false;
void SysTick_Handler(void) {
    Kitty::millisIncrease();
    systickTrigger = true;
}
}

float motorL = 0.0f;
float motorR = 0.0f;

void Kitty::uartCallback(uint8_t receivedByte) {
    switch (receivedByte) {
        case 's':
            // fctprintf(logWrite, NULL, "\nkittyStop\n", 0);
            // kitty().menu.setTriggeredOff(false);
            // kitty().motors.setValue(0, 0);
            // kitty().motors.setEnabled(false);
            // kitty().servo.set(0);
            // kitty().servo.disable();
            fctprintf(logWrite, NULL, "\nkittyPause\n", 0);
            kitty().motors.setEnabled(false);
            fctprintf(logWrite, NULL, "\nkittySV%02u\n", (uint8_t)(kitty().differential.getStartRPM() / 100));
            break;
        case 'p':
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
            kitty().newAlgorithm.clearPatterns(millis());
            kitty().servo.init();
            kitty().menu.startRace(millis());
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
        case '1': { // FORWARD
            Kitty::kitty().motors.run();

            motorL += 0.1f;
            motorR += 0.1f;

            motorL = std::clamp(motorL, -0.2f, 0.2f);
            motorR = std::clamp(motorR, -0.2f, 0.2f);

            Kitty::kitty().motors.setValue(motorL, motorR);
            break;
        }

        case '2': { // REVERSE
            Kitty::kitty().motors.run();

            motorL -= 0.1f;
            motorR -= 0.1f;

            motorL = std::clamp(motorL, -0.2f, 0.2f);
            motorR = std::clamp(motorR, -0.2f, 0.2f);

            Kitty::kitty().motors.setValue(motorL, motorR);
            break;
        }
        case '3': // right
            Kitty::kitty().servo.set(Kitty::kitty().servo.get() + 0.2f);
            break;
        case '4': // left
            Kitty::kitty().servo.set(Kitty::kitty().servo.get() - 0.2f);
            break;
        case 'x': // resetw
            fctprintf(logWrite, NULL, "kittyReset\n");
            NVIC_SystemReset();
            break;
        default: break;
    }
}

void Kitty::uartKLZCallback(uint8_t data) { Kitty::kitty().uartFrame.deserialize(&data, sizeof(data)); }

void Kitty::onKLZDataReceivedCallback(uint8_t* data, size_t length) {
    if (length == 0 || length > 2) {
        return;
    }

    uint16_t distance = 0;
    distance          = static_cast<uint16_t>(data[0] << 8) | data[1];
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
    uartDebug.initDMA();
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
    // Prepare SR04 PIT for one-shot 10us pulse generation (do not run yet)

    sr04.init();
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

uint8_t frame[147]; // 1 start + 128 danych + 1 end

bool menuActive = false;

uint8_t sr04Triggered = 0;


void Kitty::proc() {
    if (!sr04Triggered) {
        magicDiodComposition();
    }

    if (differential.isFinalStopDone()) {
        servo.set(0);
        motors.setValue(0, 0);
        return;
    }
    // TODO
    // zaimplementowac jakiegos PIDa do hamowania, aby stanąć w idealnym miejscu
    // powinien byc bardzo agresywny zwlaszcza na poczatku - nawet przy niskiej predkosci 1800
    // hamowanie z sila 0.7 jest ledwo wystarczajace, doslownie na milimetry od klocka zaczynamy sie cofac
    // tez moze trigger z sr04 jest za pozno otrzymywany?

    // Bez pida zatrzymujemy sie jakies 30 cm przed klockiem, bo cofamy za dlugo - trzeba to cofanie na pida ogarnac

    menuActive = menu.proc(millis());

    uint32_t dist = sr04.getDistanceMm();

    if (cameraDataReceived && !sr04Triggered) {
        cameraDataReceived = false;


        camera.getData(cameraDataBuf);

        float position = newAlgorithm.calculatePosition(cameraDataBuf, millis());

        ////////////////////////////// LOG ////////////////////////////////
        size_t idx   = 0;
        frame[idx++] = 0;   // start
        frame[idx++] = 255; // start
        frame[idx++] = 0;   // start
        frame[idx++] = 255; // start

        uint16_t* buffer = static_cast<uint16_t*>(cameraDataBuf);

        for (size_t i = 0; i < 128; i++) frame[idx++] = (uint8_t)(buffer[i] / 158);

        frame[132] = (uint8_t)(position + 63);

        uint16_t rpmLeft  = encoderLeft.getRPM();
        uint16_t rpmRight = encoderRight.getRPM();

        // Left RPM (2 bajty)
        frame[134] = (rpmLeft >> 8) & 0xFF; // MSB
        frame[135] = rpmLeft & 0xFF;        // LSB

        // Right RPM (2 bajty)
        frame[136] = (rpmRight >> 8) & 0xFF; // MSB
        frame[137] = rpmRight & 0xFF;        // LSB


        uint16_t startRPM = differential.getStartRPM();
        frame[138]        = (startRPM >> 8) & 0xFF;
        frame[139]        = startRPM & 0xFF;


        uint16_t sr04Distance = sr04.getDistanceMm();
        frame[140]            = (sr04Distance >> 8) & 0xFF;
        frame[141]            = sr04Distance & 0xFF;


        uint16_t diffLeftValue  = (uint16_t)((differential.getLeft() + 1) * 100);  // -1:1 -> 0:200
        uint16_t diffRightValue = (uint16_t)((differential.getRight() + 1) * 100); // -1:1 -> 0:200
        frame[142]              = (diffLeftValue >> 8) & 0xFF;
        frame[143]              = diffLeftValue & 0xFF;
        frame[144]              = (diffRightValue >> 8) & 0xFF;
        frame[145]              = diffRightValue & 0xFF;
        frame[146]              = menuActive ? 0x01 : 0x00;

        uartDebug.write((char*)frame, 147);
        //////////////////////////////////////////////////////////////////////

        if (menuActive) {
            return;
        }

        float servoPosition = -(position / 20.0f);
        if (differential.isDistanceModeActive()) servoPosition = 0;

        servo.set(servoPosition);
        differential.proc(position, millis(), dist);
        motors.setValue(differential.getLeft(), differential.getRight());
    }
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