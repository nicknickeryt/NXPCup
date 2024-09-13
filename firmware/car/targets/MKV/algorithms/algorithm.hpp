/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Dominik Michalczyk
 *
 * Kitty algorithm 
 *
 */


#include <stdint.h>
#include <stdbool.h>
#include <cstddef>

#include "pid.hpp"

#pragma once

class Algorithm
{
private:
    bool lowPassFilterEnabled = false;
    bool initialized = false;
    PID pid;
public:
    Algorithm(){ };
    int32_t calculatePosition(uint16_t *data);
    int32_t proc(uint16_t *data);
};
