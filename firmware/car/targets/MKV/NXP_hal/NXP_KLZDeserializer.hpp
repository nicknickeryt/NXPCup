#include <stddef.h>
#include <stdint.h>
#include <span>

#pragma once

class NXP_KLZDeserializer {
  private:
    uint8_t startFlag[4] = {255, 255, 255, 255};
    uint8_t dataBuffer[256];
    size_t dataBufferIndex = 0;

    enum class State {
        RECEIVING_DATA,
        WAITING_FOR_START_FLAG,
        WAITING_FOR_DATA,
    } state = State::WAITING_FOR_START_FLAG;


  public:
    NXP_KLZDeserializer() {}

    void deserialize(uint8_t* data, size_t length);

  private:
    void handleWaitingForStartFlag(uint8_t* data, size_t length);
    void handleReceivingData(int8_t* data, size_t length);
  };