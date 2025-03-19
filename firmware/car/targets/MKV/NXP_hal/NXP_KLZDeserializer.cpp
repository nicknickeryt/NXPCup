#include "NXP_KLZDeserializer.hpp"

void NXP_KLZDeserializer::handleWaitingForStartFlag(uint8_t* data, size_t length) {

}

void NXP_KLZDeserializer::handleReceivingData(uint8_t* data, size_t length) {

}

void NXP_KLZDeserializer::deserialize(uint8_t* data, size_t length) {
    for (size_t i = dataBufferIndex; i < length; ++i) {
        switch (state) {
            case State::WAITING_FOR_START_FLAG:
                handleWaitingForStartFlag(data, length);
                break;
            case State::RECEIVING_DATA:
                handleReceivingData(data);
                break;
        }
    }
}