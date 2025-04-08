#pragma once
#include "NXP_encoder.hpp"

#include <utility>

class Differential {
    int   position;
    float startVelocity;
    float valueLeft;
    float valueRight;

    uint8_t diffRatio = 160;
    uint32_t breakRatio = 60;

    uint32_t breakHoldTimer = 0;
    bool breakPunch = false;

    NXP_Encoder& encoderLeft;
    NXP_Encoder& encoderRight;

    uint8_t klzDistance = 0;
    bool distanceStopTrigger = false;

    float lastDistance = 0;
    uint32_t lastTime = 0;

    bool veryClose = false;

  public:
    Differential(float startVelocity, NXP_Encoder& encoderLeft, NXP_Encoder& encoderRight);
    void  proc(float position);
    float getLeft();
    float getRight();
    void  setStartVelocity(float value);
    float getStartVelocity();

    uint8_t getDiffRatio() { return diffRatio; }

    void setDiffRatio(uint8_t newRatio) { diffRatio = newRatio; }

    void setKlzDistance(uint8_t distance) { klzDistance = distance; }

    uint8_t getKlzDistance() { return klzDistance; }

    bool isBreakTriggered() { return distanceStopTrigger; }
};
