/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 * NXP HALina implementation of motor driver
 *
*/


#include "NXP_motor.hpp"
#include "NXP_Kitty.hpp"

void NXP_Motor::init(){
    enablePin.init();
    block();
    pwmForward.init();
    pwmBackward.init();
}

void NXP_Motor::setValue(float value) {

    value = std::clamp(value, 0.0f, 0.5f);;

    if (value <= 0.0f) {
        motorDirection = MotorDirection::FORWARDS;
        pwmForward.setDutyCycle(-value);
        pwmBackward.setDutyCycle(0.0f);
    } else {
        motorDirection = MotorDirection::BACKWARDS;
        pwmForward.setDutyCycle(0.0f);
        pwmBackward.setDutyCycle(value);
    }
}


float NXP_Motor::getValue() {
    switch(motorDirection){
        case MotorDirection::FORWARDS:
            return pwmForward.getDutyCycle();
        case MotorDirection::BACKWARDS:
            return pwmBackward.getDutyCycle();
        default:
            break;
    }
    return 0.0;
}
