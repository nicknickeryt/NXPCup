#include "motor.hpp"

#include <utility>

#include <cmath>

/*
 *@brief set velocity adequate to current conditions on track
 *@param startVelocity is the default speed of the car
 *@param position is the current position of the car on the track
 */

Differential::Differential(float startVelocityValue, NXP_Encoder& encoderLeft, NXP_Encoder& encoderRight) : startVelocity(startVelocityValue), encoderLeft(encoderLeft), encoderRight(encoderRight) {}

void Differential::proc(float position) {
    
    float breakComponent = (abs(position) / breakRatio);
    float diffComponent = (abs(position) / diffRatio);

    if(abs(position) > 13 && breakHoldTimer < 5000) {
        valueLeft = 0;
        valueRight = 0;
        breakPunch = true;
        breakHoldTimer++;
        return; 
    }

    if (position >= 0) {
        valueLeft  = startVelocity - breakComponent;
        valueRight = startVelocity - diffComponent - breakComponent;
    } else if (position < 0) {
        valueRight = startVelocity - breakComponent;
        valueLeft  = startVelocity - diffComponent - breakComponent;
    }

    if(abs(position) < 2) {
        breakPunch = false;
        breakHoldTimer = 0;
    }
}

float Differential::getLeft() { return valueLeft; }
float Differential::getRight() { return valueRight; }

void  Differential::setStartVelocity(float value) { startVelocity = value; }
float Differential::getStartVelocity() { return startVelocity; }
