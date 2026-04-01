#define LOG_CHANNEL CAMERA

#include "NXP_camera.hpp"
#include "logger.h"
#include "NXP_Kitty.hpp"

NXP_Camera* cameraHandler = nullptr;

NXP_Camera::NXP_Camera(Type type, NXP_ADC& adc, NXP_GPIO& clockPin, NXP_GPIO& SIPin,  NXP_ADC::Sample& sampleCamera1, NXP_ADC::Sample& sampleCamera2, NXP_Uart& debug) :
    type(type), adc(adc), clockPin(clockPin), SIPin(SIPin), sampleCamera1(sampleCamera1), sampleCamera2(sampleCamera2), debug(debug) {
    cameraHandler = this;
}

void NXP_Camera::init() {
    clockPin.init();
    SIPin.init();

    for (uint8_t i = 0; i < 8; i ++) {
        adc.appendSample(&sampleCamera1);
    }

    adc.init();
    clockPin.reset();
    SIPin.reset();
};

void NXP_Camera::proc(bool& trigger){
    if(trigger){
        trigger = false;
        __disable_irq();
        camera1DataBuffer[0] = 0xff;
        camera1DataBuffer[1] = 0xff;
        camera1DataBuffer[2] = 0xff;
        camera1DataBuffer[3] = 0xff;
        memcpy(&camera1DataBuffer[4], buffer1Data, 256);
        __enable_irq();
        debug.write(camera1DataBuffer, 260);
    }
}

void NXP_Camera::adcInterruptEndOfMeasurementStatic(uint8_t) {
    cameraHandler->adcInterruptEndOfMeasurement();
}

void NXP_Camera::pitInterruptStatic(uint32_t*) {
    cameraHandler->pitInterrupt();
}

void NXP_Camera::adcInterruptEndOfMeasurement() {
        uint16_t* data = adc.getBufferValues(sampleCamera1.converterType);
        uint32_t result = 0;
        for (uint8_t i = 0; i < 8; i++) {
            result += data[i];
        }
        buffer1Data[currentPixelIndex] = result / 8;
}

void NXP_Camera::pitInterrupt() {
    static uint8_t waitEnd = 0;

    switch(cameraState) {
        case CameraState::START:
            cameraState = CameraState::SET_SI_PIN;
        break;
        case CameraState::SET_SI_PIN:
            SIPin.set();
            cameraState = CameraState::SET_FIRST_CLOCK_PIN;
        break;
        case CameraState::SET_FIRST_CLOCK_PIN:
            clockPin.set();
            cameraState = CameraState::RESET_SI_PIN;
        break;
        case CameraState::RESET_SI_PIN:
            SIPin.reset();
            cameraState = CameraState::RESET_FIRST_CLOCK_PIN;
        break;
        case CameraState::RESET_FIRST_CLOCK_PIN:
            clockPin.reset();
            currentPixelIndex = -1;
            waitEnd = 0;
            cameraState = CameraState::WAIT_1;
        break;
        case CameraState::WAIT_1:
            cameraState = CameraState::WAIT_2;
        break;
        case CameraState::WAIT_2:
            cameraState = CameraState::SET_CLOCK_PIN;
        break;
        case CameraState::SET_CLOCK_PIN:
            currentPixelIndex++;

            if (currentPixelIndex == 128) {
                cameraDataReceived = true;
                cameraState = CameraState::STOPPED;
                cameraState = CameraState::WAIT_END_1;
            } else {
                cameraState = CameraState::RESET_CLOCK_PIN;
                clockPin.set();    
            }
        break;
        case CameraState::RESET_CLOCK_PIN:
            adc.startConversion();
            clockPin.reset();
            cameraState = CameraState::SET_CLOCK_PIN;
        break;
        case CameraState::WAIT_END_1:
            waitEnd++;
            if (waitEnd > 1) {
                waitEnd = 0;
                cameraState = CameraState::START;
            }
        break;

        case CameraState::STOPPED:
        break;

        default: // CAMERA_WAIT_END_2 etc
        break;
    }
}

void NXP_Camera::getData(uint16_t*& dataBuffer){
    __disable_irq();
    // Avoid copying the same thing twice, just point to the same buffer
    dataBuffer = buffer1Data;
    __enable_irq();
}