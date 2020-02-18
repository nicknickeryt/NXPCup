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

void TrackLinesDetector::detect(uint16_t* cameraData){
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

void TrackLinesDetector::findLine(LineType lineType, uint16_t* data){
    Line* line;
    bool wasPreviousPixelBlack = false;
    // index of first black pixel found
    uint8_t firstBlackPixelIndex = 0;
    uint16_t blackPixelsCounter = 0;
    if(data != nullptr){
        switch(lineType){
            case LineType::LEFT:
                line = &leftLine;
                break;
            case LineType::RIGHT:
                line = &rightLine;
                break;
            default:
                break;
        }

        line->isDetected = false;
        if(line->rightBorderIndex != line->leftBorderIndex){
            // iterate through the all values from left to right border to find maximum, it is a center
            for(auto i = line->leftBorderIndex; i < line->rightBorderIndex; i++){
                // check if given pixel is black or white (1 or 0)
                if(data[i] == 1){
                    // check if previous one was also black
                    if(wasPreviousPixelBlack) {
                        blackPixelsCounter++;
                    } else {
                        // if it wasn't start counting black pixels start
                        blackPixelsCounter = 1;
                        // remember first black pixel index
                        firstBlackPixelIndex = i;
                        wasPreviousPixelBlack = true;
                    }
                } else{
                    wasPreviousPixelBlack = false;
                }
                // if you found enough black pixels and another one is white (black line ended), break
                if((blackPixelsCounter >= blackPixelsNumberToDetectLine) && (!wasPreviousPixelBlack)){
                    line->isDetected = true;
                    line->centerIndex = firstBlackPixelIndex + (blackPixelsCounter/2);
                    lineSearchingWindow = standardLineSearchingWindow;
                    break;
                }
            }

        }
    }
}