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
    LineNodeRequest lineNodeRequest(3);
    sendRequest(lineNodeRequest);

    LineNodeResponse lineNodeResponse;
    getResponse<LineNodeResponse>(lineNodeResponse);
    log_debug("%d", lineNodeResponse.payload.data1);
    log_debug("%d", lineNodeResponse.payload.data2);
    log_debug("%d", lineNodeResponse.payload.data3);
}

void Pixy::sendRequest(PixyPacketRequest& packet){
    log_debug("Sending request to PIXY");
    packet.serialize(txPacketBuffer, &txPacketLength);
    uart.flushRxBuffer();
    uart.write(txPacketBuffer, txPacketLength);
}

template<typename T>
void Pixy::getResponse(T& packet){
    volatile uint16_t bytesToRead;
    switch(packet.header.type){
        case PacketType::LINE_NODE_RESPONSE: {
            // first get only header, inside it there is info about payload size
            bytesToRead = sizeof(packet.header);
            // wait for given number of bytes
            while(bytesToRead != uart.getBufferLevel()){
                asm("nop");
            }
            uart.read(rxPacketBuffer, bytesToRead);
            packet.getHeader(rxPacketBuffer);
            // get payload, now we know the size
            bytesToRead = packet.header.payloadLength;
            // wait for given number of bytes
            while(bytesToRead != uart.getBufferLevel()){
                asm("nop");
            }
            uart.read(rxPacketBuffer, bytesToRead);
            packet.deserialize(rxPacketBuffer);
            }
            break;
        default:
            break;
    }
}
