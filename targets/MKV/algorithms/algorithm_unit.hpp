/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 *
 *
 */

#pragma once

#include "HALina.hpp"
#include "track_lines_detector.hpp"
#include "obstacle_detector.hpp"
#include "patterns_detector.hpp"
#include "NXP_uart.hpp"
#include "NXP_servo.hpp"
#include "pid.hpp"

class AlgorithmUnit{
    friend class Kitty;
public:
    class Line{
    public:
        bool isDetected = false;
        uint16_t position = 0;
    };
private:

    enum class DataType{
        CAMERA_DATA,
        ENCODERS_DATA,
        // fixme: other data
    };

    enum class State{
        CAMERA_DATA_PREPROCESSING,
        FINDING_TRACK_LINES,
        OBSTACLE_AVOIDING,
        PATTERN_DETECTION,
    };

    State state;
    float speed = 0.2;
    Line lineRight{};
    Line lineLeft{};
    PID pid ={0.01, 0.0, 0.7, 0.3, 454.375,-71.875};

    NXP_Servo& servo;
    NXP_Uart& debug;
    halina::Switches &switches;

public:
    class AlgorithmData{
    public:
    };
    AlgorithmData algorithmData;

private:
    int16_t computeCarPositionOnTrack();

    void setServo(int16_t value);

public:
    AlgorithmUnit(NXP_Servo& servo, NXP_Uart& debug, halina::Switches &switches) : servo(servo), debug(debug), switches(switches){}

    void analyze();

    void checkSwitches();
};
