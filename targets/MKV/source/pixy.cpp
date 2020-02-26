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

void Pixy::init(){
    if(lampOn){
        log_debug("Turning on lamps");
        SetLampRequest setLampRequest(1,1);
        sendRequest(setLampRequest);
    } else{
        log_debug("Turning off lamps");
        SetLampRequest setLampRequest(0,0);
        sendRequest(setLampRequest);
    }
}

void Pixy::control(){

}

void Pixy::sendRequest(PixyPacketRequest& packet){
    log_debug("Sending request to PIXY");
    packet.serialize(txPacketBuffer, &txPacketLength);
    uart.write(txPacketBuffer, txPacketLength);
}

void Pixy::getResponse(PixyPacketResponse& packet){

}