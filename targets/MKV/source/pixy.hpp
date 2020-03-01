/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 *
 *
 */
#pragma once

#include <algorithms/algorithm_unit.hpp>
#include "NXP_uart.hpp"
#include "pixy_packet.hpp"

class Pixy{
    public:
    constexpr static auto cameraLinesSize = 640;
    constexpr static auto trackWidith = 468;
    private:
        static constexpr uint16_t bufferSize = 1024;
        static constexpr uint32_t readingTimeout = 10000000;
        uint8_t txPacketBuffer[bufferSize];
        uint8_t rxPacketBuffer[bufferSize];
        uint32_t txPacketLength;
        uint32_t rxPacketLength;
        NXP_Uart& uart;
        bool lampOn;

    public:
        Pixy(NXP_Uart& uart, bool lampOn) : uart(uart), lampOn(lampOn){};

        void init();

        void getLines(AlgorithmUnit::Line &lineLeft, AlgorithmUnit::Line &lineRight);

    private:
        void sendRequest(PixyPacketRequest& packet);
        template<typename T>
        bool getResponse(T& packet);
};