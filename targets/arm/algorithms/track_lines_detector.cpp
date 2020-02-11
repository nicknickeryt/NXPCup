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
        uint16_t correlationResultsBuffer[convolutionWindowSize];
        computeDataAndLinePatternsCorrelation(cameraData, correlationResultsBuffer);
        findLine(LineType::LEFT, correlationResultsBuffer);
        findLine(LineType::RIGHT, correlationResultsBuffer);

        // make sure than found lines are not outside of the window (too close to the camera window edge)
        if(leftLine.leftBorderIndex < lineSearchingMargin){
            leftLine.isDetected = false;
            log_debug("Found left line");
        }
        if(rightLine.rightBorderIndex > cameraDataSize-lineSearchingMargin){
            rightLine.isDetected = false;
            log_debug("Found right line");
        }
    }
}

void TrackLinesDetector::computeDataAndLinePatternsCorrelation(uint16_t* cameraData, uint16_t* correlationResults){
    int32_t summary=0;
    // calculate convolution of camera data and line pattern
    for(auto i=0; i<convolutionWindowSize; i++){
        summary += cameraData[i] * linePattern[0];
        summary += cameraData[i + 1] * linePattern[1];
        summary += cameraData[i + 2] * linePattern[2];
        summary += cameraData[i + 3] * linePattern[3];
        summary += cameraData[i + 4] * linePattern[4];
        summary += cameraData[i + 4] * linePattern[5];
        summary += cameraData[i + 6] * linePattern[6];
        correlationResults[i] = summary;
        //log_notice("%d");
    }
}

void TrackLinesDetector::findLine(LineType lineType, uint16_t* correlationDataBuffer){
    Line* line;
    if(correlationDataBuffer != nullptr){
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
        float tempValue = 0;
        uint16_t tempIndex;
        if(line->rightBorderIndex != line->leftBorderIndex){
            // iterate through the all values from left to right border to find maximum, it is a center
            for(auto i = line->leftBorderIndex; i < line->rightBorderIndex; i++){
                if(correlationDataBuffer[i] >= tempValue){
                    tempValue = correlationDataBuffer[i];
                    tempIndex = i;
                }
            }
            //log_notice("Tempval: %d", tempValue);
            // checking if found maximum is bigger than convolution threshold to assume line is detected
            if ((tempValue >= lineConvolutionThreshold)) {
                line->centerIndex = (line->centerIndex + tempIndex) >> 1;
                line->isDetected = true;
                lineSearchingWindow = standardLineSearchingWindow;
            }
        }
    }
}