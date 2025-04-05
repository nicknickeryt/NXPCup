/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2025
 * Authors: Kacper Cwiokowski
 *
 * Kitty algorithm
 *
 */

#include "NXP_encoder.hpp"

#include <cstddef>
#include <stdbool.h>
#include <stdint.h>

#define brightnessCut 0
#define alpha         0.1


#pragma once

class Algorithm {
	private:
    	bool                   initialized          = false;
    	constexpr static int   lineSearchOffset     = 10;
    	constexpr static int   cameraBufferWith     = 128;
    	constexpr static int   halfCameraBufferWith = cameraBufferWith / 2;
   	 	constexpr static float algorithmOffset      = 3.5f;

    	uint16_t               smoothedData[128]    = {0};
    	uint32_t               brightness           = 0;
   		uint32_t               crossings            = 0;
   		uint32_t               leftLinePosition     = 0;
		uint32_t               rightLinePosition    = cameraBufferWith;
		uint32_t               leftLineRange        = halfCameraBufferWith;
		uint32_t               rightLineRange       = halfCameraBufferWith;
		uint32_t               position             = halfCameraBufferWith;
		float                  adjustedPosition     = halfCameraBufferWith;
		float                  filteredPosition     = halfCameraBufferWith;

		NXP_Encoder& encoderLeft;
		NXP_Encoder& encoderRight;

		void findPatterns(uint16_t* data);

  public:
    Algorithm(NXP_Encoder& encoderLeft, NXP_Encoder& encoderRight) : encoderLeft(encoderLeft), encoderRight(encoderRight) {};
    float    calculatePosition(uint16_t* data);
    uint32_t getBrightness() const { return brightness; }
};