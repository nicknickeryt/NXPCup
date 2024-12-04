/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Dominik Michalczyk, Kacper Cwiokowski
 *
 * Kitty algorithm
 *
 */

#include "A_servo.hpp"
#include <cstddef>
#include <stdbool.h>
#include <stdint.h>

#pragma once

#define IMAGE_CUT 15                               // Cut unused pixels from both sides
#define MAX_OUTPUT_CHANGE 20                       // Max allowed change between previous and actual turnvalue
#define BRIGHTNESMAX_MODIFIER 0.85                 // Multiply brigtnessMax by [value] to get brightness
#define CENTER_OFFSET 4                            // Move the center of the image (value) to the left

#define RETURN_MODIFIER 128 - CENTER_OFFSET * 2  // Move values from [0,128] to [-64,64] and add offset

class Algorithm {

  private:
    bool initialized  = false;
    PID pid;

    uint32_t brightness = 0;                        // Calculated brightness, used for finding crossings with camera data
    uint32_t brightnessMax = 0;                     // The brightest pixel value

    uint32_t crossNumber = 0;                       // Which cross are we looking for
    uint32_t firstCross = 0;                        // First cross of the camera data & brightness 
    uint32_t secondCross = 0;                       // Second 
    uint32_t thirdCross = 0;                        // Third 
    uint32_t fourthCross = 0;                       // Fourth 
   
    uint32_t cameraTurnValueFirst = 0;              // Calculated first turn value
    uint32_t cameraTurnValueSecond = 0;             // Calculated second turn value

    uint32_t cameraTurnValueActual = 0;             // Actual Turn Value
    uint32_t cameraTurnValuePrevious = 120;         // Previous turn value             
   
  public:
    Algorithm() {};
    int32_t calculatePosition(uint16_t* data);
    int32_t proc(uint16_t* data);
    int16_t getBrightness() const { return brightness; }      // Get brightness, used to print on the chart
};

