/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2025
 * Authors: Kacper Cwiokowski
 *
 * Kitty algorithm
 *
 */

#include "camera.hpp"
#include "motor.hpp"


float Algorithm::calculatePosition(uint16_t* data, uint32_t currentMillis) {
    // Brightness
    brightness = 0;

    for (size_t i = 0; i != cameraBufferWith; i++) brightness += (data[i] / cameraBufferWith);

    // Right line position
    rightLineRange = ((position / 2) - lineSearchOffset);

    for (size_t i = 0; i != rightLineRange; i++)
        if (data[i] < brightness) rightLinePosition = i;

    // Left line position
    leftLineRange = ((position / 2) + lineSearchOffset);

    for (size_t i = (cameraBufferWith - 1); i != leftLineRange; i--)
        if (data[i] < brightness) leftLinePosition = i;

    // Calculte position
    position         = (leftLinePosition + rightLinePosition);
    adjustedPosition = (static_cast<float>(position) / 2.0f) - halfCameraBufferWith;

    // Lo pass filter position
    filteredPosition = (filteredPosition * (1 - alpha)) + ((adjustedPosition + algorithmOffset) * alpha);

    // if(findPatterns(data, currentMillis))
    //     differential.setPatternDetected(true);

    // Return filtered position
    return (filteredPosition);
}

bool Algorithm::findPatterns(uint16_t* data, uint32_t currentMillis) {

    // if(currentMillis - algorithmStartTime < patternDetectTimeoutMs) return false;

    crossings = 0;
    // Smoothing
    for (auto i = 1; i <= 126; i++) smoothedData[i] = (data[i - 1] + (5*data[i]) + data[i + 1]) / 8;

    // Crossings with brightness
    for (auto i = 1; i <= 125; i++) {
        // down-up crossing
        if (smoothedData[i - 1] < brightness && smoothedData[i] < brightness && smoothedData[i + 1] > brightness && smoothedData[i + 2] > brightness) crossings++;

        // up-down crossing
        if (smoothedData[i - 1] > brightness && smoothedData[i] > brightness && smoothedData[i + 1] < brightness && smoothedData[i + 2] < brightness) crossings++;
    }

    // Find Patterns
    if (crossings > 6) {
        return true;

        if (!patternDetected) {
            patternDetected = true;
            patternsStartTime = currentMillis;
            // return true;
        }
        if (currentMillis - patternsStartTime > 5) {
            patternDetected = false;
            return true;
        }
    } else
        patternDetected = false;

    return false;
}

void Algorithm::clearPatterns(uint32_t currentMillis) {
    patternDetected = false;
    patternsStartTime = currentMillis;
    differential.clearAllFlags();
}

void Algorithm::setAlgorithmStartTime(uint32_t currentMillis) {
    algorithmStartTime = currentMillis;
}