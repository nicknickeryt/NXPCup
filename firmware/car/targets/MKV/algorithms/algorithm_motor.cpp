#include "algorithm_motor.hpp"
#include <utility>
#define A 3600.0 //enter the square of engine cut value based on camera algorithm return value (float)
                // be careful of what value you enter (A > max_position^2) 

/*
*@brief set velocity adequate to current conditions on track
*@param st_velocity is the default speed of the car
*@param position is the current position of the car on the track
*/

std::pair<float, float> differential(float st_velocity, int position){ 

        std::pair<float, float> ret;

        ret.first = st_velocity; //ret.first is left engine
        ret.second = st_velocity; //ret.second is right engine

        if(position < 0){
            ret.first = (st_velocity*(A - (position*position)))/A;
        }
        else if(position > 0){
            ret.second = (st_velocity*(A - (position*position)))/A;
        }

        return ret;

        }

