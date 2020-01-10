#pragma once
#include "NXP_gpio.hpp"
#include "NXP_PIT.hpp"
#include "NXP_adc.hpp"

void camera0Callback();
void camera1Callback();

class NXP_Camera {
public:
    enum CameraIndex {
        CAMERA_0,
        CAMERA_1
    };

    NXP_Camera(CameraIndex index, uint32_t clockFrequencyInHz, uint32_t siFrequencyInHz, NXP_ADC& adc, NXP_GPIO& clockPin, NXP_GPIO& SIPin);

    void init();

    static void dummy(uint8_t x);

public:
    CameraIndex index;
    uint32_t clockFrequencyInHz;
    uint32_t siFrequencyInHz;
    NXP_ADC& adc;
    uint16_t clockPeriodsToSIGeneration;
    uint16_t clockPeriodsCounter;
    NXP_GPIO& clockPin;
    NXP_GPIO& SIPin;

private:



};

