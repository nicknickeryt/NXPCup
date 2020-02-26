/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 *
 *
 */
#include "Pixy.hpp"

#define LOG_CHANNEL PIXY
#define PIXY_LOG_CHANNEL 5
#define PIXY_LOG_CHANNEL_LEVEL LOG_LEVEL_DEBUG

#include "logger.h"

bool Pixy::getData(){

//    uint8_t request[] = {0xae,0xc1, 0x0e, 0x00 };
//    uint8_t byte;
//
//    if(!uart.isBufferEmpty()){
//        log_notice("Got frame from PIXY");
//        while(!uart.isBufferEmpty()){
//            byte = uart.read();
//            log_notice("0x%x", byte);
//        }
//    } else{
//        uart.write(request, sizeof(request));
//    }

//    uart.write(request, sizeof(request));
//
//    while(uart.isBufferEmpty());
//
//    uint8_t byte = uart.read();
//    log_notice("Got frame from PIXY");

//    if(!uart.isBufferEmpty()){
//        uint8_t byte = uart.read();
//        if(byte == startOfFrame) {
//            log_notice("Got frame from PIXY");
//            return true;
//        }
//    }
//    uint8_t byte;
//    if(!uart.isBufferEmpty()){
//        log_notice("Got frame from PIXY");
//        while(!uart.isBufferEmpty()){
//            byte = uart.read();
//            log_notice("0x%x", byte);
//        }
//    } else{
        sendRequest();
 //  }

    return false;
}

void Pixy::sendRequest(){
    static uint8_t counter;
    uint8_t request[] = {0xae,0xc1, 20, 3, 0, 255, counter };
    counter++;
    uart.write(request, sizeof(request));
    for(volatile uint32_t i=0; i<1000000; i++){;}
}