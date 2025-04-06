#include <stddef.h>
#include <stdint.h>
#include <string.h>

#pragma once

typedef void (*OnDataReceived)(uint8_t* data, size_t length);

class UART_Frame {
  private:
    constexpr static uint8_t startOfFrame[] = {0xFF, 0xFF, 0xFF, 0xFF};
    constexpr static uint8_t endOfFrame = 0xFF;

    uint8_t dataBuffer[256] = {0};
    size_t dataCount = 0;

    size_t frameIndex = 0;    

    enum class State {
        RECEIVING_HEADER,
        RECEIVING_DATA,
    } state = State::RECEIVING_HEADER;

    OnDataReceived onDataReceived;
  
  public:
    UART_Frame(OnDataReceived onDataReceived) : onDataReceived(onDataReceived) {}
    void deserialize(const uint8_t* data, size_t length);

  private:
    void handleHeaderReception(uint8_t data);
    void handleIncomingDataReception(uint8_t data);
  };