#pragma once
#include <utility>

#define ENGINE_CUT 50.0 // enter the square of engine cut value based on camera algorithm return value (float)
// be careful of what value you enter (A > max_position^2)
#define ENGINE_CUT_SQ ENGINE_CUT * ENGINE_CUT
#define ENGINE_BRAKE ENGINE_CUT * 1.5

class Differential {
    int   position;
    float startVelocity;
    float valueLeft;
    float valueRight;

  public:
    Differential(float startVelocity);
    void  proc(int position);
    float getLeft();
    float getRight();
    void  setStartVelocity(float value);
    float getStartVelocity();
};
