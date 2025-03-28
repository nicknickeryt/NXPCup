/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2025
 * Authors: Kacper Cwiokowski
 *
 * Kitty algorithm
 *
 */

 #include <cstddef>
 #include <stdbool.h>
 #include <stdint.h>

 #include "NXP_encoder.hpp"
 
 #define brightnessCut 0
 #define alpha 0.2
 
 
 #pragma once
 
 class Algorithm {
    private:
     bool initialized  = false;
 
     uint16_t smoothedData[128];
     uint32_t brightness = 0;
     uint8_t crossings = 0;
     uint8_t leftLinePosition = 0;
     uint8_t rightLinePosition = 0;
     uint8_t position = 63; 
     float filteredPosition = 63;

     NXP_Encoder& encoderLeft;
     NXP_Encoder& encoderRight;
     
     void findPatterns(uint16_t* data);
 
   public:
 
     Algorithm(NXP_Encoder& encoderLeft, NXP_Encoder& encoderRight) : encoderLeft(encoderLeft), encoderRight(encoderRight) {};
     float calculatePosition(uint16_t* data);
     uint32_t getBrightness() const { return brightness; }
 };