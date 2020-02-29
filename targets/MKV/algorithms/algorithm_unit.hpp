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

class AlgorithmUnit{
private:
    static constexpr auto cameraDataBufferSize = 128;
    // factor used to spread normalization output from range(0,1) to wider (0,1000)
    static constexpr auto cameraDataNormalizationFactor = 1000;
    static constexpr auto lostLineOffset = 5;
    static constexpr auto keepPreviousPositionTime = 500;

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
    TrackLinesDetector trackLinesDetector;
    ObstacleDetector obstacleDetector;
    PatternsDetector patternsDetector;
    uint8_t carPosition{cameraDataBufferSize / 2};
    uint16_t keepPreviousPositionCounter{0};
    uint16_t threshold = 0;

    bool darkBackgroundMode = false;

    NXP_Servo& servo;
    NXP_Uart& debug;
    halina::Switches &switches;

public:
    float startSpeed = 0.2;

public:
    class AlgorithmData{
    public:
        uint16_t cameraData[cameraDataBufferSize];
        //fixme: przykladowe dane
        uint16_t velocityRightMotor;
        uint16_t velocityLeftMotor;
    };
    AlgorithmData algorithmData;

private:
    /**
     * Method normalizes given data using min-max normalization algorithm
     * It works by given equation:
     * f(x) = (x- min(x))/(max(x)-min(x))
     * This is the traditional min-max algorithm but it gives output in range(0,1).
     * For spreading this values, the normalization factor is used (look at cameraDataNormalizationFactor)
     * This type of normalization is called normalization by linear function
     *
     * @param dataType type of data to normalize
     * @param data pointer to data buffer to normalize
     */
    void normalize(DataType dataType, uint16_t* data);

    void quantization(uint16_t* data);

    void filter(uint16_t* data, uint8_t maxCount);

    void diff(int16_t* data);

    void setThreshold(uint16_t* data);

    int8_t computeCarPositionOnTrack();

    void setServo(int8_t value);

    void deleteUnusedLines(uint16_t* data);

public:
    AlgorithmUnit(NXP_Servo& servo, NXP_Uart& debug, halina::Switches &switches) : trackLinesDetector(cameraDataBufferSize, 7),
                                                                                   patternsDetector(trackLinesDetector.leftLine, trackLinesDetector.rightLine), servo(servo), debug(debug), switches(switches){}

    void analyze();

    void checkSwitches();
};
