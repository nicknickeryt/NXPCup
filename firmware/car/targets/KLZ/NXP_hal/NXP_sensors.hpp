#include "NXP_I2C.hpp"
#include "VL53L0X.h"

#pragma once

typedef struct {
  uint16_t average;
  uint16_t lastValue;
  float delta;
  float alpha;
} Filter;

class NXP_Sensors {
  private:
    NXP_I2C& i2c;
    VL53L0X device;
    Filter filter;
    uint16_t filterMeasurement(uint16_t value);

  public:
    NXP_Sensors(NXP_I2C& i2c);

    void init();
    uint16_t getDistance();
};
