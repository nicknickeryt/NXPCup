#include <stdint.h>
#include <stdbool.h>

#pragma once

class PID {
    private:
        float Kp;
        float Ki;
        float Kd;

        float integral = 0;
        float previousError = 0;
        float previousTarget = 0;
        float previousDerivative = 0;

        float maxValue;
        float bias;
    public:
        PID(float kp = -1.8f, float ki = -0.0001f, float kd = 0.0f,
            float maxValue = 100.0f, float bias = 0.0f);
        int32_t calculate(int32_t setpoint, int32_t current);
};