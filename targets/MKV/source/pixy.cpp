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
    Pixy::pixy = this;
    uart.init();
    uart.setRedirectHandler(Pixy::byteFromUart);

//    if(lampOn){
//        log_debug("Turning on lamps");
//        SetLampRequest setLampRequest(1,1);
//        sendRequest(setLampRequest);
//    } else{
//        log_debug("Turning off lamps");
//        SetLampRequest setLampRequest(0,0);
//        sendRequest(setLampRequest);
//    }

    getResponse();

    // uint8_t data [] = {0xAE, 0xC1, 0x1C, 0};
    // sendData(data, sizeof(data));
}

void Pixy::control(){

}

void Pixy::sendRequest(PixyPacketRequest& packet){
    log_debug("Sending request to PIXY");
    packet.serialize(txPacketBuffer, &txPacketLength);
    uart.write(txPacketBuffer, txPacketLength);
}

void Pixy::getResponse(){
    waitForBytes = 12;
    rxPacketLength = 0;
    uint8_t data [] = {0xAE, 0xC1, 0x1C, 0};
    sendData(data, sizeof(data));
}

void Pixy::proc() {
    if (rxPacketLength == waitForBytes) {
        rxPacketLength = 0;
        uint16_t response[3];
        memcpy(response, &rxPacketBuffer[6], 6);
        log_debug("mam %d %d %d", response[0], response[1], response[2]);
    }
}

void Pixy::byteFromUart(uint8_t byte) {
    pixy->addToBuffer(byte);
}