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

class PatternsDetector{
    private:
        // distance from left line center from which patterns finding algorithm starts sweeping
        // example:
        //     |    .    |   x
        //    lllb llc  llrb d
        // lllb - left line left border
        // llc - left line center
        // llrb - left line right border
        // d - sweep start point
        // distance = d - llc
        constexpr static auto distanceFromLeftLineCenterInPixels = 7;
        // same as upper one
        constexpr static auto distanceFromRightLineCenterInPixels = 3;

        TrackLinesDetector::Line& leftTrackLine;
        TrackLinesDetector::Line& rightTrackLine;

    private:
        uint16_t findMaximalCameraValue(uint16_t* cameraData);

    public:
        PatternsDetector(TrackLinesDetector::Line& leftTrackLine, TrackLinesDetector::Line& rightTrackLine) : leftTrackLine(leftTrackLine), rightTrackLine(rightTrackLine){}

        void detect(uint16_t* cameraData);
};