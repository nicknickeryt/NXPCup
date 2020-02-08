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
bool sendCameraData = false;
void pit_sendCameraData(uint8_t) {
//    sendCameraData = true;
}
uint8_t camera1DataBuffer [258];


void Kitty::init() {
    BOARD_InitBootPins();
    BOARD_InitBootClocks();

    FTM_Init();
    uartDebug.init();
    uartCommunication.init();
    uartCommunication.initDMA();
    ledLine.init();
    display.init();
    servo.init();
    camera.init();
    pitCamera.init();
    pitSendCameraData.init();
//    intr.init();
//    intl.init();
    log_notice("Witaj swiecie!");
    uartCommunication.write("Bejbi don't hurt me", 19);
    log_notice("KiTTy init finished");


    motors.init();
    motors.run();


//    motorRight.init();
//    motorRight.run();
//
//    motorLeft.init();
//    motorLeft.run();

//    motorRight.setValue(-0.5f);
//
    camera1DataBuffer[0] = 'A';
    camera1DataBuffer[1] = 'B';
    camera.start();
}

void Kitty::proc() {
    magicDiodComposition();
    display.update();

    if (sendCameraData) {
        sendCameraData = false;
        __disable_irq();
        //memset(camera.buffer1Data, 666, 128);
        memcpy(&camera1DataBuffer[2], camera.buffer1Data, 256);
        __enable_irq();

//        uartCommunication.appendDMA(camera1DataBuffer, 258);

        uartCommunication.write(camera1DataBuffer, sizeof(camera1DataBuffer));
    }

    static float value = 0.0;
    static int x;
    static bool direction = true;
    if(50000 == x++) {
        servo.set(value);
//        motorRight.setValue(value);
//        motorLeft.setValue(value);

//        motors.setValue(value, value);
//        display.print(value, 2);m 1
        if(direction){
            value += 0.01;
        }else{
            value -= 0.01;
        }
        if(value >= 1.0 || value <= -1.0) {
            direction = !direction;
        }
        x = 0;
    }
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