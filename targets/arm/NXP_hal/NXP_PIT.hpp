#pragma once

#include "HALina.hpp"
#include <device/MKV58F24.h>
#include <functional>
#include <utility>
#include <drivers/fsl_clock.h>

class NXP_PIT {
private:
    uint8_t channel;
    uint32_t frequency;
    bool callbackFunctionStatus = false;

public:
    static void (*handlers[4])(uint8_t);
    enum class CHANNEL : uint8_t {
        _0 = 0,
        _1 = 1,
        _2 = 2,
        _3 = 3
    };

    static void enable();
    static void disable();

    NXP_PIT(CHANNEL channel, uint32_t frequency, void(*callbackFunction)(uint8_t) ) : channel(static_cast<uint8_t>(channel)), frequency(frequency) {
        if (callbackFunction) {
            handlers[static_cast<uint8_t >(channel)] = callbackFunction;
            callbackFunctionStatus = true;
        }
    }

    bool init();

    void channelDisable();

    void channelEnable();
};





