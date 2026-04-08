    #include "motor.hpp"

    #include "pid.hpp"

    #include <utility>

    #include <cmath>

    /*
    *@brief set velocity adequate to current conditions on track
    *@param startVelocity is the default speed of the car
    *@param position is the current position of the car on the track
    */

    Differential::Differential(float startRPMValue, NXP_Encoder& encoderLeft, NXP_Encoder& encoderRight) : startRPM(startRPMValue), encoderLeft(encoderLeft), encoderRight(encoderRight) {}

    void Differential::proc(float position, uint32_t currentMillis, uint16_t sr04Distance) {
        // patternDetected = 0;
        
        if (patternDetected) {
            setStartRPM(800);
        }

        if (sr04Distance < 250 && patternDetected) {
            distanceModeActive = true;
        }

        if(finalStopDone) {
            leftMotorPower  = 0.0f;
            rightMotorPower = 0.0f;
            return;
        }

        if (patternDetected && distanceModeActive && !finalStopDone) {
            float target  = 90.0f;
            float current = sr04Distance;

            bool inRange = fabs(current - target) < 15.0f;

            // 🔥 jeśli jesteśmy blisko → NIE używamy PID
            if (inRange) {
                distancePID.reset(); // 🔥 bardzo ważne

                leftMotorPower  = 0.0f;
                rightMotorPower = 0.0f;

                if (!stableActive) {
                    stableActive = true;
                    stableTimer  = currentMillis;
                } else if (currentMillis - stableTimer > 3000) {
                    finalStopDone = true;
                    leftMotorPower  = 0.0f;
                    rightMotorPower = 0.0f;
                    return;
                }

            } else {
                stableActive = false;

                float output = distancePID.calculate(current, target);
                output       = std::clamp(output, -0.7f, 0.18f);

                leftMotorPower  = output;
                rightMotorPower = output;

                // 🔥 zabezpieczenie max mocy
                if (fabs(output) >= 0.5f) {
                    if (!maxPowerActive) {
                        maxPowerActive = true;
                        maxPowerTimer  = currentMillis;
                    } else if (currentMillis - maxPowerTimer > 1000) {
                        leftMotorPower  = 0.0f;
                        rightMotorPower = 0.0f;
                        return;
                    }
                } else {
                    maxPowerActive = false;
                }
            }

            return;
        }

        // float breakComponent = (abs(position) / breakRatio);
        float diffComponent = (1 - (abs(position) / brakeOne));
        diffComponent = std::clamp(diffComponent, 0.6f, 1.0f);

        brakeComponent = 1 - (abs(position) / brakeAll);
        brakeComponent = std::clamp(brakeComponent, 0.6f, 1.0f);


        // if (startRPM * brakeComponent < cornerRPM) brakeComponent = cornerRPM / (startRPM);

        if (position >= 0) {
            setLeftMotorRPM  = startRPM * brakeComponent;
            if(setLeftMotorRPM < cornerOutsideRPM) setLeftMotorRPM  = cornerOutsideRPM;

            setRightMotorRPM = startRPM * diffComponent * brakeComponent;
            if(setRightMotorRPM < cornerInsideRPM) setRightMotorRPM = cornerInsideRPM;
        } else if (position < 0) {
            setRightMotorRPM = startRPM * brakeComponent;
            if(setRightMotorRPM < cornerOutsideRPM) setRightMotorRPM = cornerOutsideRPM;

            setLeftMotorRPM  = startRPM * diffComponent * brakeComponent;
            if(setLeftMotorRPM < cornerInsideRPM) setLeftMotorRPM = cornerInsideRPM;
        }

        float pidOutLeft  = pidLeft.calculate((float) setLeftMotorRPM / (float)startRPM, encoderRight.getRPM() / (float)startRPM);
        float pidOutRight = pidRight.calculate((float)setRightMotorRPM / (float) startRPM, encoderLeft.getRPM() / (float)startRPM);

        leftMotorPower  = pidOutLeft;
        rightMotorPower = pidOutRight;

        if (encoderRight.getRPM() > 5000 || encoderLeft.getRPM() > 5000) {
            leftMotorPower      = 0.0f;
            rightMotorPower     = 0.0f;
            emergencyBrakeTimer = currentMillis;
            emergencyBrake      = true;
        }
    }

    float Differential::getLeft() { return leftMotorPower; }
    float Differential::getRight() { return rightMotorPower; }

    void     Differential::setStartRPM(uint32_t value) { startRPM = value; }
    uint32_t Differential::getStartRPM() { return startRPM; }
