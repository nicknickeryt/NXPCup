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
    
    // Find brightest pixel of the image
    brightnessMax = data[imageCut];
    for (auto i = imageCut + 5; i < 128 - imageCut; i+= 5) {
        if(data[i] > brightnessMax)
            brightnessMax = data[i]; 
    } 
    // Calculate brightness
    brightness = brightnessMax * brightnesMaxModifier;   
   
    // Calculate the distance from the center of the image 
    for (auto i = imageCut; i < 128 - imageCut; i++) {
        if(leftLineFound == false && data[i] > brightness){
            leftLinePixel = i;       // Left line pixel at [i]
            leftLineFound = true;
        }    

        else if (leftLineFound == true && data[i] < brightness){
            rightLinePixel = i - 1;  // Right line pixel at [i]
            leftLineFound = false;
        }
    }
    
    // Calculate turning value from camera feed
    cameraTurnValuePrevious = cameraTurnValueActual;
    cameraTurnValueActual = (leftLinePixel + rightLinePixel) - 128;   

    // Check if lost the line and try to come back  
    // If turn value is (maxCameraTurnValueChange) smaller than previous turn value => use previous value
    if(cameraTurnValueActual > cameraTurnValuePrevious + maxCameraTurnValueChange){
        cameraTurnValueActual = cameraTurnValuePrevious;
    }   
    else if(cameraTurnValueActual < cameraTurnValuePrevious - maxCameraTurnValueChange){
        cameraTurnValueActual = cameraTurnValuePrevious;
    }
    
    return cameraTurnValueActual + imageCenterOffset * 2;
}

int32_t Algorithm::proc(uint16_t* data) {
    assert(data != nullptr);
    // Calculate the position of the vehicle based on the camera image
    int32_t position = calculatePosition(data);
    return pid.calculate(0, position);
}


