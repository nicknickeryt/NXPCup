/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Dominik Michalczyk
 *
 * PID
 *
 */


#include "pid.hpp"

#include <iostream>

PID::PID(float kp, float ki, float kd, float maxValue) {
    Kp             = kp;
    Ki             = ki;
    Kd             = kd;
    this->maxValue = maxValue;
}

float PID::calculate(float setpoint, float current) {
    float error = setpoint - current;

    integral += Ki * (error + previousError) / 2.0f;
    float maxIntegral = maxValue;
    if (integral > maxIntegral) integral = maxIntegral;
    if (integral < -maxIntegral) integral = -maxIntegral;

    float derivative = (current - previousTarget) * Kd;

    previousError      = error;
    previousTarget     = current;

    float output = Kp * error + integral + derivative;
    if (output > maxValue) output = maxValue;
    if (output < -maxValue) output = -maxValue;

    return output;
}

void PID::setKp(float kp) { Kp = kp; }

float PID::getKp() const { return Kp; }