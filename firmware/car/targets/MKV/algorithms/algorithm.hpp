/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Dominik Michalczyk, Kacper Cwiokowski
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
    bool initialized  = false;
    PID pid;

    const uint32_t imageCut = 15;                                      // Cut unused pixels from both sides
    const uint32_t imageCenterOffset = 4;                              // Set the center of the image(4 to the left)
    const uint32_t brightnessBias = 5000;  
    uint32_t brightness = 0;

  public:
    Algorithm() {};
    int32_t calculatePosition(uint16_t* data);
    int32_t proc(uint16_t* data);
    int16_t getBrightness() const { return brightness; }
};

