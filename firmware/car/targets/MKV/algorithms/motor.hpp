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

  public:
    Differential(float startVelocity, NXP_Encoder& encoderLeft, NXP_Encoder& encoderRight);
    void  proc(float position);
    float getLeft();
    float getRight();
    void  setStartVelocity(float value);
    float getStartVelocity();

    uint8_t getDiffRatio() { return diffRatio; }

    void setDiffRatio(uint8_t newRatio) { diffRatio = newRatio; }
};
