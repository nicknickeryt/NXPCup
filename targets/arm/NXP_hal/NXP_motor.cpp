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

void NXP_Motor::setValue(int32_t value) {
//    static int32_t mean[NUMPR];
//    for (uint8_t i =0; i < (NUMPR-1); i ++)
//    {
//        mean[i] = mean[i+1];
//    }
//    mean[NUMPR-1] = value;
//
//    value = 0;
//    for (uint8_t i =0; i < NUMPR; i ++)
//    {
//        value += mean[i];
//    }
//    value /= NUMPR;

    pwm.setDutyCycle(value);
}


int32_t NXP_Motor::getValue() {
    return pwm.getDutyCycle();
}
