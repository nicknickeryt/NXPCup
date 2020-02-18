/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 *
 *
 */

#include "track_lines_detector.hpp"

#define LOG_CHANNEL TRACK
#define TRACK_LOG_CHANNEL 4
#define TRACK_LOG_CHANNEL_LEVEL LOG_LEVEL_DEBUG

#include "logger.h"

void TrackLinesDetector::detect(int16_t* cameraData){
    if(cameraData != nullptr){
        findLine(LineType::LEFT, cameraData);
        findLine(LineType::RIGHT, cameraData);

        // make sure than found lines are not outside of the window (too close to the camera window edge)
        if(leftLine.leftBorderIndex < lineSearchingMargin){
            leftLine.isDetected = false;
        }
        if(rightLine.rightBorderIndex > cameraDataSize-lineSearchingMargin){
            rightLine.isDetected = false;
        }
    }
}

void TrackLinesDetector::findLine(LineType lineType, int16_t* data){
    Line* line;
    uint8_t startIndex = 0;
    uint8_t stopIndex = 128;
    int16_t maxValue = 0;
    int32_t sum = 0;
    int32_t average = 0;
    uint8_t maxValueIndex = 0;

    if(data != nullptr){
        switch(lineType){
            case LineType::LEFT:
                startIndex = 0;
                stopIndex = 63;
                line = &leftLine;
                break;
            case LineType::RIGHT:
                startIndex = 64;
                stopIndex = 127;
                line = &rightLine;
                break;
            default:
                break;
        }

        // find max value
        for(auto i = line->leftBorderIndex; i < line->rightBorderIndex; i++){
            if(data[i] > maxValue){
                maxValue = data[i];
                maxValueIndex = i;
            }
        }

        // get average from all pixels
        for(auto i = startIndex; i <= stopIndex; i++){
            sum += data[i];
        }
        average = sum / cameraDataSize;

        // decide if value is higher than current average
        if((maxValue - average) > lineDetectedthreshold){
            line->isDetected = true;
            line->centerIndex = maxValueIndex;
            lineSearchingWindow = standardLineSearchingWindow;
        }else{
            line->isDetected = false;
        }
    }
}