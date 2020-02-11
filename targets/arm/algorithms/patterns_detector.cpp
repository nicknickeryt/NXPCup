/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 *
 *
 */
#include "patterns_detector.hpp"

void PatternsDetector::detect(uint16_t* cameraData){
    if(cameraData != nullptr){
        uint16_t maxCameraValue = findMaximalCameraValue(cameraData);

    }
}

uint16_t PatternsDetector::findMaximalCameraValue(uint16_t* cameraData){
    uint16_t maxValue = 0;
    if(cameraData != nullptr){
        for(auto i = leftTrackLine.centerIndex + distanceFromLeftLineCenterInPixels; i < rightTrackLine.centerIndex - distanceFromRightLineCenterInPixels; i++){
            if(cameraData[i] > maxValue){
                maxValue = cameraData[i];
            }
        }
    }
    return maxValue;
}