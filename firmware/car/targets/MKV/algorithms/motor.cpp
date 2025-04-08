#include "motor.hpp"

#include "NXP_Kitty.hpp"

#include <utility>

#include <cmath>

/*
 *@brief set velocity adequate to current conditions on track
 *@param startVelocity is the default speed of the car
 *@param position is the current position of the car on the track
 */

Differential::Differential(float startVelocityValue, NXP_Encoder& encoderLeft, NXP_Encoder& encoderRight) : startVelocity(startVelocityValue), encoderLeft(encoderLeft), encoderRight(encoderRight) {}

void Differential::proc(float position) {
    float    currentDistance = klzDistance;
    uint32_t currentTime     = Kitty::kitty().millis();

    if (currentDistance <= 120 && currentDistance != 0) { // jesli jestesmy bardzo blisko to cala wstecz
        veryClose  = true; 
        valueLeft  = -1.0f; 
        valueRight = -1.0f;
        distanceStopTrigger = true;
        return;
    }

    if (veryClose && currentDistance > 80) {
        valueLeft  = 0;
        valueRight = 0;
        veryClose = false;
        distanceStopTrigger = true;
    }

    if (!veryClose && currentDistance < 220 && currentDistance != 0) {
        float dt           = (currentTime - lastTime) / 1000.0f; 
        float dDistance    = (lastDistance - currentDistance);   
        float distanceRate = (dt > 0) ? dDistance / dt : 0.0f;

        float targetDistance = 250.0f;                           
        float error          = targetDistance - currentDistance;
        float Kp             = -1.2f;
        float Kd             = -0.4f;

        float control = (Kp * error) + (Kd * distanceRate);

        if (control > 0) control = 0;
        if (control < -0.6f) control = -0.6f;

        valueLeft           = control;
        valueRight          = control;
        distanceStopTrigger = true;

        lastDistance = currentDistance;
        lastTime     = currentTime;

        return;
    }

    lastDistance = currentDistance;
    lastTime     = currentTime;

    if (distanceStopTrigger) {
        valueLeft  = 0;
        valueRight = 0;
        return;
    }


    float breakComponent = (abs(position) / breakRatio);
    float diffComponent  = (abs(position) / diffRatio);

    if (abs(position) > 13 && breakHoldTimer < 5000) {
        valueLeft  = 0;
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

    if (abs(position) < 2) {
        breakPunch     = false;
        breakHoldTimer = 0;
    }
}

float Differential::getLeft() { return valueLeft; }
float Differential::getRight() { return valueRight; }

void  Differential::setStartVelocity(float value) { startVelocity = value; }
float Differential::getStartVelocity() { return startVelocity; }
