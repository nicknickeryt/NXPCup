#pragma once
#include <utility>

#include "NXP_encoder.hpp"

#define DIFF_RATIO 0.007

class Differential {
    int   position;
    float startVelocity;
    float valueLeft;
    float valueRight;

    NXP_Encoder& encoderLeft;
    NXP_Encoder& encoderRight;

  public:
    Differential(float startVelocity, NXP_Encoder& encoderLeft, NXP_Encoder& encoderRight);
    void  proc(float position);
    float getLeft();
    float getRight();
    void  setStartVelocity(float value);
    float getStartVelocity();
};
