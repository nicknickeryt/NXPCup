#include "NXP_sensors.hpp"
#include "HALina.hpp"

#include <math.h>

#define LOG_CHANNEL               SENSORS
#define SENSORS_LOG_CHANNEL       3
#define SENSORS_LOG_CHANNEL_LEVEL LOG_LEVEL_DEBUG

#include "logger.h"

enum {
  SENSOR_MIN_VALUE = 50,
  SENSOR_MAX_VALUE = 300,
};

uint16_t NXP_Sensors::filterMeasurement(uint16_t value) {
    if (value < SENSOR_MIN_VALUE || value > SENSOR_MAX_VALUE) {
        value = static_cast<uint16_t>(SENSOR_MAX_VALUE);
    } 
    
    filter.average = static_cast<uint16_t>(filter.average * (1 - filter.alpha) + value * filter.alpha);
    filter.lastValue = abs(value - filter.average) < filter.delta ? value : static_cast<uint16_t>(SENSOR_MAX_VALUE);
    return filter.lastValue;
}

NXP_Sensors::NXP_Sensors(NXP_I2C& i2c) : i2c(i2c), device(i2c) {}

void NXP_Sensors::init() {
    if (!device.init()) {
        log_error("VL53L0X sensor initialization failed!");
        delay_ms(1000);
        NVIC_SystemReset();
    }
    log_debug("VL53L0X sensor initailized corectly");

    // Initalize filter
    filter.alpha     = 0.1f;
    filter.delta     = 100;
    filter.average   = 0;
    filter.lastValue = 0;
}

uint16_t NXP_Sensors::getDistance() {
    return filterMeasurement(device.readRangeSingleMillimeters());
}