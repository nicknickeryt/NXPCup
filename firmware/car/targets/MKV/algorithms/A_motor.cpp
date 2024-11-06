#include "A_motor.hpp"
#include <utility> 

/*
*@brief set velocity adequate to current conditions on track
*@param startVelocity is the default speed of the car
*@param position is the current position of the car on the track
*/

Differential::Differential(float startVelocityValue) : startVelocity(startVelocityValue) {}

void Differential::proc(int position) { 
        //valueLeft = startVelocity;
        //valueRight = startVelocity;

        if(position < 0){
            valueRight = (startVelocity*(ENGINE_CUT + (position)))/ENGINE_CUT;
            valueLeft  = (startVelocity*(ENGINE_BREAKE + (position)))/ENGINE_BREAKE;
        }
        else if(position > 0){
            valueLeft  =  (startVelocity*(ENGINE_CUT - (position)))/ENGINE_CUT;
            valueRight =  (startVelocity*(ENGINE_BREAKE - (position)))/ENGINE_BREAKE;
        }
}
float Differential::getLeft() { return valueLeft; }
float Differential::getRight() { return valueRight; }
void Differential::setStartVelocity(float value) { startVelocity = value; }
float Differential::getStartVelocity() { return startVelocity; }

