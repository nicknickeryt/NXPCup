/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Dominik Michalczyk, Kacper Cwiokowski
 *
 * Kitty algorithm
 *
 */

#include "servo.hpp"

#include <cstddef>
#include <stdbool.h>
#include <stdint.h>

#pragma once

class Algorithm {
  private:
    constexpr static uint8_t imageWindowSize    = 15;                     // Cut unused pixels from both sides
    constexpr static uint8_t maxOutputChange    = 5;                     // Max allowed change between previous and actual turnvalue
    constexpr static float   brightnessModifier = 1.0f;                   // Multiply brigtnessMax by [value] to get brightness
    constexpr static uint8_t centerOffset       = 4;                      // Move the center of the image (value) to the left
    constexpr static uint8_t returnModifier     = 128 - centerOffset * 2; // Move values from [0,128] to [-64,64] and add offset

    constexpr static float alpha           = 0.05; // Filter parameter
    constexpr static float delta           = 5; // Filter parameter
    float                  average         = 0;   // Average value of the measurements
    float                  lastMeasurement = 0;   // Last measurement


    uint32_t brightness = 0; // Calculated brightness, used for finding crossings with camera data

    uint32_t crossNumber = 0; // Which cross are we looking for
    uint32_t firstCross  = 0; // First cross of the camera data & brightness
    uint32_t secondCross = 0; // Second
    uint32_t thirdCross  = 0; // Third
    uint32_t fourthCross = 0; // Fourth

    uint32_t cameraTurnValueFirst  = 0; // Calculated first turn value
    uint32_t cameraTurnValueSecond = 0; // Calculated second turn value

    uint32_t cameraTurnValueActual   = 0;   // Actual Turn Value
    uint32_t cameraTurnValuePrevious = 120; // Previous turn value

    int32_t lastPosition = 0;

  public:
    Algorithm(){};
    int32_t calculatePosition(uint16_t* data);
    int16_t getBrightness() const { return brightness; } // Get brightness, used to print on the chart

  private:
    uint32_t calculateBrightness(uint16_t* data);
    void     differentiate(uint16_t* input, int16_t* output);
    int32_t  meanFilter(int32_t measurement);
};
