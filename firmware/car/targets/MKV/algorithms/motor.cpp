#include "motor.hpp"

#include <utility>

#include <cmath>

/*
 *@brief set velocity adequate to current conditions on track
 *@param startVelocity is the default speed of the car
 *@param position is the current position of the car on the track
 */

Differential::Differential(float startVelocityValue, NXP_Encoder& encoderLeft, NXP_Encoder& encoderRight)
 : startVelocity(startVelocityValue), encoderLeft(encoderLeft), encoderRight(encoderRight) {}

void Differential::proc(float position) {
    if (position >= 0) {
        valueRight = startVelocity;
        valueLeft  = startVelocity - (abs(position) * DIFF_RATIO);
    } else if (position < 0) {
        valueLeft  = startVelocity;
        valueRight = startVelocity - (abs(position) * DIFF_RATIO);
    }
}

float Differential::getLeft() { return valueLeft; }
float Differential::getRight() { return valueRight; }

void  Differential::setStartVelocity(float value) { startVelocity = value; }
float Differential::getStartVelocity() { return startVelocity; }
