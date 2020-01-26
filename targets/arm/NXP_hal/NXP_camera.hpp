#pragma once
#include "NXP_gpio.hpp"
#include "NXP_PIT.hpp"
#include "NXP_adc.hpp"

class NXP_Camera {
public:
    uint16_t buffer1Data [128] = {0};
    uint16_t buffer2Data [128] = {0};

    int16_t currentPixelIndex = 0; // must be int

    enum class Type : uint8_t {
        CAMERA_1,
        CAMERA_2,
        BOTH
    };

    enum class CameraState : uint8_t {
        STOPPED = 0,
        START,
        SET_SI_PIN,
        RESET_SI_PIN,

        SET_CLOCK_PIN,
        RESET_CLOCK_PIN,

        GET_DATA_FROM_ADC,
    };

    NXP_Camera(Type type, NXP_ADC& adc, NXP_GPIO& clockPin, NXP_GPIO& SIPin,  NXP_ADC::Sample& sampleCamera1, NXP_ADC::Sample& sampleCamera2);

    CameraState cameraState = CameraState::STOPPED;

    void init();

    static void adcInterruptEndOfMeasurementStatic(uint8_t);

    static void pitInterruptStatic(uint8_t);

    void adcInterruptEndOfMeasurement();

    void pitInterrupt();
public:
    Type type;
    NXP_ADC& adc;

    NXP_GPIO& clockPin;
    NXP_GPIO& SIPin;

    NXP_ADC::Sample& sampleCamera1;
    NXP_ADC::Sample& sampleCamera2;
};

