#pragma once

#include "HALina.hpp"
#include "MKV58F24.h"
#include <functional>
#include <utility>
#include "fsl_clock.h"

class MKV_PIT {
private:
    uint8_t channel;
    uint32_t frequency;
    void(*callbackFunctions[4])(uint32_t*);
    uint32_t* args[4] = {nullptr};
    uint8_t callbackNumber = 0;
public:
    enum class CHANNEL : uint8_t {
        _0 = 0,
        _1 = 1,
        _2 = 2,
        _3 = 3
    };

    static void enable();
    static void disable();

    MKV_PIT(CHANNEL channel, uint32_t frequency, void (*callbackFunction)(uint32_t *), uint32_t *arg) :
        channel(static_cast<uint8_t>(channel)), frequency(frequency) {
        appendCallback(callbackFunction, arg);
    }

    bool init();

    void channelDisable();

    void channelEnable();

    void appendCallback(void(*callbackFunction)(uint32_t*), uint32_t* arg) {
        if (callbackNumber == 4 || callbackFunction == nullptr) {
            return;
        }
        callbackFunctions[callbackNumber] = callbackFunction;
        args[callbackNumber] = arg;
        callbackNumber++;
    }

    void runCallback() {
        for(uint8_t i =0; i < callbackNumber; i++) {
            callbackFunctions[i](args[i]);
        }
    }
};




