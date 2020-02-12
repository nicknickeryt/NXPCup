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

class AlgorithmUnit{
    private:
        static constexpr auto cameraDataBufferSize = 128;
        // factor used to spread normalization output from range(0,1) to wider (0,8192)
        static constexpr auto cameraDataNormalizationFactor = 1000; //8192;
        //
        static constexpr auto blackOrWhitePixelThreshold = 55000;

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

        NXP_Uart& debug;

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

    public:
        AlgorithmUnit(NXP_Uart& debug) : trackLinesDetector(cameraDataBufferSize, 7, 3),
                                         patternsDetector(trackLinesDetector.leftLine, trackLinesDetector.rightLine),
                                         debug(debug){}

        void analyze();
};
