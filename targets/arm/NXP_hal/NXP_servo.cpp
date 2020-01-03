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

    SIM->SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK;
    SIM->SOPT2 |= SIM_SOPT2_TIMESRC(1);
    SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;
    SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
    SIM->SCGC6 |= SIM_SCGC6_FTM0_MASK;

    pwm.init();
    filter.init();
}

void NXP_Servo::set(float value){
    if(value > 1.0){
        value = 1.0;
    } else if(value < -1.0){
        value = -1.0;
    }
    value *= (static_cast<float>(servoMaxValue - servoMinValue))/2.0;
    value += static_cast<float>(servoCenterValue);
    auto servoValue = int32_t(value);
    filter.runningAverage(&servoValue);
    pwm.setDutyCycle(servoValue);
}

float NXP_Servo::get(){
    return 0;
}