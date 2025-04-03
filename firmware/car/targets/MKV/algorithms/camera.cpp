/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2025
 * Authors: Kacper Cwiokowski
 *
 * Kitty algorithm
 *
 */

 #include "camera.hpp"

 #include <assert.h>
 float Algorithm::calculatePosition(uint16_t* data) {
     assert(data != nullptr);
     
     // Brightness
     brightness = 0;
     for (auto i = (brightnessCut - 1); i <= (127 - brightnessCut); i++){
        brightness += data[i]; 
     }  

    brightness /= (127 - (2 * brightnessCut));

 
     // Right line position
     for (auto i = 0; i < ( (position / 2) - 10); i++){
        if(data[i] < brightness){
            rightLinePosition = i;
        }
     }
     
 
     // Left line position
     for (auto i = 127; i > ( (position / 2) + 10); i--){
        if(data[i] < brightness){
           leftLinePosition = i;
        }
     }
 
     // Calculte position
     position = (leftLinePosition + rightLinePosition);
 
     // TODO - speed-adjusted alpha
     float rpmAverage = ((float) encoderLeft.getRPM() + (float) encoderRight.getRPM()) / 2.0f;
     //  float rpmAlpha = (0.001996 * rpmAverage) + 0.01;
     float rpmAlpha = -exp(- rpmAverage / 500) + 1;

     if(rpmAlpha < 0) rpmAlpha = 0;

     // Lo pass filter position
     filteredPosition = filteredPosition * (1 - (alpha)) + ((static_cast<float>(position) / 2.0f) - 60.5f) * alpha;
 
     // Find patterns
     // findPatterns(data);

     // Return filtered position 
     return (filteredPosition);
 }
 
 void Algorithm::findPatterns(uint16_t* data) {
     // Smoothing
     for (auto i = 1; i <= 126; i++) {
         smoothedData[i]= (data[i-1] + data[i] + data[i+1]) / 3;     
     } 
 
     // Crossings with brightness
     for (auto i = 1; i <= 125; i++) {
         // down-up crossing
         if(smoothedData[i-1] < brightness && smoothedData[i] < brightness && smoothedData[i+1] > brightness && smoothedData[i+2] > brightness){
             crossings++;
         }
         // up-down crossing
         if(smoothedData[i-1] > brightness && smoothedData[i] > brightness && smoothedData[i+1] < brightness && smoothedData[i+2] < brightness){
             crossings++;
         }
     }
 
     // Find Patterns
     if(crossings > 6){
     // Pattern found
     }
    
 }