#include "NXP_camera.hpp"

NXP_Camera* nxpCamera0Handler;
NXP_Camera* nxpCamera1Handler;

void camera0Callback(){
    nxpCamera0Handler->SIPin.toggle();
}
void camera1Callback(){

}

NXP_Camera::NXP_Camera(CameraIndex index, uint32_t clockFrequencyInHz, NXP_GPIO& clockPin, NXP_GPIO& SIPin) : index(index), clockFrequencyInHz(clockFrequencyInHz), clockPin(clockPin), SIPin(SIPin){
    if(index == CameraIndex::CAMERA_0){
        nxpCamera0Handler = this;
    } else if(index == CameraIndex::CAMERA_1){
        nxpCamera1Handler = this;
    }
}

void NXP_Camera::init(){
    clockPin.init();
    SIPin.init();

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
