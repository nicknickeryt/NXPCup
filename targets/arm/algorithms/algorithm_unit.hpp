/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 *
 *
 */

#pragma once

#include "HALina.hpp"

class AlgorithmUnit{
    private:
        static constexpr auto cameraDataBufferSize = 128;

    public:
        class AlgorithmData{
            public:
                uint16_t cameraData[cameraDataBufferSize];
                //fixme: przykladowe dane
                uint16_t velocityRightMotor;
                uint16_t velocityLeftMotor;
        };
        AlgorithmData algorithmData;

    private:

    public:
        AlgorithmUnit(){}

        void analyze();
};
