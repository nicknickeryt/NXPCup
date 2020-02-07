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
    pwm.init();
}

void NXP_Motor::setValue(float value) {
    value = std::clamp(value, -1.0f, 1.0f);
    value *= (static_cast<float>(maxValue - minValue))/2.0f;
    value += static_cast<float>(centerValue);
    auto motorValue = int32_t(value);
    filter.runningAverage(&motorValue);
//    pwm.setDutyCycle(motorValue);
}


float NXP_Motor::getValue() {
    // todo write conversion form int32_t to float
//    return static_cast<float>(pwm.getDutyCycle());
}
