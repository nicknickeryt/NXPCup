#include "NXP_sensors.hpp"
#include "NXP_frame.hpp"
#include "NXP_gpio.hpp"
#include "NXP_I2C.hpp"
#include "NXP_uart.hpp"
#include "VL53L0X.h"

#include <array>

#pragma once

class Algorithm {
    constexpr static auto sendThreshold = 250;
    constexpr static auto sensorNumber = 1; 

  private:
    NXP_Sensors              center;
    NXP_Frame                frame;
    std::array<NXP_GPIO, 4>& ledDebug;

  public:
    Algorithm(NXP_I2C &i2c, NXP_Uart& toMKV, std::array<NXP_GPIO, 4>& ledDebug) : center(i2c), frame(toMKV, sensorNumber), ledDebug(ledDebug) {}

    void init();

    void proc(volatile bool&);
};
