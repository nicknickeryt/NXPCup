#include "NXP_camera.hpp"

void NXP_Camera::init(){
    clockPin.init();
    SIPin.init();

    clockPin.reset();
    SIPin.reset();
};

void NXP_Camera::dummy(void){
    //SIPin.toggle();
}
