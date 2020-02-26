/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 *
 *
 */
#pragma once

#include "NXP_uart.hpp"
#include "pixy_packet.hpp"

class Pixy{
public:
    inline static Pixy* pixy = nullptr;
    uint8_t waitForBytes = 0;
    // uint8_t recivedBytes = 0;

    private:
        static constexpr uint16_t bufferSize = 1024;
        uint8_t txPacketBuffer[bufferSize];
        uint8_t rxPacketBuffer[bufferSize];
        uint32_t txPacketLength;
        uint32_t rxPacketLength;
        NXP_Uart& uart;
        bool lampOn;

    public:
        Pixy(NXP_Uart& uart, bool lampOn) : uart(uart), lampOn(lampOn){};

        void init();

        void control();

        void sendData(uint8_t* data, uint8_t len) {
            uart.write(data, len);
        }

        void addToBuffer(uint8_t byte) {
            rxPacketBuffer[rxPacketLength++] = byte;
        }

        void proc();

        static void byteFromUart(uint8_t byte) ;

    private:
        void sendRequest(PixyPacketRequest& packet);
        void getResponse();
};