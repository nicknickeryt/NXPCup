/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 *
 *
 */
#include "algorithm_unit.hpp"
#include "NXP_display.hpp"
#include "NXP_Kitty.hpp"

#define LOG_CHANNEL ALGORITHM
#define ALGORITHM_LOG_CHANNEL 3
#define ALGORITHM_LOG_CHANNEL_LEVEL LOG_LEVEL_NOTICE
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

int16_t AlgorithmUnit::computeCarPositionOnTrack(){
    if(lineLeft.isDetected && lineRight.isDetected){
        carPosition = (lineRight.position + lineLeft.position) / 2;
        Kitty::kitty().display.print(10000);
    }else if(lineLeft.isDetected && !lineRight.isDetected){
        Kitty::kitty().display.print(10000);
        carPosition = Pixy::trackCenter - (lineLeft.position - Pixy::theoreticalLeftLinePosition);
    }else if(!lineLeft.isDetected && lineRight.isDetected){
        Kitty::kitty().display.print(10000);
        carPosition = Pixy::theoreticalRightLinePosition - lineRight.position + Pixy::trackCenter;
    }else{
        if(keepWheelsPositionCounter < keepWheelsPositionTime){
            Kitty::kitty().display.print(6969);
            keepWheelsPositionCounter++;
        } else{
            keepWheelsPositionCounter = 0;
            carPosition = Pixy::trackCenter;
        }
    }
    return carPosition;
}

void AlgorithmUnit::setServo(int16_t value){
    int16_t converted = 0;
    if(lineLeft.isDetected && lineRight.isDetected){
        converted = value - (float(Pixy::cameraLinesSize)/2.0);
    }else{
        converted = (float(Pixy::cameraLinesSize)/2.0) - value;
    }

    log_debug("servo: %d", value);
    log_debug("servo converted: %d", converted);
    if(converted > 0){
        auto val = float(float(converted)/(float(Pixy::cameraLinesSize)));
        servo.set(val);
        log_debug("servo: %f", val);
    }else if(converted < 0){
        auto val = float(float(converted)/(float(Pixy::cameraLinesSize)));
        servo.set(val);
        log_debug("servo: %f", val);
    }else{
        servo.set(0.0);
    }
}

void AlgorithmUnit::checkSwitches() {
}