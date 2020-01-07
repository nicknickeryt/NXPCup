#pragma once

#include <device/MKV58F24.h>
#include <functional>
#include <utility>
#include <drivers/fsl_clock.h>

class NXP_PIT {
private:
    uint8_t channel;
    uint32_t interval;
    bool callbackFunctionStatus = false;

public:
    static std::array<std::function<void(void)>, 4> handlers;
    enum class CHANNEL : uint8_t {
        _0 = 0,
        _1 = 1,
        _2 = 2,
        _3 = 3
    };

    static void PITEnable();
    static void PITDisable();

    NXP_PIT(CHANNEL channel, uint32_t interval, std::function<void(void)> callbackFunction ) : channel(static_cast<uint8_t>(channel)), interval(interval) {
        if (callbackFunction) {
            handlers.at(static_cast<uint8_t >(channel)) = std::move(callbackFunction);
            callbackFunctionStatus = true;
        }
    }

    bool init();

    void disable();

    void enable();
};





