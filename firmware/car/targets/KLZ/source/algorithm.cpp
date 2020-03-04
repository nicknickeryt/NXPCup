/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 * Algorithm for KL25z
 *
 */


#define LOG_CHANNEL KITTY
#define KITTY_LOG_CHANNEL 2
#define KITTY_LOG_CHANNEL_LEVEL LOG_LEVEL_NOTICE

#include "logger.h"
#include "algorithm.h"

void Algorithm::init() {
    for(auto i = 0; i < 4; i++){
        ledDebug.at(i).init();
    }
    if(!right.init()){
        log_error("right sensor is not connected!");
    }
//    if(!rightFront.init()){
//        log_error("rightFront sensor is not connected!");
//    }
//    if(!left.init()){
//        log_error("left sensor is not connected!");
//    }
//    if(!leftFront.init()){
//        log_error("leftFront sensor is not connected!");
//    }
}

void Algorithm::proc(volatile bool &trigger) {
    uint16_t rightValue = UINT8_MAX - 1;
    uint16_t rightFrontValue = UINT8_MAX - 1;
    uint16_t leftValue = UINT8_MAX - 1;
    uint16_t leftFrontValue = UINT8_MAX - 1;
    uint16_t additional_1Value = UINT8_MAX - 1;
    uint16_t additional_2Value = UINT8_MAX - 1;
    uint8_t data[sensorNumber];
    memset(data, 0, sizeof(data));
    if(trigger) {
        rightValue = right.readRangeSingleMillimeters();
//        rightFrontValue = rightFront.readRangeSingleMillimeters();
//        leftValue = left.readRangeSingleMillimeters();
//        leftFrontValue = leftFront.readRangeSingleMillimeters();
        log_debug("Sensors: %d - %d - %d - %d - %d - %d", rightValue, rightFrontValue, leftValue, leftFrontValue, additional_1Value, additional_2Value);
        if(leftValue <= sendThreshold){
            ledDebug.at(0).set();
            data[0] = leftValue;
        }else{
            ledDebug.at(0).reset();
        }
        if(rightValue <= sendThreshold){
            data[3] = rightValue;
            ledDebug.at(3).set();
        }else{
            ledDebug.at(3).reset();
        }
        if(leftFrontValue <= sendThreshold){
            data[2] = leftFrontValue;
            ledDebug.at(2).set();
        }else{
            ledDebug.at(2).reset();
        }
        if(rightFrontValue <= sendThreshold){
            data[1] = rightFrontValue;
            ledDebug.at(1).set();
        }else{
            ledDebug.at(1).reset();
        }
        frame.send(data);
        trigger = false;
    }
}