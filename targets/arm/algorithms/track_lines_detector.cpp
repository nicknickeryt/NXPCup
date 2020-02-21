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

void TrackLinesDetector::detect(uint16_t const* cameraData){
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

void TrackLinesDetector::findLine(LineType lineType, uint16_t const* data){
    Line* line;
    uint8_t startIndex = 0;
    uint8_t stopIndex = 128;

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
        line->isDetected = false;
        for(auto i = line->leftBorderIndex; i < line->rightBorderIndex; i++){
            if(data[i] != 0){
                line->isDetected = true;
                line->centerIndex = i - (lineWidth/2);
                lineSearchingWindow = standardLineSearchingWindow;
                break;
            }
        }
    }
}