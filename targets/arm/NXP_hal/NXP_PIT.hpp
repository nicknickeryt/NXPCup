#pragma once

#include <device/MKV58F24.h>
#include <functional>
#include <utility>
#include <drivers/fsl_clock.h>

class NXP_PIT {
    uint8_t channel;
    uint32_t interval;
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

    NXP_PIT(CHANNEL channel, uint32_t interval, std::function<void(void)> callback_function ) : channel(static_cast<uint8_t>(channel)), interval(interval) {
        handlers.at(static_cast<uint8_t >(channel)) = std::move(callback_function);
    }

    void init();

    void disable();

    void enable();
};





