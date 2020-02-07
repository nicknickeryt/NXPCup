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

void AlgorithmUnit::analyze() {
    log_notice("ANALyzing data:");
    log_notice("Got camera data:");
    for(uint8_t i=0; i<cameraDataBufferSize; i++){
        log_notice("%d", algorithmData.cameraData[i]);
    }
}

