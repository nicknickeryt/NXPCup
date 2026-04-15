#pragma once
#include "NXP_encoder.hpp"
#include "NXP_Params.hpp"
#include "pid.hpp"

#include <utility>

class Differential {
    int   position;
    float leftMotorPower;
    float rightMotorPower;

    uint32_t setLeftMotorRPM;
    uint32_t setRightMotorRPM;

    NXP_Encoder& encoderLeft;
    NXP_Encoder& encoderRight;

    NXP_Params& params;

    uint16_t klzDistance = 0;

    float brakeComponent = 0;

    uint32_t patternDetectedMillis = 0;
    uint32_t patternRpm =  0;
    bool slowedDown = false;
    //bool     patternSetRPM         = false;

    // Kp     Ki     Kd     maxValue
    PID pidLeft  = PID(params.getPidKp(), params.getPidKi(), params.getPidKd(), 1.0f);
    PID pidRight = PID(params.getPidKp(), params.getPidKi(), params.getPidKd(), 1.0f);

    bool patternDetected = false;

    bool     emergencyBrake      = false;
    uint32_t emergencyBrakeTimer = 0;

    bool obstacleFinalBrake = false;


    PID distancePID = PID(
        0.015f,   // Kp — szybciej reaguje
        0.0002f,  // Ki — lekka korekta
        0.012f    // Kd — tłumienie
    );

    uint32_t maxPowerTimer  = 0;
    bool     maxPowerActive = false;

    uint32_t stableTimer  = 0;
    bool     stableActive = false;

    bool finalStopDone      = false;
    bool distanceModeActive = false;

  public:
    Differential(NXP_Encoder& encoderLeft, NXP_Encoder& encoderRight, NXP_Params& params);
    void proc(float position, uint32_t currentMillis, uint16_t sr04Distance);

    float getLeft();
    float getRight();

    PID& getLeftPID() { return pidLeft; }
    PID& getRightPID() { return pidRight; }

    void     setKlzDistance(uint16_t distance) { klzDistance = distance; }
    uint16_t getKlzDistance() { return klzDistance; }

    void setPatternDetected(bool detected) { patternDetected = detected; }

    void clearAllFlags() {
        patternDetected    = false;
        obstacleFinalBrake = false;
        emergencyBrake     = false;
    }

    bool isDistanceModeActive() const { return distanceModeActive; }

    bool isFinalStopDone() const { return finalStopDone; }
};
