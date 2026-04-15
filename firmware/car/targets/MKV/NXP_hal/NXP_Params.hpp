#include "NXP_PIT.hpp"
#include "algorithms/pid.hpp"

#pragma once

class NXP_Params {
  private:
    // Divides position in final servo set in NXP_Kitty
    float servoDivider = 19.0f;

    // EMA filter alpha for algorithm position from camera data
    float algorithmFilterAlpha = 0.7f;

    // Threshold for correlation-based pattern detection in algorithm
    float patternCorrelationThreshold = 0.58f;

    // Target RPM value for motors
    uint32_t startRPM = 3800;

    // Braking parameters for motor algorithm - in corners
    // Lower - more brake!
    float brakeAll = 19.0f;
    float brakeOne = 19.5f;

    // Braking clamp for motor algorithm - in corners
    float brakeClamp = 0.55f;
    float diffClamp = 0.0f;

    // RPMs in corners
    uint32_t rpmOffset = 750;

    // PID params for motors
    float pidKp = 2.0f;
    float pidKi = 0.00659f;
    float pidKd = 0.01f;

    float crossingsBrightnessMultiplier = 1.0f;
    float brightnessMeanAlpha = 0.3f;
    uint8_t crossingsCut = 0;

    float cameraCrossingFilterAlpha = 0.5f;

    uint32_t cameraPitFrequency = 50000;

    float algorithmOffset = 1.7f;

		uint32_t patternDetectTimeoutMs = 1500;

    bool isPatternStopEnabled = true; 

    bool uartEnabled = true;

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

    float getDiffClamp() const { return diffClamp; }
    void  setDiffClamp(float value) { diffClamp = value; }

    uint32_t getRPMOffset() const { return rpmOffset; }
    void     setRPMOffset(uint32_t rpm) { rpmOffset = rpm;  }

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

    float getCrossingsBrightnessMultiplier() const { return crossingsBrightnessMultiplier; }
    void  setCrossingsBrightnessMultiplier(float multiplier) { crossingsBrightnessMultiplier = multiplier; }

    float getBrightnessMeanAlpha() const { return brightnessMeanAlpha; }
    void  setBrightnessMeanAlpha(float alpha) { brightnessMeanAlpha = alpha; }

    uint8_t getCrossingsCut() const { return crossingsCut; }
    void  setCrossingsCut(uint8_t cut) { crossingsCut = cut; }

    float getCameraCrossingFilterAlpha() const { return cameraCrossingFilterAlpha; }
    void  setCameraCrossingFilterAlpha(float alpha) { cameraCrossingFilterAlpha = alpha; }

    uint32_t getCameraPitFrequency() const { return cameraPitFrequency; }
    void     setCameraPitFrequency(NXP_PIT &pit, uint32_t frequency) { cameraPitFrequency = frequency; pit.setFrequency(frequency); }

    float getAlgorithmOffset() const { return algorithmOffset; }
    void  setAlgorithmOffset(float offset) { algorithmOffset = offset; }

    uint32_t getPatternDetectTimeoutMs() const { return patternDetectTimeoutMs; }
    void     setPatternDetectTimeoutMs(uint32_t timeout) { patternDetectTimeoutMs = timeout; }

    bool getIsPatternStopEnabled() const { return isPatternStopEnabled; }
    void setIsPatternStopEnabled(bool enabled) { isPatternStopEnabled = enabled; }

    float getPidKd() const { return pidKd; }
    void  setPidKd(float kd, PID& pidLeft, PID& pidRight) { 
        pidLeft.setKd(kd);
        pidRight.setKd(kd);
        pidKd = kd;
    }

    bool getUartEnabled() { return uartEnabled; }
    void setUartEnabled(bool enabled) { uartEnabled = enabled; }

};