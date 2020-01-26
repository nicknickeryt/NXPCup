/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 * NXP Main file
 *
 */

#include "HALina.hpp"
#include "NXP_Kitty.hpp"
#include "clock_config.h"

#define LOG_CHANNEL APP
#define APP_LOG_CHANNEL 1
#define APP_LOG_CHANNEL_LEVEL LOG_LEVEL_DEBUG

#include "logger.h"

using namespace halina;

bool flag = true;
NXP_PORT adc0mux = {PORTB, 0U, 0x00};
NXP_ADC::Sample camera1Sample = {adc0mux, NXP_ADC::ChannelSingleEnded::B_CH2};
NXP_PORT adc1mux = {PORTB, 1U, 0x00};
NXP_ADC::Sample camera2Sample = {adc1mux, NXP_ADC::ChannelSingleEnded::B_CH3};

void converterATriggered(uint8_t) {

}

void converterBTriggered(uint8_t) {

}

NXP_ADC adc = {HSADC0, converterATriggered, converterBTriggered};

void adc_triggered() {
//    flag = false;
    Kitty::kitty().ledLine.at(0).toggle();
//    Kitty::kitty().ledLine.at(0).toggle();
}

void adc_trigger(uint8_t) {
    adc.startConversion();
    Kitty::kitty().ledLine.at(1).toggle();
}

int main(){
    BOARD_BootClockRUN();
    Kitty& kitty = Kitty::kitty();
    kitty.init();

    NXP_PIT pittt = NXP_PIT(NXP_PIT::CHANNEL::_3, 400, adc_trigger);

    adc.appendSample(&camera1Sample);
    adc.appendSample(&camera1Sample);
    adc.appendSample(&camera1Sample);
    adc.appendSample(&camera1Sample);

    adc.appendSample(&camera2Sample);
    adc.appendSample(&camera2Sample);
    adc.appendSample(&camera2Sample);
    adc.appendSample(&camera2Sample);
    adc.init();


    volatile uint16_t dddd= HSADC0->CTRL2;
    volatile uint16_t ddd= HSADC0->CTRL1;
    pittt.init();


    while (true){

    }
}



