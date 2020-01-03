/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 * NXP HALina implementation of motor driver
 *
*/

#include "NXP_motor.hpp"
#include "NXP_Kitty.hpp"

void NXP_Motor::NXP_MotorLeft::init() {
    SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
    SIM->SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK;
    SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
    SIM->SCGC6 |= SIM_SCGC6_FTM3_MASK;
    pwm.init();
}

void NXP_Motor::NXP_MotorRight::init(){
    // clocks was started in Right motor
    pwm.init();
}

void NXP_Motor::NXP_MotorRight::setValue(int32_t value) {
    static int32_t mean[NUMPR];
    for (uint8_t i =0; i < (NUMPR-1); i ++)
    {
        mean[i] = mean[i+1];
    }
    mean[NUMPR-1] = value;

    value = 0;
    for (uint8_t i =0; i < NUMPR; i ++)
    {
        value += mean[i];
    }
    value /= NUMPR;

    pwm.setDutyCycle(value);
}

void NXP_Motor::NXP_MotorLeft::setValue(int32_t value) {
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

    motor.kitty.display.print(value);

    pwm.setDutyCycle(value);
}

int32_t NXP_Motor::NXP_MotorRight::getValue() {
    return pwm.getDutyCycle();
}

int32_t NXP_Motor::NXP_MotorLeft::getValue() {
    return pwm.getDutyCycle();
}