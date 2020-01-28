#define LOG_CHANNEL CAMERA
#define CAMERA_LOG_CHANNEL 2
#define CAMERA_LOG_CHANNEL_LEVEL LOG_LEVEL_DEBUG

#include "NXP_camera.hpp"
#include "logger.h"
#include "NXP_Kitty.hpp"

NXP_Camera* cameraHandler = nullptr;

NXP_Camera::NXP_Camera(Type type, NXP_ADC& adc, NXP_GPIO& clockPin, NXP_GPIO& SIPin,  NXP_ADC::Sample& sampleCamera1, NXP_ADC::Sample& sampleCamera2) :
    type(type), adc(adc), clockPin(clockPin), SIPin(SIPin), sampleCamera1(sampleCamera1), sampleCamera2(sampleCamera2) {
    cameraHandler = this;
}

void NXP_Camera::init() {
    clockPin.init();
    SIPin.init();

    if (type == NXP_Camera::Type::BOTH) {
        adc.appendSample(&sampleCamera1);
        adc.appendSample(&sampleCamera1);
        adc.appendSample(&sampleCamera1);
        adc.appendSample(&sampleCamera1);

        adc.appendSample(&sampleCamera2);
        adc.appendSample(&sampleCamera2);
        adc.appendSample(&sampleCamera2);
        adc.appendSample(&sampleCamera2);
    } else if (type == NXP_Camera::Type::CAMERA_1) {
        for (uint8_t i = 0; i < 8; i ++) {
            adc.appendSample(&sampleCamera1);
        }
    } else if (type == NXP_Camera::Type::CAMERA_2) {
        for (uint8_t i = 0; i < 8; i ++) {
            adc.appendSample(&sampleCamera2);
        }
    }

    adc.init();
    clockPin.reset();
    SIPin.reset();
};


void NXP_Camera::adcInterruptEndOfMeasurementStatic(uint8_t) {
    cameraHandler->adcInterruptEndOfMeasurement();
}

void NXP_Camera::pitInterruptStatic(uint8_t) {
    cameraHandler->pitInterrupt();
}

void NXP_Camera::adcInterruptEndOfMeasurement() {
    Kitty::kitty().ledLine.at(0).toggle();
    if (type == NXP_Camera::Type::BOTH) {
        uint16_t* data = adc.getBufferValues(sampleCamera1.converterType);
        uint32_t result = 0;
        for (uint8_t i = 0; i < 4; i++) {
            result += data[i];
        }
        buffer1Data[currentPixelIndex] = result / 4;
        result = 0;
        for (uint8_t i = 4; i < 8; i++) {
            result += data[i];
        }
        buffer2Data[currentPixelIndex] = result / 4;
    } else if (type == NXP_Camera::Type::CAMERA_1) {
        uint16_t* data = adc.getBufferValues(sampleCamera1.converterType);
        uint32_t result = 0;
        for (uint8_t i = 0; i < 8; i++) {
            result += data[i];
        }
        buffer1Data[currentPixelIndex] = result / 8;
    } else if (type == NXP_Camera::Type::CAMERA_2) {
        uint16_t* data = adc.getBufferValues(sampleCamera2.converterType);
        uint32_t result = 0;
        for (uint8_t i = 0; i < 8; i++) {
            result += data[i];
        }
        buffer2Data[currentPixelIndex] = result / 8;
    }
}

void NXP_Camera::pitInterrupt() {
//    Kitty::kitty().ledLine.at(0).toggle();
static uint8_t waitEnd = 0;
    if (cameraState == CameraState::START) {
        cameraState = CameraState::SET_SI_PIN;
    }
    if (cameraState == CameraState::SET_SI_PIN) {
        SIPin.set();
        cameraState = CameraState::SET_FIRST_CLOCK_PIN;
    } else if (cameraState == CameraState::SET_FIRST_CLOCK_PIN) {
        clockPin.set();
        cameraState = CameraState::RESET_SI_PIN;
    } else if (cameraState == CameraState::RESET_SI_PIN) {
        SIPin.reset();
        cameraState = CameraState::RESET_FIRST_CLOCK_PIN;
    } else if (cameraState == CameraState::RESET_FIRST_CLOCK_PIN) {
        clockPin.reset();
        currentPixelIndex = -1; // because increment is next
        waitEnd = 0;
        cameraState = CameraState::WAIT_1;
    } else if (cameraState == CameraState::WAIT_1) {
        cameraState = CameraState::WAIT_2;
    } else if (cameraState == CameraState::WAIT_2) {

        cameraState = CameraState::SET_CLOCK_PIN;
    } else if (cameraState == CameraState::SET_CLOCK_PIN) {
        currentPixelIndex++;
        clockPin.set();

        if (currentPixelIndex == 128) {
            cameraState = CameraState::STOPPED;
            clockPin.reset();
            cameraState = CameraState::WAIT_END_1;
        } else {
            cameraState = CameraState::RESET_CLOCK_PIN;
        }
    } else if (cameraState == CameraState::RESET_CLOCK_PIN) {
        adc.startConversion();
        clockPin.reset();
        cameraState = CameraState::SET_CLOCK_PIN;
    } else if (cameraState == CameraState::WAIT_END_1) {
        waitEnd++;
        if (waitEnd > 7) {
            waitEnd = 0;
            cameraState = CameraState::START;
        }
    }
}
