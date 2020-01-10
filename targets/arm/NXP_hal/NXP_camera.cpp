#define LOG_CHANNEL CAMERA
#define CAMERA_LOG_CHANNEL 2
#define CAMERA_LOG_CHANNEL_LEVEL LOG_LEVEL_DEBUG

#include "NXP_camera.hpp"
#include "logger.h"

NXP_Camera* nxpCamera0Handler;
NXP_Camera* nxpCamera1Handler;

void camera0Callback(){
    // check if it is time to generate SI impulse
    if(nxpCamera0Handler->clockPeriodsCounter == nxpCamera0Handler->clockPeriodsToSIGeneration){
        nxpCamera0Handler->clockPeriodsCounter = 0;
        nxpCamera0Handler->SIPin.set();
    } else{
        nxpCamera0Handler->SIPin.reset();
    }
    nxpCamera0Handler->clockPeriodsCounter++;
    nxpCamera0Handler->adc.startConversion();
    static int x = 0;
    if(1000 == x++) {
        log_notice("adc value: %d", int32_t(nxpCamera0Handler->adc.getValue()));
        x = 0;
    }
}
void camera1Callback(){

}

NXP_Camera::NXP_Camera(CameraIndex index, uint32_t clockFrequencyInHz, uint32_t siFrequencyInHz, NXP_ADC& adc, NXP_GPIO& clockPin, NXP_GPIO& SIPin) :
                    index(index),
                    clockFrequencyInHz(clockFrequencyInHz),
                    siFrequencyInHz(siFrequencyInHz),
                    adc(adc),
                    clockPin(clockPin),
                    SIPin(SIPin){
    if(index == CameraIndex::CAMERA_0){
        nxpCamera0Handler = this;
    } else if(index == CameraIndex::CAMERA_1){
        nxpCamera1Handler = this;
    }
    clockPeriodsToSIGeneration = clockFrequencyInHz/siFrequencyInHz;
}

void NXP_Camera::init(){
    clockPin.init();
    SIPin.init();
    adc.init();
    clockPin.reset();
    SIPin.reset();
};

void NXP_Camera::dummy(uint8_t cameraIndex){
    if(cameraIndex == CAMERA_0){
        camera0Callback();
    } else if(cameraIndex == CAMERA_1){
        camera1Callback();
    }
}
