/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2025
 * Authors: Kacper Cwiokowski
 *
 * Kitty algorithm
 *
 */

#include "camera.hpp"



float Algorithm::calculatePosition(uint16_t* data) {

    // Brightness
    brightness = 0;

    for (size_t i = 0; i != cameraBufferWith; i++) {
        brightness += (data[i] / cameraBufferWith);
    }

    // Right line position
    rightLineRange = ((position / 2) - lineSearchOffset);

    for (size_t i = 0; i != rightLineRange; i++) {
        if (data[i] < brightness) {
            rightLinePosition = i;
        }
    }

    // Left line position
    leftLineRange = ((position / 2) + lineSearchOffset);

    for (size_t i = (cameraBufferWith - 1); i != leftLineRange; i--) {
        if (data[i] < brightness) {
            leftLinePosition = i;
        }
    }

    // Calculte position
    position = (leftLinePosition + rightLinePosition);
    adjustedPosition = (static_cast<float>(position) / 2.0f) - halfCameraBufferWith;

    // Lo pass filter position
    filteredPosition = (filteredPosition * (1 - alpha)) + ((adjustedPosition + algorithmOffset) * alpha);

    // Return filtered position
    return (filteredPosition);
}

void Algorithm::findPatterns(uint16_t* data) {
    // Smoothing
    for (auto i = 1; i <= 126; i++) {
        smoothedData[i] = (data[i - 1] + data[i] + data[i + 1]) / 3;
    }

    // Crossings with brightness
    for (auto i = 1; i <= 125; i++) {
        // down-up crossing
        if (smoothedData[i - 1] < brightness && smoothedData[i] < brightness && smoothedData[i + 1] > brightness && smoothedData[i + 2] > brightness) {
            crossings++;
        }
        // up-down crossing
        if (smoothedData[i - 1] > brightness && smoothedData[i] > brightness && smoothedData[i + 1] < brightness && smoothedData[i + 2] < brightness) {
            crossings++;
        }
    }

    // Find Patterns
    if (crossings > 6) {
        // Pattern found
    }
}