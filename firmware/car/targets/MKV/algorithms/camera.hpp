/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2025
 * Authors: Kacper Cwiokowski
 *
 * Kitty algorithm
 *
 */

#include "NXP_encoder.hpp"
#include "NXP_Params.hpp"
#include "motor.hpp"

#include <cstddef>
#include <stdbool.h>
#include <stdint.h>

#define brightnessCut 0

#pragma once

class Algorithm {
	private:
    	bool                    initialized          = false;
    	constexpr static int    lineSearchOffset     = 10;
    	constexpr static size_t cameraBufferWith     = 128;
    	constexpr static int    halfCameraBufferWith = cameraBufferWith / 2;
   	 	constexpr static float  algorithmOffset      = 1.7f;

    	uint16_t                smoothedData[128]    = {0};
    	uint32_t                brightness           = 0;
		uint32_t 				brightnessCrossThreshold = 0;
    	uint32_t                brightnessMean 	  	 = 0;
   		uint8_t                 crossings            = 0;
   		uint32_t                leftLinePosition     = 0;
		uint32_t                rightLinePosition    = cameraBufferWith;
		uint32_t                leftLineRange        = halfCameraBufferWith;
		uint32_t                rightLineRange       = halfCameraBufferWith;
		uint32_t                position             = halfCameraBufferWith;
		float                   adjustedPosition     = halfCameraBufferWith;
		float                   filteredPosition     = halfCameraBufferWith;

		uint16_t				filteredData[128]      = {0};

		NXP_Encoder& encoderLeft;
		NXP_Encoder& encoderRight;
		
		Differential& differential;
		NXP_Params& params;

		uint32_t algorithmStartTime = 0;

		uint32_t patternDetectTimeoutMs = 1500;
		uint32_t patternsAmount = 0;
		bool patternDetected = false;
		uint32_t patternsStartTime = 0;
		bool findPatterns(uint16_t* data, uint32_t currentMillis);
		bool findCrossings(uint16_t* data);

  public:
    Algorithm(NXP_Encoder& encoderLeft, NXP_Encoder& encoderRight, Differential& differential, NXP_Params& params) : encoderLeft(encoderLeft), encoderRight(encoderRight), differential(differential), params(params) {};
    float    calculatePosition(uint16_t* data, uint32_t currentMillis);

    uint32_t getBrightness() const { return brightness; }
	uint32_t getBrightnessCrossThreshold() const { return brightnessCrossThreshold; }
	uint32_t getLeftLinePosition() const { return leftLinePosition; }
	uint32_t getRightLinePosition() const { return rightLinePosition; }

	uint8_t getCrossings() const { return crossings; }
	bool isPatternDetected() const { return patternDetected; }

	void clearPatterns(uint32_t currentMillis);

	void setAlgorithmStartTime(uint32_t currentMillis);
};