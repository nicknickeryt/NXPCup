/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 * MKV HALina implementation of servo driver
 *
 */

#include "MKV_servo.hpp"

void MKV_Servo::init(){
    pwm.init();

    uint32_t ticks = pwm.getTicksPerSecond() / 1000; // ticks per milliseconds
    uint32_t ticksDeviation = ticks / 2;
    centerTicks = ticks + ticksDeviation;
    pwm.setRawPeriod(centerTicks, pwm.getChannel());

    maxTicksDeviation = maxDegreeDeviation * ticksDeviation / 90;
}

void MKV_Servo::set(float value){
    value = std::clamp(value + offset, -1.0f, 1.0f);
    currentValue = value;
    value = (float)maxTicksDeviation * value * servoMultiplier;
    pwm.setRawPeriod(centerTicks + value, pwm.getChannel());
}

void MKV_Servo::setOffset(float value) {
    offset = value;
}
