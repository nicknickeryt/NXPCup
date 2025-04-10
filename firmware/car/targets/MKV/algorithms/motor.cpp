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

void Differential::proc(float position) {
    // float breakComponent = (abs(position) / breakRatio);
    float diffComponent = (1 - (abs(position) / differentialValue));

    brakeComponent = 1 - (abs(position) / 105.0f);
    brakeComponent = std::clamp(brakeComponent, 0.0f, 1.0f);

    if (startRPM * brakeComponent < cornerRPM) brakeComponent = cornerRPM / (startRPM);

    if (position >= 0) {
        setLeftMotorRPM  = startRPM * brakeComponent;
        setRightMotorRPM = startRPM * diffComponent * brakeComponent;
    } else if (position < 0) {
        setRightMotorRPM = startRPM * brakeComponent;
        setLeftMotorRPM  = startRPM * diffComponent * brakeComponent;
    }

    float pidOutLeft  = (float)pidLeft.calculate(setLeftMotorRPM, encoderRight.getRPM()) / 100.0f;
    float pidOutRight = (float)pidRight.calculate(setRightMotorRPM, encoderLeft.getRPM()) / 100.0f;

    leftMotorPower  = pidOutLeft;
    rightMotorPower = pidOutRight;
}

float Differential::getLeft() { return leftMotorPower; }
float Differential::getRight() { return rightMotorPower; }

void     Differential::setStartRPM(uint32_t value) { startRPM = value; }
uint32_t Differential::getStartRPM() { return startRPM; }
