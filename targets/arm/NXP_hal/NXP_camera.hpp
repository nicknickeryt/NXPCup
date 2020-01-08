#pragma once
#include "NXP_gpio.hpp"
#include "NXP_PIT.hpp"

class NXP_Camera {
public:
    NXP_Camera(uint32_t clockFrequencyInHz, NXP_GPIO& clockPin, NXP_GPIO& SIPin) : clockFrequencyInHz(clockFrequencyInHz), clockPin(clockPin), SIPin(SIPin){}

    void init();

    void dummy();

public:
    uint32_t clockFrequencyInHz;

private:
    NXP_GPIO& clockPin;
    NXP_GPIO& SIPin;


};

