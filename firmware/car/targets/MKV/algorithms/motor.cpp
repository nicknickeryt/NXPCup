#include "motor.hpp"

#include "pid.hpp"

#include <utility>

#include <cmath>

/*
 *@brief set velocity adequate to current conditions on track
 *@param startVelocity is the default speed of the car
 *@param position is the current position of the car on the track
 */

Differential::Differential(float startRPMValue, NXP_Encoder& encoderLeft, NXP_Encoder& encoderRight) : startRPM(startRPMValue), encoderLeft(encoderLeft), encoderRight(encoderRight) {}

void Differential::proc(float position, uint32_t currentMillis) {
    if (obstacleFinalBrake) {
        leftMotorPower  = 0.0f;
        rightMotorPower = 0.0f;
        return;
    }

    if (emergencyBrake) {
        leftMotorPower  = 0.0f;
        rightMotorPower = 0.0f;

        if (currentMillis - emergencyBrakeTimer > 300) return;

        if (encoderRight.getRPM() < 2000 && encoderLeft.getRPM() < 2000) emergencyBrake = false;
    }

    if (patternDetected) {
        startRPM     = 900;
        cornerRPM    = 900;
        brakeDivider = 400.0f;

        if (klzDistance < 200) {
            leftMotorPower     = 0.0f;
            rightMotorPower    = 0.0f;
            obstacleFinalBrake = true;
            return;
        } else if (klzDistance < 400) {
            leftMotorPower     = -0.1f;
            rightMotorPower    = -0.1f;
            return;
        }
        return;
    }

    // float breakComponent = (abs(position) / breakRatio);
    float diffComponent = (1 - (abs(position) / differentialValue));

    brakeComponent = 1 - (abs(position) / brakeDivider);
    brakeComponent = std::clamp(brakeComponent, -1.0f, 1.0f);

    if (startRPM * brakeComponent < cornerRPM) brakeComponent = cornerRPM / (startRPM);

    if (position >= 0) {
        setLeftMotorRPM  = startRPM * brakeComponent;
        setRightMotorRPM = startRPM * diffComponent * brakeComponent;
    } else if (position < 0) {
        setRightMotorRPM = startRPM * brakeComponent;
        setLeftMotorRPM  = startRPM * diffComponent * brakeComponent;
    }

    float pidOutLeft  = pidLeft.calculate(setLeftMotorRPM / 3000.0f, encoderRight.getRPM() / 3000.0f);
    float pidOutRight = pidRight.calculate(setRightMotorRPM / 3000.0f, encoderLeft.getRPM() / 3000.0f);

    leftMotorPower  = pidOutLeft;
    rightMotorPower = pidOutRight;

    if (encoderRight.getRPM() > 4000 || encoderLeft.getRPM() > 4000) {
        leftMotorPower      = 0.0f;
        rightMotorPower     = 0.0f;
        emergencyBrakeTimer = currentMillis;
        emergencyBrake      = true;
    }
}

float Differential::getLeft() { return leftMotorPower; }
float Differential::getRight() { return rightMotorPower; }

void     Differential::setStartRPM(uint32_t value) { startRPM = value; }
uint32_t Differential::getStartRPM() { return startRPM; }
