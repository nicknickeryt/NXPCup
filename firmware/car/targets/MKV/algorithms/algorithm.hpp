/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Dominik Michalczyk
 *
 * Kitty algorithm
 *
 */


#include "pid.hpp"

#include <cstddef>
#include <stdbool.h>
#include <stdint.h>

#pragma once

class Algorithm {
  private:
    bool initialized          = false;
    PID  pid;

    const uint32_t imageCut   = 10;
    const uint32_t bightnessBias = 5000;

  public:
    Algorithm() {};
    int32_t calculatePosition(uint16_t* data);
    int32_t proc(uint16_t* data);
};
