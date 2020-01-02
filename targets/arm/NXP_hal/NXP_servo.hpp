//
// Created by Kamil on 02.01.2020.
//

#ifndef PROJECT_NAME_NXP_SERVO_HPP
#define PROJECT_NAME_NXP_SERVO_HPP

#include "NXP_hal.hpp"

class NXP_Servo : public halina::Servo{
public:
    NXP_Servo();

    void set(float position) override;

    float get() override;
};

#endif //PROJECT_NAME_NXP_SERVO_HPP
