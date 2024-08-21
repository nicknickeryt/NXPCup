#pragma once
#include "MKV_gpio.hpp"
#include "MKV_PIT.hpp"
#include "MKV_adc.hpp"
#include "MKV_uart.hpp"

class MKV_Camera {
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
    uint8_t camera1DataBuffer [260];
    uint8_t camera2DataBuffer [260];
    CameraState cameraState = CameraState::STOPPED;
    Type type;
    MKV_ADC& adc;
    MKV_GPIO& clockPin;
    MKV_GPIO& SIPin;
    MKV_ADC::Sample& sampleCamera1;
    MKV_ADC::Sample& sampleCamera2;
    MKV_Uart& debug;

public:
    MKV_Camera(Type type, MKV_ADC& adc, MKV_GPIO& clockPin, MKV_GPIO& SIPin,  MKV_ADC::Sample& sampleCamera1, MKV_ADC::Sample& sampleCamera2, MKV_Uart& debug);

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
