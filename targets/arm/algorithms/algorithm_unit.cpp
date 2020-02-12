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

void AlgorithmUnit::analyze() {
    state = State::CAMERA_DATA_PREPROCESSING;
    normalize(DataType::CAMERA_DATA, algorithmData.cameraData);
    quantization(algorithmData.cameraData);

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
    trackLinesDetector.detect(algorithmData.cameraData);
    if(trackLinesDetector.leftLine.isDetected){
        log_notice("Left line - centerIndex: %d", trackLinesDetector.leftLine.centerIndex);
    }
    if(trackLinesDetector.rightLine.isDetected){
        log_notice("Right line - centerIndex: %d", trackLinesDetector.rightLine.centerIndex);
    }

    state = State::OBSTACLE_AVOIDING;
    // avoid obstacle if any

    state = State::PATTERN_DETECTION;
    // detect patterns
    patternsDetector.detect(algorithmData.cameraData);
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