/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Dominik Michalczyk
 *
 * PID 
 *
 */


#include "pid.hpp"

#include <iostream>

PID::PID(float kp, float ki, float kd, float maxValue, float bias) {
    Kp = kp;
    Ki = ki;
    Kd = kd;
    this->maxValue = maxValue;
    this->bias = bias;
}

float PID::calculate(int32_t setpoint, int32_t current) {    
    float error = setpoint - current;

    integral += Ki * (error + previousError);
    float maxIntegral = maxValue * 0.12f;
    if (integral > maxIntegral) integral = maxIntegral;
    if (integral < -maxIntegral) integral = -maxIntegral;

    float derivative = (current - previousTarget) * previousDerivative * Kd;

    previousError = error;
    previousTarget = current;
    previousDerivative = derivative;

    float output = Kp * error + integral + derivative + bias;
    if (output > maxValue) output = maxValue;
    if (output < -maxValue) output = -maxValue;

    return output;
}
