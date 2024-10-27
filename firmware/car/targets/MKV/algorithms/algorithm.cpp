/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Dominik Michalczyk, Kacper Cwiokowski
 *
 * Kitty algorithm
 *
 */

#include "algorithm.hpp"
#include <assert.h>

enum {
    HALF_CAMERA_WIDTH = 63,
};

#ifdef ALGORITHM_LOWPASS_EN
static void lowPassFilter(uint16_t* data, const uint32_t size) {
    uint32_t    previousData = 0;
    const float alpha        = 0.9f;
    for (uint32_t i = 0; i < size; i++) {
        uint32_t temp = data[i];
        data[i]       = (1 - alpha) * data[i] + (alpha)*previousData;
        previousData  = temp;
    }
}
#endif


int32_t Algorithm::calculatePosition(uint16_t* data) {
    assert(data != nullptr);
    
    // Apply low pass filter on camera image
    #ifdef ALGORITHM_LOWPASS_EN
    lowPassFilter(data, 128);
    #endif

    // Calculate average brightness of the left half of the image
    brightness = 0;
    for (auto i = imageCut; i < HALF_CAMERA_WIDTH + imageCut; i++) {
        brightness += data[i] / 64; // tu ma byc int
    }
    // brightness += bightnessBias; //mnozymy zamiast dodawac
   
    // Calculate the distance from the center of the image
    uint32_t leftSideDistance  = 0;  // Distance from the center
    uint32_t rightSideDistance = 0;  // Distance from the center
    for (auto i = imageCut; i < 64; i++) {
        if (data[i] < brightness) 
          leftSideDistance = i;
        if (data[127 - i] < brightness) 
          rightSideDistance = i + 1;
    }

    return leftSideDistance - rightSideDistance;
}

int32_t Algorithm::proc(uint16_t* data) {
    assert(data != nullptr);
    // Calculate the position of the vehicle based on the camera image
    int32_t position = calculatePosition(data);
    return pid.calculate(0, position);
}


