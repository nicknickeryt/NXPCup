#pragma once
#include "NXP_gpio.hpp"
#include "NXP_PIT.hpp"
#include "NXP_adc.hpp"
#include "NXP_uart.hpp"

class NXP_Camera {
public:
    enum class Type : uint8_t {
        CAMERA_1,
        CAMERA_2,
        BOTH
    };

    enum class CameraState : uint8_t {
        STOPPED = 0,
        START,
        SET_SI_PIN,
        SET_FIRST_CLOCK_PIN,
        RESET_SI_PIN,
        RESET_FIRST_CLOCK_PIN,
        WAIT_1,
        WAIT_2,

        SET_CLOCK_PIN,
        RESET_CLOCK_PIN,

        WAIT_END_1,
        WAIT_END_2,
        WAIT_END_3,
        WAIT_END_4,
        WAIT_END_5,
        WAIT_END_6,
        WAIT_END_7,
        GET_DATA_FROM_ADC,
    };

private:
    uint16_t buffer1Data [128] = {0};
    uint16_t buffer2Data [128] = {0};
    int16_t currentPixelIndex = 0;
    uint8_t camera1DataBuffer [258];
    uint8_t camera2DataBuffer [258];
    CameraState cameraState = CameraState::STOPPED;
    Type type;
    NXP_ADC& adc;
    NXP_GPIO& clockPin;
    NXP_GPIO& SIPin;
    NXP_ADC::Sample& sampleCamera1;
    NXP_ADC::Sample& sampleCamera2;
    NXP_Uart& debug;

public:
    NXP_Camera(Type type, NXP_ADC& adc, NXP_GPIO& clockPin, NXP_GPIO& SIPin,  NXP_ADC::Sample& sampleCamera1, NXP_ADC::Sample& sampleCamera2, NXP_Uart& debug);

    void proc(bool& trigger);
    void init();
    static void adcInterruptEndOfMeasurementStatic(uint8_t);
    static void pitInterruptStatic(uint32_t*);
    void adcInterruptEndOfMeasurement();
    void pitInterrupt();
    void start() {
        cameraState = CameraState::START;
    }
    bool getData(Type camera, uint16_t* dataBuffer);


};

