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
    // send request about line nodes
    LineNodeRequest lineNodeRequest(3);
    sendRequest(lineNodeRequest);

    // wait for response about line nodes
    LineNodeResponse lineNodeResponse;
    // check if response is valid
    if(getResponse<LineNodeResponse>(lineNodeResponse)){
        log_debug("Got response");
        for(uint32_t i = 0; i<(lineNodeResponse.header.payloadLength/sizeof(uint16_t)); i++){
            log_debug("Point nr: %d, value: %d", i, lineNodeResponse.payload.points[i]);
        }
    } else{
        log_debug("Get response failed");
    }

}

void Pixy::sendRequest(PixyPacketRequest& packet){
    log_debug("Sending request to PIXY");
    packet.serialize(txPacketBuffer, &txPacketLength);
    uart.flushRxBuffer();
    uart.write(txPacketBuffer, txPacketLength);
}

template<typename T>
bool Pixy::getResponse(T& packet){
    volatile uint16_t bytesToRead;
    uint32_t timeout = 0;
    switch(packet.header.type){
        case PacketType::LINE_NODE_RESPONSE: {
            // first get only header, inside it there is info about payload size
            bytesToRead = sizeof(packet.header);
            // wait for given number of bytes
            while(bytesToRead != uart.getBufferLevel()){
                asm("nop");
                timeout++;
                // timeout guard to avoid locking
                if(timeout >= readingTimeout){
                    return false;
                }
            }
            timeout = 0;
            uart.read(rxPacketBuffer, bytesToRead);
            packet.getHeader(rxPacketBuffer);
            // get payload, now we know the size
            bytesToRead = packet.header.payloadLength;
            // wait for given number of bytes
            while(bytesToRead != uart.getBufferLevel()){
                asm("nop");
                timeout++;
                // timeout guard to avoid locking
                if(timeout >= readingTimeout){
                    return false;
                }
            }
            uart.read(rxPacketBuffer, bytesToRead);
            packet.deserialize(rxPacketBuffer);
            }
            break;
        default:
            break;
    }
    return true;
}
