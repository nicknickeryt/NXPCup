/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 *
 *
 */

#pragma once

#include "HALina.hpp"

class TrackLinesDetector{
    private:
        uint8_t cameraDataSize;
        uint8_t lineWidth;
        // width of window used in correlation computing
        uint8_t convolutionWindowSize = cameraDataSize-lineWidth;
        // margin in pixels - if the line is found closer than this value to the boarder of camera width, line is invalid
        // example:
        //   cam left boarder  invalid line  lineSearchingMargin   valid line  valid line cameraDataSize-lineSearchingMargin invalid line cam right boarder
        //          |               x                  |                ok          ok                  |                         x             |
        constexpr static uint8_t lineSearchingMargin = 20;

        // fixme: UWAGA ODTAD W DOL MAMY MAGICZNE PARAMTERY, KTORYCH RACZEJ NIE MOZEMY RUSZAC
        constexpr static int32_t linePattern[] ={ 3847, 1005, -2679, -4344, -2679, 1005, 3847 };
        // number of pixels between lines centers
        constexpr static uint8_t spaceBetweenLinesInPixels = 63;
        // number of pixels around start line position, where it will be searched
        constexpr static uint8_t standardLineSearchingWindow = 24;
        constexpr static uint8_t widerLineSearchingWindow = 54;
        uint8_t lineSearchingWindow = standardLineSearchingWindow;
        // value of convolution with camera data to assume that line is detected
        constexpr static uint32_t lineConvolutionThreshold = 9000000;

        enum class LineType{
            LEFT,
            RIGHT
        };

    public:
        class Line{
            public:
                bool isDetected;
                uint8_t centerIndex;
                uint8_t leftBorderIndex;
                uint8_t rightBorderIndex;
        };

        Line leftLine;
        Line rightLine;

    private:
        /**
         * Method calculates correlation between camera data and some fixed lines patterns.
         * It uses convolution operation to get correlation results. If some areas have high enough
         * correlation factor, they are assumed as lines.
         *
         * @param cameraData - pointer to buffer with camera data
         * @param correlationResults - pointer to buffer where correlation results will be stored
         */
        void computeDataAndLinePatternsCorrelation(uint16_t* cameraData, uint16_t* correlationResults);

        void findLine(LineType line, uint16_t* correlationDataBuffer);

    public:
        TrackLinesDetector(uint8_t cameraDataSize, uint8_t lineWidthInPixels) : cameraDataSize(cameraDataSize), lineWidth(lineWidthInPixels){
            // magic initialization of start lines states
            leftLine.isDetected = false;
            leftLine.centerIndex = (cameraDataSize >> 1) - (spaceBetweenLinesInPixels >> 1);
            leftLine.leftBorderIndex = (cameraDataSize >> 1) - (spaceBetweenLinesInPixels >> 1) - (lineSearchingWindow >> 1);
            leftLine.rightBorderIndex = (cameraDataSize >> 1) - (spaceBetweenLinesInPixels >> 1) + (lineSearchingWindow >> 1);

            rightLine.isDetected = false;
            rightLine.centerIndex = (cameraDataSize >> 1) + (spaceBetweenLinesInPixels >> 1);
            rightLine.leftBorderIndex = (cameraDataSize >> 1) + (spaceBetweenLinesInPixels >> 1) - (lineSearchingWindow >> 1);
            rightLine.rightBorderIndex = (cameraDataSize >> 1) + (spaceBetweenLinesInPixels >> 1) + (lineSearchingWindow >> 1);
        }

        void detect(uint16_t* cameraData);

};