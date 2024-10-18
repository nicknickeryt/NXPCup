#include "algorithm.hpp"

#include <assert.h>

enum {
    HALF_CAMERA_WIDTH = 63,
};

#ifdef ALGORITHM_LOWPASS_EN
static void lowPassFilter(uint16_t* data, const uint32_t size) {
    uint32_t    previousData = 0;
    const float alpha        = 0.9f;
    for (uint32_t i = 0; i < size; i++) {
        uint32_t temp = data[i];
        data[i]       = (1 - alpha) * data[i] + (alpha)*previousData;
        previousData  = temp;
    }
}
#endif


int32_t Algorithm::calculatePosition(uint16_t* data) {
    assert(data != nullptr);

// Apply low pass filter on camera image
#ifdef ALGORITHM_LOWPASS_EN
    lowPassFilter(data, 128);
#endif

    // Calculate average brightness of the left half of the image
    float average = 0;
    for (auto i = imageCut; i < HALF_CAMERA_WIDTH - imageCut; i++) {
        average += (static_cast<float>(data[i]) / 63.0f);
    }
    uint32_t averageInt = static_cast<uint32_t>(average);
    averageInt += bightnessBias;

    // Calculate the distance from the center of the image
    uint32_t leftSideDelta  = 0;
    uint32_t rightSideDelta = 0;
    for (auto i = imageCut; i < HALF_CAMERA_WIDTH - imageCut; i++) {
        if (data[i] < averageInt) 
          leftSideDelta = i - imageCut;
        if (data[2 * HALF_CAMERA_WIDTH - i] < averageInt) 
          rightSideDelta = i - imageCut + 1;
    }

    return leftSideDelta - rightSideDelta;
}

int32_t Algorithm::proc(uint16_t* data) {
    assert(data != nullptr);
    // Calculate the position of the vehicle based on the camera image
    int32_t position = calculatePosition(data);
    return pid.calculate(0, position);
}
