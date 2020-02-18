/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 *
 *
 */
#include "algorithm_unit.hpp"

#define LOG_CHANNEL ALGORITHM
#define ALGORITHM_LOG_CHANNEL 3
#define ALGORITHM_LOG_CHANNEL_LEVEL LOG_LEVEL_DEBUG
#include "logger.h"
#define ARM_MATH_CM7 1
#include "arm_math.h"

void AlgorithmUnit::analyze() {
    state = State::CAMERA_DATA_PREPROCESSING;
    filter(algorithmData.cameraData, 3);
    diff(algorithmData.cameraData);
    diff(algorithmData.cameraData);
//    normalize(DataType::CAMERA_DATA, algorithmData.cameraData);
//    quantization(algorithmData.cameraData);

    // fixme: DEBUG
    uint8_t camera1DataBuffer[260];
    camera1DataBuffer[0] = 0xff;
    camera1DataBuffer[1] = 0xff;
    camera1DataBuffer[2] = 0xff;
    camera1DataBuffer[3] = 0xff;
    memcpy(&camera1DataBuffer[4], algorithmData.cameraData, 256);
    debug.write(camera1DataBuffer, 260);

    state = State::FINDING_TRACK_LINES;
    // find track lines
    trackLinesDetector.detect((int16_t*)algorithmData.cameraData);

    auto result = computeCarPositionOnTrack();
    setServo(result);

    state = State::OBSTACLE_AVOIDING;
    // avoid obstacle if any

    state = State::PATTERN_DETECTION;
    // detect patterns
    patternsDetector.detect(algorithmData.cameraData);
}

void AlgorithmUnit::diff(uint16_t* data){
    int16_t diff[cameraDataBufferSize];
    for(auto i=0; i<cameraDataBufferSize; i++){
        diff[i] = (int16_t)data[i] - (int16_t)data[i+1];
    }
    diff[cameraDataBufferSize-1] = 0;

    memcpy(data, diff, sizeof(diff));
}

void AlgorithmUnit::filter(uint16_t* data, uint8_t maxCount){
    uint16_t filteredData[cameraDataBufferSize]{};
    uint32_t sum = 0;
    for(auto i = 0; i < cameraDataBufferSize; i++){
        sum = 0;
        if(((i + maxCount/2) <= (cameraDataBufferSize - 1)) && ((i - maxCount/2) >= 0)) {
            for (auto j = i-(maxCount/2); j < i + (maxCount/2); j++) {
                sum += data[j];
            }
            filteredData[i] = sum / maxCount;
        }else{
            filteredData[i] = data[i];
        }
    }
    memcpy(data, filteredData, sizeof(filteredData));
}

void AlgorithmUnit::normalize(AlgorithmUnit::DataType dataType, uint16_t* data) {
    // normalize data in other way depending on its type
    switch(dataType){
        case DataType::CAMERA_DATA: {
            uint32_t maximalPixelBrightness = 0;
            uint32_t minimalPixelBrightness = 65535;
            uint32_t meanPixelBrightness = 0;
            uint32_t summaryValue = 0;
            uint16_t *cameraValues = data;

            //Find minimum  and maximum brightness of pixels
            for (auto i = 0; i < cameraDataBufferSize; i++) {
                if (cameraValues[i] < minimalPixelBrightness) {
                    minimalPixelBrightness = cameraValues[i];
                }
                if (cameraValues[i] > maximalPixelBrightness) {
                    maximalPixelBrightness = cameraValues[i];
                }
                summaryValue += cameraValues[i];
            }

            const uint16_t minMaxDelta = maximalPixelBrightness - minimalPixelBrightness;

            if(minMaxDelta != 0){
                // calculate mean value
                meanPixelBrightness = summaryValue / cameraDataBufferSize;
                meanPixelBrightness = ((meanPixelBrightness - minimalPixelBrightness) * cameraDataNormalizationFactor) /
                                      (maximalPixelBrightness - minimalPixelBrightness);

                // normalize
                for (auto i = 0; i < cameraDataBufferSize; i++) {
                    //Normalisation
                    cameraValues[i] = ((cameraValues[i] - minimalPixelBrightness) * cameraDataNormalizationFactor) /
                                      (maximalPixelBrightness - minimalPixelBrightness)
                                      - meanPixelBrightness;
                }
            }
            }
            break;

        default:
            break;
    }
}

void AlgorithmUnit::quantization(uint16_t *data) {
    // iterate through the data and assign 1 to the values above threshold and 0 to values below
    for (auto i = 0; i < cameraDataBufferSize; i++) {
        if (data[i] >= blackOrWhitePixelThreshold) {
            data[i] = 1;
        } else{
            data[i] = 0;
        }
    }
}

int8_t AlgorithmUnit::computeCarPositionOnTrack(){
    // both lines are detected
    if(trackLinesDetector.leftLine.isDetected && trackLinesDetector.rightLine.isDetected){
        log_notice("both %d, %d", trackLinesDetector.leftLine.centerIndex, trackLinesDetector.rightLine.centerIndex);
        carPosition = (trackLinesDetector.leftLine.centerIndex + trackLinesDetector.rightLine.centerIndex)/2;
    } // only left line is detected
    else if(trackLinesDetector.leftLine.isDetected && !trackLinesDetector.rightLine.isDetected){
        log_notice("left, %d", trackLinesDetector.leftLine.centerIndex);
        carPosition = (trackLinesDetector.leftLine.centerIndex + trackLinesDetector.lineWidth + (cameraDataBufferSize/4) - lostLineOffset);
    }// only right line is detected
    else if(!trackLinesDetector.leftLine.isDetected && trackLinesDetector.rightLine.isDetected){
        log_notice("right, %d", trackLinesDetector.rightLine.centerIndex);
        carPosition = (trackLinesDetector.rightLine.centerIndex - trackLinesDetector.lineWidth - (cameraDataBufferSize/4) + lostLineOffset);
    } // no line is detected
    else{
        log_notice("none");
        if(keepPreviousPositionCounter > keepPreviousPositionTime){
            carPosition = ((carPosition - (cameraDataBufferSize >> 1)) >> 1) + (cameraDataBufferSize >> 1);
            keepPreviousPositionCounter = 0;
        }
        keepPreviousPositionCounter++;
        trackLinesDetector.lineSearchingWindow = trackLinesDetector.widerLineSearchingWindow;
        trackLinesDetector.leftLine.centerIndex = carPosition - (trackLinesDetector.lineWidth >> 1) - (trackLinesDetector.spaceBetweenLinesInPixels >> 1);
        trackLinesDetector.rightLine.centerIndex = carPosition - (trackLinesDetector.lineWidth >> 1) + (trackLinesDetector.spaceBetweenLinesInPixels >> 1);
    }
    return carPosition;
}

void AlgorithmUnit::setServo(int8_t value){
    int16_t converted = value - (cameraDataBufferSize/2);
    if(converted > 0){
        auto val = float(converted/(float(cameraDataBufferSize)/2.0));
        servo.set(val);
    }else if(converted < 0){
        auto val = float(converted/(float(cameraDataBufferSize)/2.0));
        servo.set(val);
    }else{
        servo.set(0.0);
    }
}