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

    if (value < 0.0f) {
        pwm.setDutyCycle(-value, pwm.channelFirst);
        pwm.setDutyCycle(0.0f, pwm.channelSecond);
    } else {
        pwm.setDutyCycle(value, pwm.channelSecond);
        pwm.setDutyCycle(0.0f, pwm.channelFirst);
    }
}


float NXP_Motor::getValue() {
    // todo write conversion form int32_t to float
    return 0.0f;
//    return static_cast<float>(pwm.getDutyCycle());
}
