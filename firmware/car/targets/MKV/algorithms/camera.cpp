/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Dominik Michalczyk, Kacper Cwiokowski
 *
 * Kitty algorithm
 *
 */

#include "camera.hpp"

#include <cmath>
#include <cstring>

uint32_t Algorithm::calculateBrightness(uint16_t* dataBuf) {
    uint32_t brightness = 0;
    for (size_t i = 0; i < 128; i++) {
        brightness += dataBuf[i];
    }

    float modifiedBrightness = (float)(brightness / 128) * brightnessModifier;
    return modifiedBrightness;
}

void Algorithm::differentiate(uint16_t* input, int16_t* output) {
    for (size_t i = 0; i < 128; i++) {
        output[i] = input[i] - input[i + 1];
    }
}

int32_t Algorithm::meanFilter(int32_t position) {
    float measurement = static_cast<float>(position);
    average           = average * (1 - alpha) + measurement * alpha;
    lastMeasurement   = fabsf(measurement - average) < delta ? measurement : lastMeasurement;
    return static_cast<int32_t>(lastMeasurement);
}

int32_t Algorithm::calculatePosition(uint16_t* dataBuf) {
    uint32_t brightness = calculateBrightness(dataBuf);

    // Calculate the distance from the center of the image
    uint16_t leftLinePosition  = 0;
    uint16_t rightLinePosition = 0;

    // find right line
    for (size_t i = imageWindowSize; i < cmaeraDataSize / 2; ++i) {
        if (dataBuf[i] < brightness) {
            rightLinePosition = i - imageWindowSize;
        }
    }

    // find left line
    for (size_t i = imageWindowSize; i < cmaeraDataSize / 2 - imageWindowSize; ++i) {
        if (dataBuf[cmaeraDataSize - i] < brightness) {
            leftLinePosition = i - imageWindowSize;
        }
    }

    int32_t position = leftLinePosition - rightLinePosition;

    return meanFilter(position);
}
