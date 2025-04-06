#include "NXP_algorithm.hpp"

#define LOG_CHANNEL             KITTY
#define KITTY_LOG_CHANNEL       2
#define KITTY_LOG_CHANNEL_LEVEL LOG_LEVEL_DEBUG

#include "logger.h"

void Algorithm::init() {
  // Reset devices
  enable.set();
  delay_ms(1000);
  enable.reset();

  sensor.init();
  log_debug("Algorithm init");
}

void Algorithm::proc(volatile bool &trigger) {
  
  if(trigger) {
    uint16_t distance = sensor.getDistance();
    uint8_t data[sensorNumber];

    if (distance <= sendThreshold) {
      data[0] = distance;
      log_debug("Distance send: %d", distance);
    }

    frame.send(data);
    trigger = false;
  }
}