#include "NXP_I2C.hpp"
#include "NXP_frame.hpp"
#include "NXP_gpio.hpp"
#include "NXP_sensors.hpp"
#include "NXP_uart.hpp"
#include "VL53L0X.h"

#include <array>

#pragma once

class Algorithm {
    constexpr static auto sendThreshold = 250;
    constexpr static auto sensorNumber  = 1;

  private:
    NXP_Sensors sensor;
    NXP_Frame   frame;
    NXP_GPIO    enable;

  public:
    Algorithm(NXP_I2C& i2c, NXP_GPIO enable, NXP_Uart& toMKV) : sensor(i2c), frame(toMKV, sensorNumber), enable(enable) {}

    void init();

    void proc(volatile bool&);
};
