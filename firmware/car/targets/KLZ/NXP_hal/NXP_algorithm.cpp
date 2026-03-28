#include "NXP_algorithm.hpp"

#define LOG_CHANNEL             KITTY
#define KITTY_LOG_CHANNEL       2
#define KITTY_LOG_CHANNEL_LEVEL LOG_LEVEL_DEBUG

#include "logger.h"

#include "printf.h"

void Algorithm::init() {
  // Reset devices
  enable.reset();
  delay_ms(100);
  enable.set();

  sensor.init();
  log_debug("Algorithm init");
}



void Algorithm::proc(volatile bool &trigger) {
  
  if(trigger) {
    trigger = false;

    uint16_t distance = sensor.getDistance();
    uint8_t data[dataSize];

    if (distance <= sendThreshold) {
      data[0] = static_cast<uint8_t>((distance >> 8) & 0xFF);
      data[1] = static_cast<uint8_t>(distance & 0xFF);
      
      // log_debug("Distance send: %d", distance);
    }

    // log_debug("Distance over threshold: %d", distance);
    
    frame.send(data);
  }
}