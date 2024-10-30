#pragma once
#include <utility>

#define ENGINE_CUT 60.0 // enter the square of engine cut value based on camera algorithm return value (float)
// be careful of what value you enter (A > max_position^2)
#define ENGINE_CUT_SQ ENGINE_CUT*ENGINE_CUT
#define ENGINE_BREAKE ENGINE_CUT * 2

class Differential {
    int position;
    float startVelocity;
    float valueLeft;
    float valueRight;

  public:
    Differential(float startVelocity);
    void proc(int position);
    float getLeft();
    float getRight();
};
