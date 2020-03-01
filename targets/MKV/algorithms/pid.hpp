#pragma once
#include <stdint.h>

class PID{
private:
    float kp;
    float ki;
    float kd;
    float maxSummaryError;
    float a;
    float b;
    float previousLeftMotorOutput = 0;
    float previousRightMotorOutput = 0;
    float summaryErrorLeftMotor = 0;
    float summaryErrorRightMotor = 0;
    float previousErrorLeftMotor = 0;
    float previousErrorRightMotor = 0;

public:
    PID(float kp, float ki, float kd, float maxSummaryError, float a, float b) : kp(kp), ki(ki), kd(kd), maxSummaryError(maxSummaryError), a(a), b(b){}

    void calculate(float* leftMotorValue, float* rightMotorValue, uint16_t leftEncoderValue, uint16_t rightEncoderValue){
        float outputLeftMotorValue;
        float outputRightMotorValue;

        const float leftEncoder = convertEncoderData(leftEncoderValue);
        const float rightEncoder = convertEncoderData(rightEncoderValue);

        // get current error
        const float errorLeftMotor = *leftMotorValue - leftEncoder;
        const float errorRightMotor = *rightMotorValue - rightEncoder;

        // update summary errors
        summaryErrorLeftMotor += errorLeftMotor;
        summaryErrorRightMotor += errorRightMotor;
        if(summaryErrorLeftMotor >= maxSummaryError){
            summaryErrorLeftMotor = maxSummaryError;
        }
        if(summaryErrorRightMotor >= maxSummaryError){
            summaryErrorRightMotor = maxSummaryError;
        }

        // calculate output signals
        outputLeftMotorValue = kp*errorLeftMotor +  summaryErrorLeftMotor*ki + (errorLeftMotor-previousErrorLeftMotor)*kd;
        outputLeftMotorValue += previousLeftMotorOutput;
        if(outputLeftMotorValue > 1){
            outputLeftMotorValue = 1;
        } else if(outputLeftMotorValue < 0){
            outputLeftMotorValue = 0;
        }
        outputRightMotorValue = kp*errorRightMotor +  summaryErrorRightMotor*ki + (errorRightMotor-previousErrorRightMotor)*kd;
        outputRightMotorValue += previousRightMotorOutput;
        if(outputRightMotorValue > 1){
            outputRightMotorValue = 1;
        } else if(outputRightMotorValue < 0){
            outputRightMotorValue = 0;
        }

        // remember signals 
        previousLeftMotorOutput = outputLeftMotorValue;
        previousRightMotorOutput = outputRightMotorValue;

        // remember errors
        previousErrorLeftMotor = errorLeftMotor;
        previousErrorRightMotor = errorRightMotor;

        *leftMotorValue = outputLeftMotorValue;
        *rightMotorValue = outputRightMotorValue;
    }

private:
    // encoderData = f(motorData) = a*motorData + b
    float convertEncoderData(uint16_t encoderValue){
        return (((float)encoderValue) - b)/a;
    }
};