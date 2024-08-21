/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 * MKV Debug Frame implementation
 *
 */
#include <cstdint>
#include <cstring>
#include "MKV_uart.hpp"

class MKV_Frame{
private:
    struct __attribute__((__packed__)) Header{
        uint32_t checkSum;
        uint16_t payloadLength;
    } header;

    struct __attribute__((__packed__)) payload{
        uint8_t detectedSideLinesIndexes[2]{};
        int16_t motorsValues[2]{};
        int16_t encodersValues[2]{};
        int16_t servoValue = 0;
        uint8_t detectedObstacleSide = 0;
        bool detectedSpeedUpPattern = false;
        bool detectedSpeedDownPattern = false;
        bool detectedEmergencyBreakPattern = false;
        bool detectedStopPattern = false;
        bool crossroadDetected = false;
    } payload;

    MKV_Uart &uart;
    uint8_t serializeBuffer[100];

public:
    MKV_Frame(MKV_Uart &uart) : header{0xFFFFFFFF, sizeof(payload)}, uart(uart){}

    void sendFrameProc(){
        memcpy(serializeBuffer, &header, sizeof(header));
        memcpy(serializeBuffer + sizeof(header), &payload, header.payloadLength);
        uart.write(serializeBuffer, sizeof(header) + header.payloadLength);
        clearPayload();
    }

    void setPayload(uint8_t *lines, int16_t *motors, int16_t *encoders, int16_t servo, uint8_t obstacle, bool speedUp, bool speedDown, bool emergencyBreak, bool stop, bool crossroad){
        memcpy(payload.detectedSideLinesIndexes, lines, sizeof(payload.detectedSideLinesIndexes));
        memcpy(payload.motorsValues, motors, sizeof(payload.motorsValues));
        memcpy(payload.encodersValues, encoders, sizeof(payload.encodersValues));
        payload.servoValue = servo;
        payload.detectedObstacleSide = obstacle;
        payload.detectedSpeedUpPattern = speedUp;
        payload.detectedSpeedDownPattern = speedDown;
        payload.detectedEmergencyBreakPattern = emergencyBreak;
        payload.detectedStopPattern = stop;
        payload.crossroadDetected = crossroad;
    }

private:
    void clearPayload(){
        payload = {{}, {}, {}, false, false, false, false};
    }

};