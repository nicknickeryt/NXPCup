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

#define ALGORITHM_IMAGE_CUT 15
#define ALGORITHM_MAX_CAM_TURN_VAL 20
#define ALGORITHM_BRI_MAX_MOD 0.75
#define ALGORITHM_IMAGE_CENTER_OFF 120

class Algorithm {
  private:
    bool initialized  = false;
    PID pid;

    ////////////////////////////////////// My values //////////////////////////////////////////////////////////

    const uint32_t imageCut = 15;                   // Cut unused pixels from both sides
    const uint32_t maxCameraTurnValueChange = 20;    // Max allowed change between previous and actual turnvalue
    const float brightnesMaxModifier = 0.75;        // Multiply brigtnessMax by [value] to get brightness     
    const uint32_t imageCenterOffset = 120;         // Set the center of the image(4 to the left) (128 - 2 * 4)

    ////////////////////////////////// Calculated values //////////////////////////////////////////////////////

    uint32_t brightness    = 0;                     // Calculated brightness, used for finding crossings with camera data
    uint32_t brightnessMax = 0;                     // The brightest pixel value

    uint32_t crossNumber = 0;                       // Which cross are we looking for
    uint32_t firstCross  = 0;                       // First cross of the camera data & brightness 
    uint32_t secondCross = 0;                       // Second 
    uint32_t thirdCross  = 0;                       // Third 
    uint32_t fourthCross = 0;                       // Fourth 
   
    bool firstLineFound  = false;                   // Check if first line is found
    bool secondLineFound = false;                   // Check if second line is found

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

