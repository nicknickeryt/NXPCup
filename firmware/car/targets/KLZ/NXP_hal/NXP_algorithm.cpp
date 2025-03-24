#define LOG_CHANNEL             KITTY
#define KITTY_LOG_CHANNEL       2
#define KITTY_LOG_CHANNEL_LEVEL LOG_LEVEL_NOTICE

#include "logger.h"

#include "NXP_algorithm.hpp"

void Algorithm::init() {
  center.init();
  log_debug("Algorithm init");
}

void Algorithm::proc(volatile bool &trigger) {
  uint16_t distance = center.getDistance();
  
  if(trigger) {
    log_debug("Algorithm proc");
    uint8_t data[sensorNumber];

    if (distance <= sendThreshold) {
      data[0] = distance;
      ledDebug.at(0).set();
    }

    frame.send(data);
    trigger = false;
  }
}