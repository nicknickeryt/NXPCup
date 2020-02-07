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
    // preprocessing data from camera
    normalize(DataType::CAMERA_DATA, algorithmData.cameraData);

    state = State::FINDING_TRACK_LINES;
    // find track lines

    state = State::OBSTACLE_AVOIDING;
    // avoid obstacle if any

    state = State::PATTERN_DETECTION;
    // detect patterns
}

void AlgorithmUnit::normalize(AlgorithmUnit::DataType dataType, void *data) {
    // normalize data in other way depending on its type
    switch(dataType){
        case DataType::CAMERA_DATA: {
            int32_t maximalPixelBrightness = 0;
            int32_t minimalPixelBrightness = 65535;
            int32_t meanPixelBrightness = 0;
            int32_t summaryValue = 0;
            int32_t *cameraValues = reinterpret_cast<int32_t *>(data);

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
            // calculate mean value
            meanPixelBrightness = summaryValue / cameraDataBufferSize;
            meanPixelBrightness = ((meanPixelBrightness - minimalPixelBrightness) * (cameraDataNormalizationFactor)) /
                                  (maximalPixelBrightness - minimalPixelBrightness);

            // normalize
            for (auto i = 0; i < cameraDataBufferSize; i++) {
                //Normalisation
                cameraValues[i] = ((cameraValues[i] - minimalPixelBrightness) * cameraDataNormalizationFactor) /
                                  (maximalPixelBrightness - minimalPixelBrightness)
                                  - meanPixelBrightness;
            }
            }
            break;
        case DataType::ENCODERS_DATA:
            break;
        default:
            break;
    }
}
