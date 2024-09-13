#include "algorithm.hpp"

#include <assert.h>

enum {
  HALF_CAMERA_WIDTH = 63,
};

static void lowPassFilter(uint16_t *data, uint32_t size) {
  uint16_t previousData = 0;
  const float alpha = 0.9f;
  for (auto i = 0; i < size; i++) {
    uint16_t temp = data[i];
    data[i] = (1 - alpha) * data[i] + (alpha) * previousData;
    previousData = temp;
  }
}

int32_t Algorithm::calculatePosition(uint16_t *data) {
    assert(data != nullptr);

    uint32_t imageWindow = 10;
    uint32_t bightnessBias = 5000;

    // Apply low pass filter on camera image
    if (lowPassFilterEnabled) {
        lowPassFilter(data, 128);
    }

    // Calculate average brightness of the left half of the image
    float average = 0;
    for (auto i = imageWindow; i < HALF_CAMERA_WIDTH - imageWindow; i++) {
        average += (static_cast<float>(data[i]) / 63.0f);
    }
    uint16_t averageInt = static_cast<uint16_t>(average);
    averageInt += bightnessBias;

    // Calculate the distance from the center of the image
    uint16_t leftSideDelta = 0;
    uint16_t rightSideDelta = 0;
    for (auto i = imageWindow; i < HALF_CAMERA_WIDTH - imageWindow; i++) {
        if (data[i] < averageInt) {
        leftSideDelta = i - imageWindow;
        }
        if (data[2 * HALF_CAMERA_WIDTH - i] < averageInt) {
        rightSideDelta = i - imageWindow + 1;
        }
    }

    return leftSideDelta - rightSideDelta;
}

int32_t Algorithm::proc(uint16_t *data) {
  assert(data != nullptr);
  // Calculate the position of the vehicle based on the camera image
  int32_t position = calculatePosition(data);
  return pid.calculate(0, position);
}
