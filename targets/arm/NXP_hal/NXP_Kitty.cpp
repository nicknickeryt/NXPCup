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
bool cameraTrigger = false;
void pit_sendCameraData(uint8_t) {
    cameraTrigger = true;
}


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
    intr.init();
    intl.init();
    log_notice("Witaj swiecie!");
    uartCommunication.write("Bejbi don't hurt me", 19);
    log_notice("KiTTy init finished");
    servo.set(0.1);
    camera.start();
//    distanceSensor.begin();

    sdaPort.setMux();
    sclPort.setMux();

    i2c_master_config_t masterConfig;
    I2C_MasterGetDefaultConfig(&masterConfig);
    masterConfig.baudRate_Bps = 100000;
    I2C_MasterInit(My_I2C,  &masterConfig, 24000000);
    I2C_MasterClearStatusFlags(My_I2C, 0xFF);
    I2C_Enable(My_I2C, true);
//
    if (!sensor.init()) {
        log_error("Failed to detect and initialize sensor!");
//        while (1) {}
    } else {
        log_notice("Czujnik ok");
    }


//    i2c.init();
}

void Kitty::proc() {
    magicDiodComposition();
    display.update();
    camera.proc(cameraTrigger);

    static int x;
    if(100000 <= x++) {

        uint16_t y = sensor.readRangeSingleMillimeters();
        log_notice("result: %d", y);
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

