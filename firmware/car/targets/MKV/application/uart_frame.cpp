#include "uart_frame.hpp"

void UART_Frame::handleHeaderReception(uint8_t data) {
  if (frameIndex == sizeof(startOfFrame)) {
    frameIndex = 0;
    return;
  }
  
  if (startOfFrame[frameIndex] == data) {
    frameIndex++;

    // Header matched, switch to data reception state
    if (frameIndex == sizeof(startOfFrame)) {
      state = State::RECEIVING_DATA;
    }
  } else {
    // Reset if the header not matching
    frameIndex = 0;
  }
}

void UART_Frame::handleIncomingDataReception(uint8_t data) {
  if (dataCount == sizeof(dataBuffer)) {
    dataCount = 0;
    state = State::RECEIVING_HEADER;
    return;
  }

  if(data == endOfFrame) {
    // Callback
    onDataReceived(dataBuffer, dataCount);
    // Go to header reception
    state = State::RECEIVING_HEADER;
    ++frameIndex;
    dataCount = 0;
  } else {
    // Store incoming data
    dataBuffer[dataCount] = data;
    ++dataCount;
  }
}

void UART_Frame::deserialize(const uint8_t* data, size_t length) {
  for(size_t i = 0; i != length; ++i) {
    switch (state) {
      case State::RECEIVING_HEADER:
        handleHeaderReception(data[i]);
        break;
      case State::RECEIVING_DATA:
        handleIncomingDataReception(data[i]);
        break;
      default:
        break;
    }
  }
}
