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
#include "pixy.hpp"
#include <algorithm>

void AlgorithmUnit::analyze() {
    state = State::CAMERA_DATA_PREPROCESSING;

    state = State::FINDING_TRACK_LINES;
    auto result = computeCarPositionOnTrack();

    setServo(result);

    state = State::OBSTACLE_AVOIDING;
    // avoid obstacle if any

    state = State::PATTERN_DETECTION;
    // detect patterns
}

uint16_t AlgorithmUnit::computeCarPositionOnTrack(){
    uint16_t carPosition;
    if(lineLeft.isDetected && lineRight.isDetected){
        carPosition = (lineRight.position + lineLeft.position) / 2;
    }else{
        carPosition = 0;
    }
    return carPosition;
}

void AlgorithmUnit::setServo(int16_t value){
    int16_t converted = value - (float(Pixy::cameraLinesSize)/2.0);
    log_debug("servo: %d", value);
    log_debug("servo converted: %d", converted);
    if(converted > 0){
        auto val = float(converted/(float(Pixy::trackWidith)/2.0));
        servo.set(val);
        log_debug("servo: %f", val);
    }else if(converted < 0){
        auto val = float(converted/(float(Pixy::trackWidith)/2.0));
        servo.set(val);
        log_debug("servo: %f", val);
    }else{
        servo.set(0.0);
    }
}

void AlgorithmUnit::checkSwitches() {
}