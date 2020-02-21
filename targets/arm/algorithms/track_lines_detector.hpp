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
    friend class AlgorithmUnit;
    private:
        uint8_t cameraDataSize;
        uint8_t lineWidth;

        // width of window used in correlation computing
        uint8_t convolutionWindowSize = cameraDataSize-lineWidth;
        // margin in pixels - if the line is found closer than this value to the boarder of camera width, line is invalid
        // example:
        //   cam left boarder  invalid line  lineSearchingMargin   valid line  valid line cameraDataSize-lineSearchingMargin invalid line cam right boarder
        //          |               x                  |                ok          ok                  |                         x             |
        constexpr static uint8_t lineSearchingMargin = 10;

        // MAGIC NUMBERS: values used to configure line searching window
        // number of pixels between lines centers
        constexpr static uint8_t spaceBetweenLinesInPixels = 63;
        // number of pixels around start line position, where it will be searched
        constexpr static uint8_t standardLineSearchingWindow = 24;
        constexpr static uint8_t widerLineSearchingWindow = 54;

        uint8_t lineSearchingWindow = standardLineSearchingWindow;

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
        void findLine(LineType line, uint16_t const* correlationDataBuffer);

    public:
        TrackLinesDetector(uint8_t cameraDataSize, uint8_t lineWidthInPixels, uint8_t blackPixelsNumberToDetectLine) :
                                                                                cameraDataSize(cameraDataSize),
                                                                                lineWidth(lineWidthInPixels){
            // magic initialization of start lines states - NIE RUSZAC, SPRAWDZONE, MA SENS
            leftLine.isDetected = false;
            leftLine.centerIndex = (cameraDataSize >> 1) - (spaceBetweenLinesInPixels >> 1);
            leftLine.leftBorderIndex = (cameraDataSize >> 1) - (spaceBetweenLinesInPixels >> 1) - (lineSearchingWindow >> 1);
            leftLine.rightBorderIndex = (cameraDataSize >> 1) - (spaceBetweenLinesInPixels >> 1) + (lineSearchingWindow >> 1);

            rightLine.isDetected = false;
            rightLine.centerIndex = (cameraDataSize >> 1) + (spaceBetweenLinesInPixels >> 1);
            rightLine.leftBorderIndex = (cameraDataSize >> 1) + (spaceBetweenLinesInPixels >> 1) - (lineSearchingWindow >> 1);
            rightLine.rightBorderIndex = (cameraDataSize >> 1) + (spaceBetweenLinesInPixels >> 1) + (lineSearchingWindow >> 1);
        }

        void detect(uint16_t const* cameraData);

};