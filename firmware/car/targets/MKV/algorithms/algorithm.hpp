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
    ///////////////////////////////// My values //////////////////////////////////////////////////////////
    const uint32_t imageCut = 15;                  // Cut unused pixels from both sides
    const int32_t maxCameraTurnValueChange = 40;  // Max allowed change between previous and actual turnvalue
    const float brightnesMaxModifier = 0.75;       // Multiply brigtnessMax by [value] to get brightness     
    const uint32_t imageCenterOffset = 4;          // Set the center of the image(4 to the left)

    ///////////////////////////// Calculated values //////////////////////////////////////////////////////
    uint32_t leftLinePixel  = 0;                   // Left line pixel 
    uint32_t rightLinePixel = 0;                   // Right lane pixel
    uint32_t brightness = 0;                       // Calculated brightness, used for finding black lines
    uint32_t brightnessMax = 0;                    // The brightest pixel value
    bool leftLineFound = false;                    // Check if left line is found
    int32_t cameraTurnValueActual = 0;             // Calculated turn value
    int32_t cameraTurnValuePrevious = 0;           // Previous turn value             
   


  public:
    Algorithm() {};
    int32_t calculatePosition(uint16_t* data);
    int32_t proc(uint16_t* data);
    int16_t getBrightness() const { return brightness; }      // Get brightness
};

