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
 
 #define brightnessCut 8
 #define alpha 0.1
 #define servoOffset -5
 
 #pragma once
 
 class Algorithm {
   private:
     bool initialized  = false;
 
     uint16_t smoothedData[128];
     uint32_t brightness = 0;
     uint8_t crossings = 0;
     uint8_t leftLinePosition = 0;
     uint8_t rightLinePosition = 0;
     float position = 63.5; 
     float filteredPosition = 63.5;
     
     void findPatterns(uint16_t* data);
 
   public:
 
     Algorithm() {};
     float calculatePosition(uint16_t* data);
     uint32_t getBrightness() const { return brightness; }
 };