/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 * NXP HALina implementation of servo driver
 *
 */

#include "NXP_servo.hpp"
#include "NXP_Kitty.hpp"

void NXP_Servo::init(){
    pwm.init();
    filter.init();
}

void NXP_Servo::set(float value){
    value = std::clamp(value, -1.0f, 1.0f);

    value *= (static_cast<float>(servoMaxValue - servoMinValue))/2.0f;
    value += static_cast<float>(servoCenterValue);
    auto servoValue = int32_t(value);
    filter.runningAverage(&servoValue);
//    pwm.setDutyCycle(servoValue);
}

float NXP_Servo::get(){
    // todo write conversion form int32_t to float
//    return static_cast<float>(pwm.getDutyCycle());
}