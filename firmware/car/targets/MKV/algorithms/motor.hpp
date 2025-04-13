#pragma once
#include "NXP_encoder.hpp"
#include "pid.hpp"

#include <utility>

class Differential {
    int   position;
    float leftMotorPower;
    float rightMotorPower;

    uint32_t startRPM = 0;

    uint32_t setLeftMotorRPM;
    uint32_t setRightMotorRPM;

    NXP_Encoder& encoderLeft;
    NXP_Encoder& encoderRight;

    uint16_t klzDistance = 0;

    float differentialValue = 20.0f;

    float brakeComponent = 0;

    float brakeDivider = 140.0f;

    uint32_t cornerRPM = 2700;

                    //Kp     Ki     Kd     maxValue
    PID pidLeft = PID(0.07f, 0.00f, 0.0f, 100.0f);  
    PID pidRight = PID(0.07f, 0.00f, 0.0f, 100.0f);

  public:
    Differential(float startVelocity, NXP_Encoder& encoderLeft, NXP_Encoder& encoderRight);
    void  proc(float position);

    float getLeft();
    float getRight();

    void  setStartRPM(uint32_t value);
    uint32_t getStartRPM();

    uint8_t getDiffValue() { return differentialValue; }
    void setDiffValue(uint8_t newValue) { differentialValue = newValue; }

    uint8_t getBrakeDivider() { return brakeDivider; }
    void setBrakeDivider(uint8_t newValue) { brakeDivider = newValue; }

    void setKlzDistance(uint16_t distance) { klzDistance = distance; }
    uint8_t getKlzDistance() { return klzDistance; }
};
