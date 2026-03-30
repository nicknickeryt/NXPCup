#include <stdbool.h>
#include <stdint.h>

#pragma once

class PID {
  private:
    float Kp;
    float Ki;
    float Kd;

    float integral           = 0;
    float previousError      = 0;
    float previousTarget     = 0;
    float previousDerivative = 0;

    float maxValue;

  public:
    PID(float kp = -1.8f, float ki = -0.0001f, float kd = 0.0f, float maxValue = 1.0f);
    float calculate(float setpoint, float current);

    void  setKp(float kp);
    float getKp() const;
    void  reset() {
        integral      = 0;
        previousError = 0;
    }
};