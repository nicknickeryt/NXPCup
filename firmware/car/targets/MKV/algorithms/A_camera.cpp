/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Dominik Michalczyk, Kacper Cwiokowski
 *
 * Kitty algorithm
 *
 */

#include "A_camera.hpp"
#include <assert.h>
#include <cstdlib> 

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
I
    // Find brightest pixel of the image
    brightnessMax = data[IMAGE_CUT];
    for (auto i = IMAGE_CUT + 5; i < 128 - IMAGE_CUT; i+= 5) {
        if(data[i] > brightnessMax)
            brightnessMax = data[i]; 
    } 
    // Calculate brightness
    brightness = brightnessMax * BRIGHTNESMAX_MODIFIER;   
   
    // Calculate crossings with brightness 
    for (auto i = IMAGE_CUT; i < 128 - IMAGE_CUT; i++) {
        switch (crossNumber) {
            case 1:{
                 if(data[i] > brightness){
                    firstCross  = i;
                    crossNumber = 2;
                }
                break;
            }
             case 2:{
                 if(data[i] < brightness){
                    secondCross  = i - 1;
                    crossNumber = 3;
                }
                break;
            }     
            case 3:{
                if(data[i] > brightness){
                    thirdCross  = i;
                    crossNumber = 4;
                }
                break;
            }
            case 4:{
                if(data[i] < brightness){
                    fourthCross  = i - 1;
                }
                break;
            }
        } 
    } 
      
    // Calculate center between two crosses (for each line)
    cameraTurnValueFirst = firstCross + secondCross;
    cameraTurnValueSecond = thirdCross + fourthCross;

    // Check if cameraTurnValueSecond is used
    if(crossNumber < 4){
        cameraTurnValueActual = cameraTurnValueFirst;
    }
    // Chose closest value to the last value
    else{ 
        if(abs(cameraTurnValueSecond - cameraTurnValueActual) < abs(cameraTurnValueFirst - cameraTurnValueActual)){
            cameraTurnValueActual = cameraTurnValueSecond;
        }
        else{
            cameraTurnValueActual = cameraTurnValueFirst;
        }
    }

    // Reset 
    crossNumber = 1; 
    
    // Check if lost the line and try to come back  
    // If turn value is (maxCameraTurnValueChange) smaller than previous turn value => use previous value
    if(cameraTurnValueActual > cameraTurnValuePrevious + MAX_OUTPUT_CHANGE){
        cameraTurnValueActual = cameraTurnValuePrevious;
    }   
    else if(cameraTurnValueActual < cameraTurnValuePrevious - MAX_OUTPUT_CHANGE){
        cameraTurnValueActual = cameraTurnValuePrevious;
    }
    
    // Save last value
    cameraTurnValuePrevious = cameraTurnValueActual;
    
    return cameraTurnValueActual - RETURN_MODIFIER;
}

int32_t Algorithm::proc(uint16_t* data) {
    assert(data != nullptr);
    // Calculate the position of the vehicle based on the camera image
    int32_t position = calculatePosition(data);
    return pid.calculate(0, position);
}


