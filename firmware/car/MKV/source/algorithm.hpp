/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Dominik Michalczyk
 *
 * Kitty algorithm 
 *
 */

#pragma once

#include <cstdint>

class algorithm
{
private:
    bool initialized = false;
public:
    void init(uint16_t* data);
    void proc();
};
