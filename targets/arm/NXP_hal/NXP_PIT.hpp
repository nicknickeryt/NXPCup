#pragma once

#include <device/MKV58F24.h>
#include <functional>
#include <utility>

constexpr uint32_t maxPITCounter = 24000000;

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

    static void PITEnable() {
        SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;       // Enable clock to the PIT
        PIT->MCR = 0x00U;                       // turn on PIT
    }

    static void PITDisable() {
        SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;       // Enable clock to the PIT
        PIT->MCR |= PIT_MCR_MDIS_MASK;          // turn on PIT
    }

    NXP_PIT(CHANNEL channel, uint32_t interval, std::function<void(void)> callback_function ) : channel(static_cast<uint8_t>(channel)), interval(interval) {
        handlers.at(static_cast<uint8_t >(channel)) = std::move(callback_function);
    }

    void init() {
        PITEnable();
        PIT->CHANNEL[channel].LDVAL = (maxPITCounter / interval) - 1; // set timer counting
        PIT->CHANNEL[channel].TCTRL = 0;
        enable();

        switch (channel) {
            case 0:
                NVIC_ClearPendingIRQ(PIT0_IRQn);
                NVIC_EnableIRQ(PIT0_IRQn);
                break;
            case 1:
                NVIC_ClearPendingIRQ(PIT1_IRQn);
                NVIC_EnableIRQ(PIT1_IRQn);
                break;
            case 2:
                NVIC_ClearPendingIRQ(PIT2_IRQn);
                NVIC_EnableIRQ(PIT2_IRQn);
                break;
            case 3:
                NVIC_ClearPendingIRQ(PIT3_IRQn);
                NVIC_EnableIRQ(PIT3_IRQn);
                break;
        }
    }

    void disable() {
        PIT->CHANNEL[channel].TCTRL &= ~PIT_TCTRL_TIE_MASK; // timer disable interrupt
        PIT->CHANNEL[channel].TCTRL &= ~PIT_TCTRL_TEN_MASK; // timer disable
    }

    void enable() {
        PIT->CHANNEL[channel].TCTRL |= PIT_TCTRL_TIE_MASK; // timer enable interrupt
        PIT->CHANNEL[channel].TCTRL |= PIT_TCTRL_TEN_MASK; // timer enable
    }

};





