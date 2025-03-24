#include "NXP_sensors.hpp"
#include "HALina.hpp"

#include <math.h>

#define LOG_CHANNEL               SENSORS
#define SENSORS_LOG_CHANNEL       3
#define SENSORS_LOG_CHANNEL_LEVEL LOG_LEVEL_DEBUG

#include "logger.h"

enum {
  SENSOR_MAX_VALUE = 8190,
};

uint16_t NXP_Sensors::filterMeasurement(uint16_t value) {
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
    filter.alpha     = 0.5f;
    filter.delta     = 100;
    filter.average   = 0;
    filter.lastValue = 0;
}

uint16_t NXP_Sensors::getDistance() {
    uint16_t distance = filterMeasurement(device.readRangeSingleMillimeters());

    log_debug("Sensor mesurament: %" PRIu16, distance);
    return distance;
}