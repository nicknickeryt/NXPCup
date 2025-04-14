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

    float differentialValue = 42.0f;

    float brakeComponent = 0;

    float brakeDivider = 180.0f;

    uint32_t cornerRPM = 2300;

    float pidKp = 0.00f;
    float pidKi = 0.001f;
    float pidKd = 0.0f;

                    //Kp     Ki     Kd     maxValue
    PID pidLeft = PID(pidKp, pidKi, pidKd, 1.0f);  
    PID pidRight = PID(pidKp, pidKi, pidKd, 1.0f);

    bool patternDetected = false;

    bool emergencyBrake = false;
    uint32_t emergencyBrakeTimer = 0;

  public:
    Differential(float startRPMValue, NXP_Encoder& encoderLeft, NXP_Encoder& encoderRight);
    void  proc(float position, uint32_t currentMillis);

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

    void setPidKp(float kp) {
        pidKp = kp;
        pidLeft.setKp(pidKp);
        pidRight.setKp(pidKp);
    }

    float getPidKp() {
        return pidKp;
    }

    void setPatternDetected(bool detected) {
        patternDetected = detected;
    }
};
