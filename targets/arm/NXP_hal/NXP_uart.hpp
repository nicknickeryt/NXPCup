/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 * NXP HALina implementation of uart
 *
 */

#pragma once

#include "HALina.hpp"

class NXP_Uart : public halina::UART{
public:
    void init() override;

    void write(void const*) override;

    char read() override;

    void proc();
};