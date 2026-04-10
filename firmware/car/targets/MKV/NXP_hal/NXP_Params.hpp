#include "algorithms/pid.hpp"

#pragma once

class NXP_Params {
  private:
    // Divides position in final servo set in NXP_Kitty
    float servoDivider = 20.0f;

    // EMA filter alpha for algorithm position from camera data
    float algorithmFilterAlpha = 0.7f;

    // Threshold for correlation-based pattern detection in algorithm
    float patternCorrelationThreshold = 0.88f;

    // Target RPM value for motors
    uint32_t startRPM = 4000;

    // Braking parameters for motor algorithm - in corners
    // Lower - more brake!
    float brakeAll = 22.0f;
    float brakeOne = 35.0f;

    // Braking clamp for motor algorithm - in corners
    float brakeClamp = 0.6f;

    // RPMs in corners
    uint32_t cornerOutsideRPM = 600;
    uint32_t cornerInsideRPM = 600;

    // PID params for motors
    float pidKp = 1.9f;
    float pidKi = 0.0002f;

  public:
    NXP_Params() {}
    float getServoDivider() const { return servoDivider; }
    void  setServoDivider(float divider) { servoDivider = divider; }

    float getAlgorithmFilterAlpha() const { return algorithmFilterAlpha; }
    void  setAlgorithmFilterAlpha(float alpha) { algorithmFilterAlpha = alpha; }

    float getPatternCorrelationThreshold() const { return patternCorrelationThreshold; }
    void  setPatternCorrelationThreshold(float threshold) { patternCorrelationThreshold = threshold; }

    uint32_t getStartRPM() const { return startRPM; }
    void     setStartRPM(uint32_t rpm) { startRPM = rpm; }

    float getBrakeAll() const { return brakeAll; }
    void  setBrakeAll(float value) { brakeAll = value; }

    float getBrakeOne() const { return brakeOne; }
    void  setBrakeOne(float value) { brakeOne = value; }

    float getBrakeClamp() const { return brakeClamp; }
    void  setBrakeClamp(float value) { brakeClamp = value; }

    uint32_t getCornerOutsideRPM() const { return cornerOutsideRPM; }
    void     setCornerOutsideRPM(uint32_t rpm) { cornerOutsideRPM = rpm; }

    uint32_t getCornerInsideRPM() const { return cornerInsideRPM; }
    void     setCornerInsideRPM(uint32_t rpm) { cornerInsideRPM = rpm;  }

    float getPidKp() const { return pidKp; }
    void  setPidKp(float kp, PID& pidLeft, PID& pidRight) { 
        pidLeft.setKp(kp);
        pidRight.setKp(kp);
        pidKp = kp;
    }

    float getPidKi() const { return pidKi; }
    void  setPidKi(float ki, PID& pidLeft, PID& pidRight) { 
        pidLeft.setKi(ki);
        pidRight.setKi(ki);
        pidKi = ki;
    }
};